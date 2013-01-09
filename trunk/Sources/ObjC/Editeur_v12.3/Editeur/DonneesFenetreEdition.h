//
//  DonneesFenetreEdition.h
//  Editeur
//
//  Created by L Tremblay on 12-04-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#define DEPLACEMENT_POURCENTAGE_DEFAUT 0.05f

#import <Foundation/Foundation.h>
@class Cadre;
@class CadreAjustable;
@class Tableau;
@class Noeud;

@interface DonneesFenetreEdition : NSObject <NSXMLParserDelegate>
{
    GLfloat ratioVueOpenGL;
    CadreAjustable *fenetreVirtuelle;
    Tableau *tableau;
    NSMutableDictionary *dictionnaireObjets;
}

@property (strong, nonatomic) CadreAjustable *fenetreVirtuelle;
@property (strong, nonatomic) IBOutlet VueOpenGL *vueOpenGL;
@property GLfloat ratioVueOpenGL;
@property (strong, readonly) Tableau *tableau;
@property (readonly) NSMutableDictionary* dictionnaireObjets;

- (void)boutonGaucheEnfonceDansVueOpenGLAvecPosition:(CGPoint)coordonneesDansVueOpenGL etModifierFlags:(NSUInteger)modifierFlags;
- (void)boutonGaucheDeplaceAvecPosition:(CGPoint)coordonneesDansVueOpenGL etModifierFlags:(NSUInteger)modifierFlags;
- (void)boutonGaucheRelacheAvecPosition:(CGPoint)coordonneesDansVueOpenGL etModifierFlags:(NSUInteger)modifierFlags;
- (void)rouletteSourisPivoteeAvecVariationEnY:(CGFloat)variationY etModifierFlags:(NSUInteger)modifierFlags;

- (void)zoomAvecFacteur:(GLfloat)facteur;
- (void)zoomInAvecFacteur:(GLfloat)facteur;
- (void)zoomOutAvecFacteur:(GLfloat)facteur;
- (void)zoomInAvecCadreVirtuel:(Cadre *)cadreVirtuel;
- (void)zoomOutAvecCadreVirtuel:(Cadre *)cadreVirtuel;

- (void)deplacerParSourisVueEnX:(GLfloat)x etY:(GLfloat)y;
- (void)deplacerParClavierVueEnX:(GLshort)x;
- (void)deplacerParClavierVueEnY:(GLshort)y;

- (void)changerTailleFenetreVirtuelleAvecEcart:(CGSize)ecart;

- (CGPoint)convertirPositionVueOpenGLversPositionFenetreVirtuelle:(CGPoint)positionVueOpenGL;

- (Noeud *)ajouterObjetSimple:(NSString *)nomClasseObjet avecPosition:(CGPoint)unePosition EtEchelle:(GLfloat)uneEchelle EtAngle:(GLfloat)unAngle;

- (void)effacerObjet:(Noeud *) unNoeud;

- (void) dupliquerSelection;

- (void) effacerSelection;

- (NSMutableSet *) obtenirObjetsSelectionnes;

- (void) exporterEnXML;

- (void) importerEnXML;

@end
