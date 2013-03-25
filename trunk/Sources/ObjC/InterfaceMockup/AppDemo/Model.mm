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
//#include <Box2D/Box2D.h>

@implementation Model


- (void)render
{
    ((Terrain*)mField)->renderField();
    
}
- (id)init
{
    //b2World* world = new b2World(b2Vec2(0,0));
    mField = new Terrain(NULL);
    ((Terrain*)mField)->createRandomField("test");
    ((Terrain*)mField)->setTableItemsSelection(true);
    ((Terrain*)mField)->deleteSelectedNodes();
    return self;
}

-(int)acceptSelectionVisitor:(float)positionMinX: (float)positionMinY:(float) positionMaxX:(float) positionMaxY
{
    Vecteur2 posMin = Vecteur2(positionMinX,positionMinY);
    Vecteur2 posMax = Vecteur2(positionMaxX,positionMaxY);
    VisiteurSelection visitor = VisiteurSelection(posMin,posMax);
    ((Terrain*)mField)->setTableItemsSelection(false);
    ((Terrain*)mField)->acceptVisitor(visitor);
    visitor.faireSelection();
    
    // Pop over controller pour modifier les proprietes
    return ((Terrain*)mField)->getSelectedNodes().size();;
    
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
