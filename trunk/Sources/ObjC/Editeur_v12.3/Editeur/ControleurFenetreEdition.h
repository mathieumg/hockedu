//
//  ControleurFenetreEdition.h
//  Editeur
//
//  Created by L Tremblay on 12-04-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VueOpenGL;
@class ControleurVueOpenGL;
@class DonneesFenetreEdition;

@interface ControleurFenetreEdition : NSWindowController
{
    IBOutlet VueOpenGL *vueOpenGL;
    IBOutlet DonneesFenetreEdition *donneesFenetreEdition;
    NSString *boutonObjetEnfonce;
}

@property(readonly) NSString *boutonObjetEnfonce;
@property(readonly) VueOpenGL *vueOpenGL;
@property CGSize ecartCoinSupDroitFenetreEtVueOpenGL;
@property CGPoint origineVueOpenGL;

-(void)boutonGaucheEnfonceDansVueOpenGL:(NSEvent *)theEvent;
-(void)boutonGaucheRelacheDansVueOpenGL:(NSEvent *)theEvent;

-(IBAction)charger:(id)sender;
-(IBAction)enregistrerSous:(id)sender;

@end
