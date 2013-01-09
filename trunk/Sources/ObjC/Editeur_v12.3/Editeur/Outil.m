//
//  Outil.m
//  Editeur
//
//  Created by L Tremblay on 12-04-26.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Outil.h"
#import "DonneesFenetreEdition.h"


@implementation Outil
@synthesize rectangleElastique;

- (Outil *) init
{
    self = [super init];
    if (self != nil)
    {
        boutonGaucheEstEnfonceEtDepartDansVueOpenGL = NO;
    }
    return self;
}

- (void)boutonGaucheEnfonceDansVueOpenGLAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    CGPoint coordonneesDansFenetreVirtuelle = [donneesFenetreEdition convertirPositionVueOpenGLversPositionFenetreVirtuelle:position];
    boutonGaucheEstEnfonceEtDepartDansVueOpenGL = YES;
    boutonGaucheEnfoncePosition = coordonneesDansFenetreVirtuelle;
}

- (void)boutonGaucheDeplaceAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    CGPoint coordonneesDansFenetreVirtuelle = [donneesFenetreEdition convertirPositionVueOpenGLversPositionFenetreVirtuelle:position];
    boutonGaucheEnfoncePosition = coordonneesDansFenetreVirtuelle;    
}

- (void)boutonGaucheRelacheAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    boutonGaucheEstEnfonceEtDepartDansVueOpenGL = NO;
}

- (void)rouletteSourisPivoteeAvecVariationEnY:(CGFloat)variationY etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    if (boutonGaucheEstEnfonceEtDepartDansVueOpenGL)
        return;
    
    if (variationY > 0)
        [donneesFenetreEdition zoomInAvecFacteur:1/1.1];
    else
        [donneesFenetreEdition zoomOutAvecFacteur:1.1];
}

@end
