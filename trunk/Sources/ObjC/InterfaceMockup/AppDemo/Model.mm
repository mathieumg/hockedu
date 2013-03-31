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
#include "EditionEventManager.h"


static Model3DManager* model3DManager = NULL;
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



- (void)render
{    
    ((Terrain*)mField)->renderField();  
}
- (id)init
{
    EditionEventManager::setEditionEventCallback(EditionEventCallback);
    mField = new Terrain(NULL);
    mModel3DManager = [[Model3DManager alloc]init];
    model3DManager = mModel3DManager;
    
    ((Terrain*)mField)->setModelManagerObjc(RenderNodeCallback);
    ((Terrain*)mField)->createRandomField("test");
    return self;
}

-(void)dealloc
{
    [mModel3DManager release];
    mModel3DManager = NULL;
    delete (Terrain*)mField;
    [super dealloc];
}

-(int)acceptSelectionVisitor:(float)positionMinX: (float)positionMinY:(float) positionMaxX:(float) positionMaxY
{
    Vecteur2 posMin = Vecteur2(positionMinX,positionMinY);
    Vecteur2 posMax = Vecteur2(positionMaxX,positionMaxY);
    return ((Terrain*)mField)->selectNodes(posMin,posMax,false);
}

-(void) beginModification:(FieldModificationStrategyType)type:(CGPoint)coordVirt
{
    FieldModificationStrategyEvent event;
    event.mPosition[VX] = coordVirt.x;
    event.mPosition[VY] = coordVirt.y;
    event.mType = FIELD_MODIFICATION_EVENT_CLICK;
    
    ((Terrain*)mField)->BeginModification(type, event);
}

-(void) eventModification:(FieldModificationStrategyEventType)type:(CGPoint)coordVirt
{
    FieldModificationStrategyEvent event;
    event.mType = type;
    event.mPosition[VX] = coordVirt.x;
    event.mPosition[VY] = coordVirt.y;
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
    
}

@end
