//
//  OutilDeplacement.m
//  Editeur
//
//  Created by L Tremblay on 12-04-26.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "OutilDeplacement.h"
#import "DonneesFenetreEdition.h"

@implementation OutilDeplacement

- (void)boutonGaucheDeplaceAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    if (boutonGaucheEstEnfonceEtDepartDansVueOpenGL)
    {
        CGPoint coordonneesDansFenetreVirtuelle = [donneesFenetreEdition convertirPositionVueOpenGLversPositionFenetreVirtuelle:position];
        // Le point précédent utilisé sera toujours boutonGaucheEnfoncePosition car à chaque déplacement, on bouge la fenêtre virtuelle.
        [donneesFenetreEdition deplacerParSourisVueEnX:coordonneesDansFenetreVirtuelle.x - boutonGaucheEnfoncePosition.x etY:coordonneesDansFenetreVirtuelle.y - boutonGaucheEnfoncePosition.y];        
    }    
}

@end
