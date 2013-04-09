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
#import "HockeduAppDelegate.h"

//@implementation FullPropertiesApple
//@end

static Model3DManager* model3DManager = NULL;
vue::Vue* mView = NULL;
std::deque<class RunnableField*> mRunnables;
bool canDOCenter = true;
Terrain* GlobalField = NULL;
std::string mapName;
std::string mapDescription;
int mapPublic = 1;
int mapId = 0;
int userId;
std::string auth;

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
bool renderingRectangle = false;
Vecteur3 rectanglePos1,rectanglePos2;

-(void) startRectangle:(CGPoint*)startPos
{
    mView->convertirClotureAVirtuelle(startPos->x, startPos->y, rectanglePos1);
    rectanglePos1[2] = 10;
    rectanglePos2 = rectanglePos1;
    renderingRectangle = true;
}
-(void) moveRectangle:(CGPoint*)pos
{
    mView->convertirClotureAVirtuelle(pos->x, pos->y, rectanglePos2);
    rectanglePos2[2] = 10;
}
-(void) endRectangle
{
    renderingRectangle = false;
}


-(void) setMapFields:(NSString*)pMapName : (NSString*) pMapDescription : (int) pMapPublic
{
    mapName = [pMapName UTF8String];
    mapDescription = [pMapDescription UTF8String];
    mapPublic = pMapPublic;
}

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
    
    if(renderingRectangle)
    {
    Vecteur3 point3 = rectanglePos1;
    point3[VX] = rectanglePos2[VX];
    Vecteur3 point4 = rectanglePos2;
    point4[VX] = rectanglePos1[VX];
    
    
    float vertex[15];
    int count = 0;
    vertex[count++] = rectanglePos1[0];
    vertex[count++] = rectanglePos1[1];
    vertex[count++] = rectanglePos1[2];
    vertex[count++] = point3[0];
    vertex[count++] = point3[1];
    vertex[count++] = point3[2];
    vertex[count++] = rectanglePos2[0];
    vertex[count++] = rectanglePos2[1];
    vertex[count++] = rectanglePos2[2];
    vertex[count++] = point4[0];
    vertex[count++] = point4[1];
    vertex[count++] = point4[2];
    vertex[count++] = rectanglePos1[0];
    vertex[count++] = rectanglePos1[1];
    vertex[count++] = rectanglePos1[2];
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(0.0f,0.0f,0.0f,1.0f);
    glVertexPointer (3, GL_FLOAT , 0, vertex);
    glDrawArrays (GL_LINE_STRIP, 0, 5);
    glDisableClientState(GL_VERTEX_ARRAY);
    }

}
-(bool) verifierValiditeTerrain
{
    return GlobalField->verifierValidite();
}

+(void) saveLogin:(int)uid : (NSString*)authy
{
    userId = uid;
    auth = [authy UTF8String];
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

+(void) loadField:(NSString*) stringToParse
{
    TiXmlDocument doc;
    doc.Parse([stringToParse UTF8String],0,TIXML_ENCODING_UTF8);
    
    const XmlElement* root = (const XmlElement*)&doc;
    const XmlElement* config = XMLUtils::FirstChildElement(root,"Hockedu");
    std::string version;
    if(config && XMLUtils::readAttribute(config,"Version",version) )
    {
        root = config; //backward compatibility
    }
    GlobalField->initialiserXml(root);
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
        targetPath += "/" + mapName + ".xml";
        
        
        //FacadeModele::saveField();
        //((Terrain*)mField)
        RazerGameUtilities::SaveFieldToFile(targetPath, *((Terrain*)mField));
        
        
        
        
        GlobalField->modifierNom(mapName);
        
        XmlDocument document ;
        XMLUtils::CreateDocument(document);
        
        XmlElement* version = XMLUtils::createNode("Hockedu");
        XMLUtils::writeAttribute(version,"Version",XMLUtils::XmlFieldVersion);
        XMLUtils::LinkEndChild(document,version);
        
        // Creation du noeud du terrain
        XMLUtils::LinkEndChild(version,GlobalField->creerNoeudXML());
        
        TiXmlPrinter printer;
        
        document.mNode->Accept(&printer);
        NSString* xmlData = [NSString stringWithUTF8String:printer.CStr()];
        NSData *data = [xmlData dataUsingEncoding:NSUTF8StringEncoding];
        
        
        AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:[NSURL URLWithString:@"http://hockedu.com"]];
        [httpClient setParameterEncoding:AFFormURLParameterEncoding];
        [httpClient setDefaultHeader:@"Accept" value:@"application/json"];
        
        NSLog(@"%s", mapName.c_str());
        
        NSDictionary *params = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSString stringWithFormat:@"%d", mapId], @"map_id",
                                [NSString stringWithFormat:@"%d", userId], @"user_id",
                                [NSString stringWithUTF8String:auth.c_str()], @"auth_key",
                                [NSString stringWithUTF8String:mapName.c_str()], @"name",
                                [NSString stringWithUTF8String:mapDescription.c_str()], @"description",
                                [NSString stringWithFormat:@"%d", mapPublic], @"public",
                                nil];
        
        NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:@"/remote/sendmap" parameters:params constructingBodyWithBlock: ^(id <AFMultipartFormData>formData) {
            [formData appendPartWithFileData:data name:@"mapfile" fileName:@"xmlmap.xml" mimeType:@"text/xml"];
        }];
        
        /*
         
         NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:@"/remote/sendmap" parameters:@{@"map_id":[NSString stringWithFormat:@"%d", mapId],@"user_id":[NSString stringWithFormat:@"%d", userId],@"auth_key":auth,@"map_name":[NSString stringWithUTF8String:mapName.c_str()],@"map_description":[NSString stringWithUTF8String:mapDescription.c_str()],@"map_public":[NSString stringWithFormat:@"%d", mapPublic]} constructingBodyWithBlock: ^(id <AFMultipartFormData>formData) {
         [formData appendPartWithFileData:data name:@"mapfile" fileName:@"xmlmap.xml" mimeType:@"text/xml"];
         }];
         
         */
        
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [httpClient registerHTTPOperationClass:[AFHTTPRequestOperation class]];
        [httpClient setDefaultHeader:@"Accept" value:@"application/json"];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            // Print the response body in text
            NSLog(@"Response: %@", [[NSString alloc] initWithData:responseObject encoding:NSUTF8StringEncoding]);
            
            HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
            [delegate afficherVueAnimee];
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            //NSLog(@"Error: %@", error);
        }];
        [operation start];

        
        //((Terrain*)mField)->libererMemoire();
        //RazerGameUtilities::LoadFieldFromFile(targetPath, *((Terrain*)mField));
        
        /*
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
         */
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
