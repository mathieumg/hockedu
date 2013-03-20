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
    ConteneurNoeuds *selectedNodes = new ConteneurNoeuds;
    ((Terrain*)mField)->getSelectedNodes(*selectedNodes);
    return selectedNodes->size();
    
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
    
    
    const char* cPath = [path cString];
    std::string targetPath = cPath;
    targetPath += "/test.xml";
    
    
    //FacadeModele::saveField();
    //((Terrain*)mField)
    RazerGameUtilities::SaveFieldToFile(targetPath, *((Terrain*)mField));
    ((Terrain*)mField)->libererMemoire();
    RazerGameUtilities::LoadFieldFromFile(targetPath, *((Terrain*)mField));
    
    NSURL *url = [NSURL URLWithString:@"http://httpbin.org/ip"];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    AFJSONRequestOperation *operation = [AFJSONRequestOperation JSONRequestOperationWithRequest:request success:^(NSURLRequest *request, NSHTTPURLResponse *response, id JSON) {
        NSLog(@"IP Address: %@", [JSON valueForKeyPath:@"origin"]);
    } failure:nil];
    
    [operation start];
    
}

@end
