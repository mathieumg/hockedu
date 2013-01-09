//
//  AppDelegate.h
//  Editeur
//
//  Created by L Tremblay on 12-03-21.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VueOpenGL.h"
#import "ControleurFenetreOutils.h"

@class ControleurFenetreOutils;
@class ControleurFenetrePrincipale;
@class ControleurFenetreEdition;

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    NSMutableSet *controleursFenetreEdition;
    ControleurFenetreOutils *controleurFenetreOutils;
}

#if (DEBUG_NOTIFICATIONS)
-(void)onNotification:(NSNotification*)notification;
#endif

-(void)creerFenetreEdition;
-(void)detruireFenetreEdition:(ControleurFenetreEdition*)controleurFenetreEdition;


@end
