//
//  Model.m
//  Hockedu
//
//  Created by Mathieu Parent on 2013-03-07.
//
//

#import "Model.h"
#include "Terrain.h"
#include "VisiteurSelection.h"
#include "NoeudAbstrait.h"
#include "AFJSONRequestOperation.h"
#include "AFHTTPClient.h"
#include "VuePerspectiveOrbit.h"
#include "VuePerspectiveCiel.h"
#include "VuePerspectiveLibre.h"
#include "EditionEventManager.h"
#import "VisitorGatherProperties.h"
#include <time.h>
#include "GestionnaireAnimations.h"
#include <iostream>
#include "Utilitaire.h"
#import "Facade.h"
#include "Animation.h"

//@implementation FullPropertiesApple
//@end

static Model3DManager* model3DManager = NULL;
vue::Vue* mView = NULL;
std::deque<class RunnableField*> mRunnables;
bool canDOCenter = true;
Terrain* GlobalField = NULL;

void CenterCameraTerminatedCallback(Animation* pAnim)
{
    canDOCenter = true;
    if(GlobalField && mView)
    {
        Vecteur3 pos = mView->getOptimalPosition(GlobalField->GetTableWidth());
        vue::Camera& camera = mView->obtenirCamera();
        camera.assignerPosition(pos);
        camera.assignerPointVise(Vecteur3());
        camera.assignerDirectionHaut(Vecteur3(0,1,0));
    }
    
    [Facade enableCameras];
}

class RunnableField
{
public:
    virtual ~RunnableField(){}
    virtual void execute()=0;
};
class RunnableCenterCamera: public RunnableField
{
public:
    RunnableCenterCamera(Terrain* field)
    {
        mField = field;
    }
    virtual void execute()
    {
        if(canDOCenter && mField)
        {
            canDOCenter = false;
            [Facade disableCameras];
            mView->centrerCamera(mField->GetTableWidth(),1,CenterCameraTerminatedCallback);
        }
    }
    Terrain* mField;
};


void displayMessageCallback(const char* message)
{
    NSString* msg =  [NSString stringWithFormat:@"%s" , message];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:msg delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
    // optional - add more buttons:
    //[alert addButtonWithTitle:@"Yes"];
    [alert show];
}


void EditionEventCallback(EditionEventCodes pEvent)
{
    switch (pEvent) {
        case ENABLE_PUCK_CREATION:
            [Facade EnablePuckCreation];
            break;
        case DISABLE_PUCK_CREATION:
            [Facade DisablePuckCreation];
            break;
        case ENABLE_MALLET_CREATION:
            [Facade EnableMalletCreation];
            break;
        case DISABLE_MALLET_CREATION:
            [Facade DisableMalletCreation];
            break;
        case THERE_ARE_NODES_SELECTED:
            [Facade ThereAreNodesSelected];
            break;
        case THERE_ARE_NO_NODE_SELECTED:
            [Facade ThereAreNoNodesSelected];
            break;
        case CAN_UNDO:
            [Facade CanUndo];
            break;
        case CANNOT_UNDO:
            [Facade CannotUndo];
            break;
        case CAN_REDO:
            [Facade CanRedo];
            break;
        case CANNOT_REDO:
            [Facade CannotRedo];
            break;
        default:
            break;
    }
}

bool RenderNodeCallback(RazerKey key)
{
    if(model3DManager)
    {
        return [model3DManager renderObject:key];
    }
    return false;
}

@implementation Model


float temps = clock();

- (void)render
{
    while(!mRunnables.empty())
    {
        auto v = mRunnables.back();
        if(v)
        {
            v->execute();
            delete v;
        }
        mRunnables.pop_back();
        
    }
    
    
    // pour avoir le delta time en secondes
    float delta = clock()-temps;
    delta/=1000.f;
    GestionnaireAnimations::obtenirInstance()->animer(delta);
    delta/=1000.f;
    ((Terrain*)mField)->animerTerrain(delta);
    
    
    
    temps = clock();
    mView->appliquerVue(1);
    ((Terrain*)mField)->renderField();  
}
- (id)init
{
    utilitaire::mDisplayMessageCallback = displayMessageCallback;
    EditionEventManager::setEditionEventCallback(EditionEventCallback);
    mField = new Terrain(NULL);
    mModel3DManager = [[Model3DManager alloc]init];
    model3DManager = mModel3DManager;
    mView = new vue::VuePerspectiveOrbit(
            vue::Camera(Vecteur3(0,-0.0001f,300), Vecteur3(0,0,0),
                       Vecteur3(0,1,0),Vecteur3(0,1,0)),
                                         0,400,0,400,
                                         180,50000,10,15000,1.25f,
                                         -150,150,-150,150);
    
    ((Terrain*)mField)->setModelManagerObjc(RenderNodeCallback);
    ((Terrain*)mField)->creerTerrainParDefaut("test");
    GlobalField = ((Terrain*)mField);
    return self;
}

-(void)dealloc
{
    while(!mRunnables.empty())
    {
        auto v = mRunnables.back();
        if(v)
        {
            delete v;
        }
        mRunnables.pop_back();
        
    }
    
    
    [mModel3DManager release];
    mModel3DManager = NULL;
    delete (Terrain*)mField;
    GlobalField = NULL;
    delete mView;
    [super dealloc];
}

-(int)acceptSelectionVisitor:(float)positionMinX :(float)positionMinY :(float)positionMaxX :(float) positionMaxY
{
    Vecteur3 posMin;
    Vecteur3 posMax;
    mView->convertirClotureAVirtuelle(positionMinX, positionMinY, posMin);
    mView->convertirClotureAVirtuelle(positionMaxX, positionMaxY, posMax);
    return ((Terrain*)mField)->selectNodes((Vecteur2)posMin,(Vecteur2)posMax,false);
}

-(int) beginModification:(FieldModificationStrategyType)type :(CGPoint)coordVirt
{
    FieldModificationStrategyEvent event;
    Vecteur3 pos;
    mView->convertirClotureAVirtuelle(coordVirt.x, coordVirt.y, pos);
    event.mPosition = pos;
    event.mType = FIELD_MODIFICATION_EVENT_CLICK;
    
    return ((Terrain*)mField)->BeginModification(type, event);
}

-(int) eventModification:(FieldModificationStrategyEventType)type :(CGPoint)coordVirt
{
    FieldModificationStrategyEvent event;
    event.mType = type;
    Vecteur3 pos;
    mView->convertirClotureAVirtuelle(coordVirt.x, coordVirt.y, pos);
    event.mPosition = pos;
    return ((Terrain*)mField)->ReceiveModificationEvent(event);
}

-(int) endModification
{
    return ((Terrain*)mField)->EndModification();
}

-(void) eventCancel;
{
    ((Terrain*)mField)->cancelModification();
}

-(void) duplicateSelection
{
    ((Terrain*)mField)->duplicateSelection();
}

-(void) deleteSelection
{
    ((Terrain*)mField)->deleteSelectedNodes();
}

-(void) zoom:(float)scale
{
    if(scale>0)
    {
        mView->zoomerInSmooth();
    }
    else
    {
        mView->zoomerOutSmooth();
    }
    
}

-(void) deplacerSouris:(int)deplacementX :(int)deplacementY
{
    mView->deplacerXYSouris(deplacementX, deplacementY);
}


-(void) createCameraFixed
{
    [Facade disableCameras];
    GestionnaireAnimations::obtenirInstance()->viderAnimationCamera();
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    vue::Camera& cameraCourante = mView->obtenirCamera();
	mView->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);
    
	vue::VuePerspectiveCiel* nouvelleVue = new vue::VuePerspectiveCiel(
                                                                       cameraCourante,
                                                                       0, 400, 0, 400,
                                                                       180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
                                                                       -150, 150, -150, 150);
	
	nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));
    delete mView;
    mView = nouvelleVue;
    mRunnables.push_front(new RunnableCenterCamera((Terrain*)mField));
}
-(void) createCameraOrbit
{
    [Facade disableCameras];
    GestionnaireAnimations::obtenirInstance()->viderAnimationCamera();
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    vue::Camera& cameraCourante = mView->obtenirCamera();
	mView->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);
    
	vue::VuePerspectiveOrbit* nouvelleVue = new vue::VuePerspectiveOrbit(
                                                                         cameraCourante,
                                                                         0, 400, 0, 400,
                                                                         180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
                                                                         -150, 150, -150, 150);
	
	nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));
    delete mView;
    mView = nouvelleVue;
    mRunnables.push_front(new RunnableCenterCamera((Terrain*)mField));
}
-(void) createCameraFree
{
    [Facade disableCameras];
    GestionnaireAnimations::obtenirInstance()->viderAnimationCamera();
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    vue::Camera& cameraCourante = mView->obtenirCamera();
	mView->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);
    
    vue::VuePerspectiveLibre* nouvelleVue = new vue::VuePerspectiveLibre(
                                                                         cameraCourante,
                                                                         0, 300, 0, 400,
                                                                         180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
                                                                         -150, 150, -150, 150);
	
	nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));
    delete mView;
    mView = nouvelleVue;
}


-(void) orbit:(int)deplacementX :(int)deplacementY
{
    mView->rotaterXY(Vecteur2i(deplacementX,deplacementY));
}

-(void) resizeWindow:(int)minX :(int)minY :(int)maxX :(int)maxY
{
    mView->redimensionnerFenetre(Vecteur2i(minX,minY), Vecteur2i(maxX,maxY));
}

-(void) undo
{
    ((Terrain*)mField)->undoModification();
}
-(void) redo
{
    ((Terrain*)mField)->redoModification();
}

-(void) saveField
{
    if(((Terrain*)mField)->verifierValidite())
    {
    NSError* error;

    
    
    NSString *path;
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	path = [[paths objectAtIndex:0] stringByAppendingPathComponent:@"Hockedu"];
	if (![[NSFileManager defaultManager] fileExistsAtPath:path])	//Does directory already exist?
	{
		if (![[NSFileManager defaultManager] createDirectoryAtPath:path
									   withIntermediateDirectories:NO
														attributes:nil
															 error:&error])
		{
			NSLog(@"Create directory error: %@", error);
		}
	}
    
    NSLog(@"Documents directory: %@", [[NSFileManager defaultManager] contentsOfDirectoryAtPath:path error:&error]);
    
    
    const char* cPath = [path UTF8String];
    std::string targetPath = cPath;
    targetPath += "/test.xml";
    
    
    //FacadeModele::saveField();
    //((Terrain*)mField)
    RazerGameUtilities::SaveFieldToFile(targetPath, *((Terrain*)mField));
    ((Terrain*)mField)->libererMemoire();
    RazerGameUtilities::LoadFieldFromFile(targetPath, *((Terrain*)mField));
    
    
    NSURL *url = [NSURL URLWithString:@"http://hockedu.com"];
    AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:url];
    [httpClient setDefaultHeader:@"Accept" value:@"application/json"];
    [httpClient registerHTTPOperationClass:[AFJSONRequestOperation class]];
    NSDictionary *params = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"testies", @"username",
                            @"608b9a09de61fea254bbebdcadc0fe8c38ae2ccb", @"password",
                            nil];
    
    [httpClient postPath:@"/remote/authenticate" parameters:params success:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSLog(@"Error: %@", [responseObject valueForKeyPath:@"error"]);
        NSLog(@"Auth key: %@", [responseObject valueForKeyPath:@"auth_key"]);
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"[HTTPClient Error]: %@", error.localizedDescription);
    }];
    [httpClient release];
    }
}

// Point d'entre pour le menu de modification des proprietes
-(void) getProperties:(FullPropertiesApple*)prop
{
    FullProperties fullP;
    ((Terrain*)mField)->gatherSelectedNodeProperties(&fullP);
    //FullPropertiesApple *prop;
    
    prop->mFriction= fullP.mFriction;
    prop->mZoneEditionX= fullP.mZoneEditionX;
    prop->mZoneEditionY= fullP.mZoneEditionY;
    prop->mScale= fullP.mScale;
    prop->mAcceleration= fullP.mAcceleration;
    prop->mPositionX= fullP.mPositionX;
    prop->mPositionY= fullP.mPositionY;
    prop->mAttraction= fullP.mAttraction;
    prop->mAngle= fullP.mAngle;
    prop->mRebound= fullP.mRebound;
    prop->mMinBonusSpawnTime= fullP.mMinBonusSpawnTime;
    prop->mMaxBonusSpawnTime= fullP.mMaxBonusSpawnTime;
    prop->mRinkRebound1= fullP.mRinkRebound1;
    prop->mRinkRebound2= fullP.mRinkRebound2;
    prop->mRinkRebound3= fullP.mRinkRebound3;
    prop->mRinkRebound4= fullP.mRinkRebound4;
    prop->mRinkRebound5= fullP.mRinkRebound5;
    prop->mRinkRebound6= fullP.mRinkRebound6;
    prop->mRinkRebound7= fullP.mRinkRebound7;
    prop->mRinkRebound8= fullP.mRinkRebound8;
    prop->mPropertyFlagAssignment = fullP.mPropertyFlagAssignment;
    
//    return prop;
}
-(void) setProperties:(FullPropertiesApple*)prop;
{
    FullProperties fullP;
    fullP.mFriction = prop->mFriction;
    fullP.mZoneEditionX = prop->mZoneEditionX;
    fullP.mZoneEditionY = prop->mZoneEditionY;
    fullP.mScale = prop->mScale;
    fullP.mAcceleration = prop->mAcceleration;
    fullP.mPositionX = prop->mPositionX;
    fullP.mPositionY = prop->mPositionY;
    fullP.mAttraction = prop->mAttraction;
    fullP.mAngle = prop->mAngle;
    fullP.mRebound = prop->mRebound;
    fullP.mMinBonusSpawnTime = prop->mMinBonusSpawnTime;
    fullP.mMaxBonusSpawnTime = prop->mMaxBonusSpawnTime;
    fullP.mRinkRebound1 = prop->mRinkRebound1;
    fullP.mRinkRebound2 = prop->mRinkRebound2;
    fullP.mRinkRebound3 = prop->mRinkRebound3;
    fullP.mRinkRebound4 = prop->mRinkRebound4;
    fullP.mRinkRebound5 = prop->mRinkRebound5;
    fullP.mRinkRebound6 = prop->mRinkRebound6;
    fullP.mRinkRebound7 = prop->mRinkRebound7;
    fullP.mRinkRebound8 = prop->mRinkRebound8;
    fullP.mPropertyFlagAssignment = prop->mPropertyFlagAssignment;
    ((Terrain*)mField)->applySelectedNodeProperties(&fullP);
}
-(RazerKey) getSelectedNodesType
{
    return ((Terrain*)mField)->getSelectedNodeUniqueKey();
}

@end
