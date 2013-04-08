//
//  Facade.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-04-08.
//
//

#import "Facade.h"
#import "EAGLViewController.h"
static EAGLViewController* mViewController = NULL;


@implementation Facade


+(void)registerController:(void*)controller
{
    mViewController = controller;
}

+(NSArray *)listFileAtPath
{
    //-----> LIST ALL FILES <-----//
    NSLog(@"LISTING ALL FILES FOUND");
    NSMutableArray* matches = [[NSMutableArray alloc]init];
    NSString* item;
    int count;
    NSString *path;
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	path = [[paths objectAtIndex:0] stringByAppendingPathComponent:@"Hockedu"];
    NSArray *directoryContent = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:path error:NULL];

    
    for (item in directoryContent)
    {
        if ([[item pathExtension] isEqualToString:@"xml"])
        {
            [matches addObject:item];
        }
    }
    
    for (count = 0; count < (int)[matches count]; count++)
    {
        NSLog(@"File %d: %@", (count + 1), [matches objectAtIndex:count]);
    }
    
    
    return matches;
}

+(void)EnablePuckCreation
{
    [mViewController enablePuckCreation];
}
+(void)DisablePuckCreation
{
    [mViewController disablePuckCreation];
}
+(void)EnableMalletCreation
{
    [mViewController enableMalletCreation];
}
+(void)DisableMalletCreation
{
    [mViewController disableMalletCreation];
}
+(void)ThereAreNodesSelected
{
    [mViewController thereAreNodesSelected];
}
+(void)ThereAreNoNodesSelected
{
    [mViewController thereAreNoNodesSelected];
}
+(void)CanUndo
{
    [mViewController canUndo];
}
+(void)CannotUndo;
{
    [mViewController cannotUndo];
}
+(void)CanRedo
{
    [mViewController canRedo];
}
+(void)CannotRedo
{
    [mViewController cannotRedo];
}
+(void)disableCameras
{
    
}
+(void)enableCameras
{
    
}
@end
