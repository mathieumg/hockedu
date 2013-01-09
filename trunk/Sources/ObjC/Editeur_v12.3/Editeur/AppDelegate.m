//
//  AppDelegate.m
//  Editeur
//
//  Created by L Tremblay on 12-03-21.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"
#import "VueOpenGL.h"
#import "Noeud.h"
#import "NoeudComposite.h"
#import "NoeudSimple.h"
#import "ControleurFenetreOutils.h"
#import "ControleurFenetreEdition.h"
#import "DonneesEditeur.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
#if (DEBUG_NOTIFICATIONS)
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onNotification:) name:nil object:nil];
#endif

    // Creation du conteneur pour les instances de controleur de fenetre d'edition
    controleursFenetreEdition = [[NSMutableSet alloc] init];
    // Creation d'une fenetre d'edition
    [self creerFenetreEdition];
    // Creation de la fenetre d'outils
    controleurFenetreOutils = [[ControleurFenetreOutils alloc] initWithWindowNibName:@"FenetreOutils"];
    [controleurFenetreOutils showWindow:nil];
    
    [DonneesEditeur obtenirInstance].vueBoiteOutils = controleurFenetreOutils.vueBoiteOutils;
    
}

- (void)creerFenetreEdition
{
    // Creation du controleur et ajout de celui-ci dans le set.
    ControleurFenetreEdition *controleurFenetreEdition = [[ControleurFenetreEdition alloc] initWithWindowNibName:@"FenetreEdition"];
    [controleursFenetreEdition addObject:controleurFenetreEdition];    
    // Rend la fenetre visible
    [controleurFenetreEdition showWindow:nil];
    // Doit être après showWindow, sinon vueOpengl sera nil.
    [controleurFenetreEdition.vueOpenGL mettre_A_JourProjection];    
}

-(void)detruireFenetreEdition:(ControleurFenetreEdition *)controleurFenetreEdition
{
    // Demande de relacher le controleur de fenetre edition.
    [controleursFenetreEdition removeObject:controleurFenetreEdition];
}


// À modifier ici si jamais on veut vérifier si des éléments de menu sont légaux
// ou non.
- (BOOL) validateMenuItem:(NSMenuItem *)menuItem
{
    return YES;
}

- (void) newDocument:(id)sender
{
    [[NSApp delegate] creerFenetreEdition];
}

- (void) openDocument:(id)sender
{  
}

#if (DEBUG_NOTIFICATIONS)
-(void)onNotification:(NSNotification*)notification
{
    NSLog(@"Notification name is %@ sent by %@",[notification name], [[notification object] description] );
}
#endif

@end
