//
//  ControleurFenetreEdition.m
//  Editeur
//
//  Created by L Tremblay on 12-04-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ControleurFenetreEdition.h"
#import "VueOpenGL.h"
#import "DonneesEditeur.h"
#import "DonneesFenetreEdition.h"
#import "AppDelegate.h"
#import "GLUT/glut.h"
#import "Outil.h"
@implementation ControleurFenetreEdition
@synthesize ecartCoinSupDroitFenetreEtVueOpenGL;
@synthesize origineVueOpenGL;
@synthesize boutonObjetEnfonce;
@synthesize vueOpenGL;

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


- (void)boutonGaucheEnfonceDansVueOpenGL: (NSEvent *)theEvent
{
    // Conversion des coordonnées de fenetre en coordonnées de la vue opengl.
    CGPoint coordonneesDansVueOpenGL = [vueOpenGL convertPoint:[theEvent locationInWindow] 
                                                      fromView:[[self window] contentView]];
    [donneesFenetreEdition boutonGaucheEnfonceDansVueOpenGLAvecPosition:coordonneesDansVueOpenGL etModifierFlags:[theEvent modifierFlags]];
}

- (void)mouseDragged:(NSEvent *)theEvent;
{
    // Conversion des coordonnées de fenetre en coordonnées de la vue opengl.    
    CGPoint coordonneesDansVueOpenGL = [vueOpenGL convertPoint:[theEvent locationInWindow] 
                                                      fromView:[[self window] contentView]];
    [donneesFenetreEdition boutonGaucheDeplaceAvecPosition:coordonneesDansVueOpenGL etModifierFlags:[theEvent modifierFlags]];    
}

-(void)boutonGaucheRelacheDansVueOpenGL:(NSEvent *)theEvent
{
    if (boutonObjetEnfonce != nil)
    {
        boutonObjetEnfonce = nil;
    }
    
}

- (void)mouseUp:(NSEvent *)theEvent
{
    // Conversion des coordonnées de fenetre en coordonnées de la vue opengl.    
    CGPoint coordonneesDansVueOpenGL = [vueOpenGL convertPoint:[theEvent locationInWindow] 
                                                      fromView:[[self window] contentView]];
    [donneesFenetreEdition boutonGaucheRelacheAvecPosition:coordonneesDansVueOpenGL etModifierFlags:[theEvent modifierFlags]];    
}

 - (void)scrollWheel:(NSEvent *)theEvent
{
    [donneesFenetreEdition rouletteSourisPivoteeAvecVariationEnY:[theEvent deltaY] etModifierFlags:[theEvent modifierFlags]];
}


-(IBAction)charger:(id)sender
{
    [donneesFenetreEdition importerEnXML];
}


-(IBAction)enregistrerSous:(id)sender
{
    [donneesFenetreEdition exporterEnXML];
}

- (void) keyDown:(NSEvent *)theEvent
{    
    unichar caractere = [[theEvent characters] characterAtIndex:0];
    
    switch (caractere) {
        case NSLeftArrowFunctionKey: // Fleche gauche
            [donneesFenetreEdition deplacerParClavierVueEnX:-1];
            break;
            
        case NSRightArrowFunctionKey: // Fleche droite
            [donneesFenetreEdition deplacerParClavierVueEnX:1];            
            break;
            
        case NSDownArrowFunctionKey: // Fleche bas
            [donneesFenetreEdition deplacerParClavierVueEnY:-1];            
            break;

        case NSUpArrowFunctionKey: // Fleche haut
            [donneesFenetreEdition deplacerParClavierVueEnY:1];            
            break;

        case '+': // Fleche bas
            [donneesFenetreEdition zoomAvecFacteur:0.5];            
            break;
            
        case '-': // Fleche haut
            [donneesFenetreEdition zoomAvecFacteur:2.0]; 
            break;            
            
        case 'd': // Pour dupliquer
            [donneesFenetreEdition dupliquerSelection];
            break;
            
        case 'e': // Pour effacer
            [donneesFenetreEdition effacerSelection];
            break;            
            
        default:
            break;
    }
    
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    // Pour la fenetre: trouver la difference entre la taille actuelle et la nouvelle
    // et trouver le nouveau ratio.
    // Demander a modifier la fenetre virtuelle en fonction de ces donnees.
    [donneesFenetreEdition changerTailleFenetreVirtuelleAvecEcart:CGSizeMake(frameSize.width - self.window.frame.size.width, frameSize.height - self.window.frame.size.height)];
    return frameSize;    
}

- (void)awakeFromNib
{
    origineVueOpenGL.x = vueOpenGL.frame.origin.x;
    origineVueOpenGL.y = vueOpenGL.frame.origin.y;
    ecartCoinSupDroitFenetreEtVueOpenGL.width = self.window.frame.size.width - vueOpenGL.frame.origin.x - vueOpenGL.frame.size.width;
    ecartCoinSupDroitFenetreEtVueOpenGL.height = self.window.frame.size.height - vueOpenGL.frame.origin.y - vueOpenGL.frame.size.height;
}


- (void) windowDidResize:(NSNotification *) aNotification
{  
    [vueOpenGL setFrame:CGRectMake(origineVueOpenGL.x, origineVueOpenGL.y,
                                   self.window.frame.size.width - ecartCoinSupDroitFenetreEtVueOpenGL.width - origineVueOpenGL.x,
                                   self.window.frame.size.height - ecartCoinSupDroitFenetreEtVueOpenGL.height - origineVueOpenGL.y)];       
    donneesFenetreEdition.ratioVueOpenGL = vueOpenGL.frame.size.width / vueOpenGL.frame.size.height;
    glViewport(0, 0, self.vueOpenGL.frame.size.width, self.vueOpenGL.frame.size.height);
    [vueOpenGL mettre_A_JourProjection];
}

- (void) windowWillClose:(NSNotification *) aNotification
{
    [[NSApp delegate] detruireFenetreEdition:self];
}


@end
