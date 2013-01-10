//
//  Outil.h
//  Editeur
//
//  Created by L Tremblay on 12-04-26.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

// A considerer comme un classe abstraite.
@interface Outil : NSObject
{
    BOOL boutonGaucheEstEnfonceEtDepartDansVueOpenGL;    
    CGPoint boutonGaucheEnfoncePosition;
    CGPoint boutonGaucheEnfonceDernierePosition;
    CGRect rectangleElastique;
}

@property CGRect rectangleElastique;

- (void)boutonGaucheEnfonceDansVueOpenGLAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
- (void)boutonGaucheDeplaceAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
- (void)boutonGaucheRelacheAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
- (void)rouletteSourisPivoteeAvecVariationEnY:(CGFloat)variationY etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;

@end
