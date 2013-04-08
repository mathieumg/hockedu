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
