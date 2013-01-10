//
//  Cadre.h
//  Editeur
//
//  Created by L Tremblay on 12-05-03.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Cadre : NSObject
{
    GLfloat gauche;
    GLfloat droite;
    GLfloat bas;
    GLfloat haut;
    GLfloat largeur;
    GLfloat hauteur;
    GLfloat demiLargeur;
    GLfloat demiHauteur;
    GLfloat ratio;
    CGPoint centre;    
}

@property(readonly) GLfloat gauche;
@property(readonly) GLfloat droite;
@property(readonly) GLfloat bas;
@property(readonly) GLfloat haut;
@property(readonly) GLfloat largeur;
@property(readonly) GLfloat hauteur;
@property(readonly) GLfloat demiLargeur;
@property(readonly) GLfloat demiHauteur;
@property(readonly) GLfloat ratio;
@property(readonly) CGPoint centre;

- (Cadre *) initWithCoteGauche:(GLfloat)coteGauche droit:(GLfloat)coteDroit bas:(GLfloat)coteBas haut:(GLfloat)coteHaut;
- (void) definirCoteGauche:(GLfloat)coteGauche droit:(GLfloat)coteDroit bas:(GLfloat)coteBas haut:(GLfloat)coteHaut;
- (void) deplacerCentreAvecDeltaX:(GLfloat)deltaX etDeltaY:(GLfloat)deltaY;

@end
