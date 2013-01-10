//
//  OutilAjoutObjetSimple.m
//  Editeur
//
//  Created by L Tremblay on 12-05-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "OutilAjoutObjetSimple.h"
#import "DonneesFenetreEdition.h"

@implementation OutilAjoutObjetSimple

- (void) definirSousType:(NSString *) unSousType
{
    sousType = unSousType;
}

- (void)boutonGaucheRelacheAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition
{    
    [super boutonGaucheRelacheAvecPosition:position etModifierFlags:modifierFlags etModele:donneesFenetreEdition];
    CGPoint coordonneesDansFenetreVirtuelle = [donneesFenetreEdition convertirPositionVueOpenGLversPositionFenetreVirtuelle:position];    
    [donneesFenetreEdition ajouterObjetSimple:sousType avecPosition:coordonneesDansFenetreVirtuelle EtEchelle:1.0 EtAngle:0.0];
}


@end
