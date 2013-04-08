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
#include "EditionEventManager.h"
#import "VisitorGatherProperties.h"
#include <time.h>
#include <iostream>
#import "Facade.h"

//@implementation FullPropertiesApple
//@end

static Model3DManager* model3DManager = NULL;
vue::Vue* mView = NULL;
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
    // pour avoir le delta time en secondes
    float delta = clock()-temps;
    delta/=1000.f;
    delta/=1000.f;
    ((Terrain*)mField)->animerTerrain(delta);
    temps = clock();
    mView->appliquerVue(1);
    ((Terrain*)mField)->renderField();  
}
- (id)init
{
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
    ((Terrain*)mField)->createRandomField("test");
    return self;
}

-(void)dealloc
{
    [mModel3DManager release];
    mModel3DManager = NULL;
    delete (Terrain*)mField;
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
        mView->zoomerIn();
    }
    else
    {
        mView->zoomerOut();
    }
    
}

-(void) deplacerSouris:(int)deplacementX :(int)deplacementY
{
    mView->deplacerXYSouris(deplacementX, deplacementY);
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
