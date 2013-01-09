//
//  OutilZoom.m
//  Editeur
//
//  Created by L Tremblay on 12-04-26.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "OutilZoom.h"
#import "DonneesFenetreEdition.h"
#import "VueOpenGL.h"
#import "Cadre.h"

@implementation OutilZoom

- (void)boutonGaucheEnfonceDansVueOpenGLAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{        
    [super boutonGaucheEnfonceDansVueOpenGLAvecPosition:position 
                                               etModifierFlags:modifierFlags
                                               etModele:donneesFenetreEdition];
    rectangleElastique = CGRectMake(boutonGaucheEnfoncePosition.x, boutonGaucheEnfoncePosition.y, 0, 0);    
}

- (void)boutonGaucheDeplaceAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    if (boutonGaucheEstEnfonceEtDepartDansVueOpenGL)
    {
        CGPoint coordonneesDansFenetreVirtuelle = [donneesFenetreEdition convertirPositionVueOpenGLversPositionFenetreVirtuelle:position];
        // Efface le rectangle elastique actuel
        [donneesFenetreEdition.vueOpenGL dessineRectangleElastique];
        rectangleElastique.size.width = coordonneesDansFenetreVirtuelle.x - boutonGaucheEnfoncePosition.x;
        rectangleElastique.size.height = coordonneesDansFenetreVirtuelle.y - boutonGaucheEnfoncePosition.y;
        // Dessine le nouveau rectangle elastique
        [donneesFenetreEdition.vueOpenGL dessineRectangleElastique];
    }     
}

- (void)boutonGaucheRelacheAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{    
    if (!boutonGaucheEstEnfonceEtDepartDansVueOpenGL)
        return;
    
    CGPoint coordonneesDansFenetreVirtuelle = [donneesFenetreEdition convertirPositionVueOpenGLversPositionFenetreVirtuelle:position];
    
    Cadre *cadreVirtuel = [[Cadre alloc] init];


    GLfloat gauche, droite, bas, haut;
    if (boutonGaucheEnfoncePosition.x < coordonneesDansFenetreVirtuelle.x)
    {
        gauche = boutonGaucheEnfoncePosition.x;
        droite = coordonneesDansFenetreVirtuelle.x;
    }
    else
    {
        gauche = coordonneesDansFenetreVirtuelle.x;        
        droite = boutonGaucheEnfoncePosition.x;
    }
    
    if (boutonGaucheEnfoncePosition.y < coordonneesDansFenetreVirtuelle.y)
    {
        bas = boutonGaucheEnfoncePosition.y;
        haut = coordonneesDansFenetreVirtuelle.y; 
    }
    else
    {
        bas = coordonneesDansFenetreVirtuelle.y; 
        haut = boutonGaucheEnfoncePosition.y;
    }
    [cadreVirtuel definirCoteGauche:gauche
                              droit:droite 
                                bas:bas 
                               haut:haut];
    
    if (modifierFlags & NSAlternateKeyMask)
        [donneesFenetreEdition zoomOutAvecCadreVirtuel:cadreVirtuel];
    else
        [donneesFenetreEdition zoomInAvecCadreVirtuel:cadreVirtuel];
    rectangleElastique.size = CGSizeMake(0, 0);
    [super boutonGaucheRelacheAvecPosition:position 
                                  etModifierFlags:modifierFlags
                                  etModele:donneesFenetreEdition];
}

@end
