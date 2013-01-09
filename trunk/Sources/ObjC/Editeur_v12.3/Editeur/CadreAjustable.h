//
//  CadreAjustable.h
//  Editeur
//
//  Created by L Tremblay on 12-05-03.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Cadre.h"

@interface CadreAjustable : Cadre
{
    Cadre *cadreMaximal;
    Cadre *cadreMinimal;    
}
@property(strong, nonatomic) Cadre *cadreMaximal;
@property(strong, nonatomic) Cadre *cadreMinimal;

- (CadreAjustable *) initWithCoteGauche:(GLfloat)coteGauche droit:(GLfloat)coteDroit bas:(GLfloat)coteBas haut:(GLfloat)coteHaut etCadreMinimal:(Cadre *)unCadreMinimal unCadreMaximal:(Cadre *) unCadreMaximal;
- (void) mettre_A_Echelle:(CGFloat)facteur;

@end
