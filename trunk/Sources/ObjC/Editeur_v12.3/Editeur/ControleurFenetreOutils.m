//
//  ControleurFenetreOutils.m
//  Editeur
//
//  Created by L Tremblay on 12-04-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ControleurFenetreOutils.h"
#import "DonneesEditeur.h"
#import "BoutonOutil.h"


@implementation ControleurFenetreOutils
@synthesize vueBoiteOutils;

- (void)awakeFromNib
{
    [self.window setHidesOnDeactivate:YES];
}

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (IBAction) selectionOutil:(id)sender
{
    [dernierBoutonEnfonce setState:NSOffState];
    [[DonneesEditeur obtenirInstance] selectionnerOutil:[(BoutonOutil *)sender nomOutil] 
                                           avecSousType:[(BoutonOutil *)sender sousType]];   
    dernierBoutonEnfonce = sender;
}


@end
