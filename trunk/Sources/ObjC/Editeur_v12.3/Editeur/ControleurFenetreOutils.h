//
//  ControleurFenetreOutils.h
//  Editeur
//
//  Created by L Tremblay on 12-04-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class VueBoiteOutils;

@interface ControleurFenetreOutils : NSWindowController
{
    NSButton *dernierBoutonEnfonce;
}
@property(strong, nonatomic) IBOutlet VueBoiteOutils *vueBoiteOutils;

- (IBAction) selectionOutil:(id)sender;

@end
