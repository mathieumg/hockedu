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
+(void)EnablePuckCreation
{
    [mViewController enablePuckCreation];
}
+(void)DisablePuckCreation
{
    
}
+(void)EnableMalletCreation
{
    
}
+(void)DisableMalletCreation
{
    
}
+(void)ThereAreNodesSelected
{
    
}
+(void)ThereAreNoNodesSelected
{
    
}
+(void)CanUndo
{
    
}
+(void)CannotUndo;
{
    
}
+(void)CanRedo
{
    
}
+(void)CannotRedo
{
    
}
@end
