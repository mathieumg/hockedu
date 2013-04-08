//
//  Facade.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-04-08.
//
//

@interface Facade : NSObject
{
}
+(void)registerController:(void*)controller;
+(void)EnablePuckCreation;
+(void)DisablePuckCreation;
+(void)EnableMalletCreation;
+(void)DisableMalletCreation;
+(void)ThereAreNodesSelected;
+(void)ThereAreNoNodesSelected;
+(void)CanUndo;
+(void)CannotUndo;
+(void)CanRedo;
+(void)CannotRedo;
@end
