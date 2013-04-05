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
#include <time.h>
#include <iostream>


static Model3DManager* model3DManager = NULL;
vue::Vue* mView = NULL;
void EditionEventCallback(EditionEventCodes pEvent)
{
    switch (pEvent) {
            
        case ENABLE_PUCK_CREATION:
            //NSLog(@"Can create Puck\n");
            break;
        case DISABLE_PUCK_CREATION:
            //NSLog(@"Cannot Create Puck\n");
            break;
        case ENABLE_MALLET_CREATION:
            //NSLog(@"Can create Mallet\n");
            break;
        case DISABLE_MALLET_CREATION:
            //NSLog(@"Cannot create puck\n");
            break;
        case THERE_ARE_NODES_SELECTED:
            //NSLog(@"There are items selected\n");
            break;
        case THERE_ARE_NO_NODE_SELECTED:
            //NSLog(@"There are no nodes selected\n");
            break;
        case CAN_UNDO:
            //NSLog(@"Can Undo modification\n");
            break;
        case CANNOT_UNDO:
            //NSLog(@"Cannot Undo modification\n");
            break;
        case CAN_REDO:
            //NSLog(@"Can Redo modification\n");
            break;
        case CANNOT_REDO:
            //NSLog(@"Cannot Redo modification\n");
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

-(void) beginModification:(FieldModificationStrategyType)type :(CGPoint)coordVirt
{
    FieldModificationStrategyEvent event;
    Vecteur3 pos;
    mView->convertirClotureAVirtuelle(coordVirt.x, coordVirt.y, pos);
    event.mPosition = pos;
    event.mType = FIELD_MODIFICATION_EVENT_CLICK;
    
    ((Terrain*)mField)->BeginModification(type, event);
}

-(void) eventModification:(FieldModificationStrategyEventType)type :(CGPoint)coordVirt
{
    FieldModificationStrategyEvent event;
    event.mType = type;
    Vecteur3 pos;
    mView->convertirClotureAVirtuelle(coordVirt.x, coordVirt.y, pos);
    event.mPosition = pos;
    ((Terrain*)mField)->ReceiveModificationEvent(event);
}

-(void) endModification
{
    ((Terrain*)mField)->EndModification();
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

@end
