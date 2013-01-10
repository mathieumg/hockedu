//
//  Cadre.m
//  Editeur
//
//  Created by L Tremblay on 12-05-03.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Cadre.h"

@implementation Cadre
@synthesize gauche, droite, bas, haut;

- (Cadre *) initWithCoteGauche:(GLfloat)coteGauche droit:(GLfloat)coteDroit bas:(GLfloat)coteBas haut:(GLfloat)coteHaut
{
    self = [super init];
    if (self != nil) {
        gauche = coteGauche;
        droite = coteDroit;
        bas = coteBas;
        haut = coteHaut;
    }
    return self;    
}

- (GLfloat) largeur
{
    return droite - gauche;
}

- (GLfloat) hauteur
{
    return haut - bas;
}

- (GLfloat) demiLargeur
{
    return self.largeur / 2;
}

- (GLfloat) demiHauteur
{
    return self.hauteur / 2;
}

- (CGPoint) centre
{
    return CGPointMake(gauche + self.demiLargeur, bas + self.demiHauteur);
}

- (GLfloat)ratio
{
    return self.largeur / self.hauteur;
}

- (void) deplacerCentreAvecDeltaX:(GLfloat)deltaX etDeltaY:(GLfloat)deltaY
{
    gauche -= deltaX;
    droite -= deltaX;
    bas -= deltaY;
    haut -= deltaY;
}



- (void) definirCoteGauche:(GLfloat)coteGauche droit:(GLfloat)coteDroit bas:(GLfloat)coteBas haut:(GLfloat)coteHaut
{
    gauche = coteGauche;
    droite = coteDroit;
    bas = coteBas;
    haut = coteHaut;
}



@end




























