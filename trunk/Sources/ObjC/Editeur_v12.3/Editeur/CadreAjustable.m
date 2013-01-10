//
//  CadreAjustable.m
//  Editeur
//
//  Created by L Tremblay on 12-05-03.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "CadreAjustable.h"

@implementation CadreAjustable
@synthesize cadreMinimal, cadreMaximal;

- (CadreAjustable *) initWithCoteGauche:(GLfloat)coteGauche droit:(GLfloat)coteDroit bas:(GLfloat)coteBas haut:(GLfloat)coteHaut etCadreMinimal:(Cadre *)unCadreMinimal unCadreMaximal:(Cadre *) unCadreMaximal
{
    self = [super initWithCoteGauche:coteGauche droit:coteDroit bas:coteBas haut:coteHaut];
    if (self != nil) {
        cadreMinimal = unCadreMinimal;
        cadreMaximal = unCadreMaximal;
    }
    return self;
}


- (void) deplacerCentreAvecDeltaX:(GLfloat)deltaX etDeltaY:(GLfloat)deltaY
{
    [super deplacerCentreAvecDeltaX:deltaX etDeltaY:deltaY];
}

- (void) mettre_A_Echelle:(CGFloat)facteur
{
    GLfloat coteGauche = self.centre.x - (self.largeur * facteur) / 2;
    GLfloat coteDroite = self.centre.x + (self.largeur * facteur) / 2;    
    GLfloat coteBas = self.centre.y - (self.hauteur * facteur) / 2;
    GLfloat coteHaut = self.centre.y + (self.hauteur * facteur) / 2;    
    [super definirCoteGauche:coteGauche
                       droit:coteDroite 
                         bas:coteBas 
                        haut:coteHaut];    
}

- (void) definirCoteGauche:(GLfloat)coteGauche droit:(GLfloat)coteDroit bas:(GLfloat)coteBas haut:(GLfloat)coteHaut
{
    [super definirCoteGauche:coteGauche droit:coteDroit bas:coteBas haut:coteHaut];
}


@end
