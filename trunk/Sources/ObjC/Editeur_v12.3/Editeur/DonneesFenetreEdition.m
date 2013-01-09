//
//  DonneesFenetreEdition.m
//  Editeur
//
//  Created by L Tremblay on 12-04-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "DonneesFenetreEdition.h"
#import "DonneesEditeur.h"
#import "Outil.h"
#import "Cadre.h"
#import "CadreAjustable.h"
#import "Noeud.h"
#import "Tableau.h"
#import "Configuration.h"


@implementation DonneesFenetreEdition
@synthesize vueOpenGL;
@synthesize ratioVueOpenGL;
@synthesize fenetreVirtuelle;
@synthesize tableau;
@synthesize dictionnaireObjets;

- (DonneesFenetreEdition*)init
{
    self = [super init];
    if (self != nil) {
        tableau = [[Tableau alloc] init];
        dictionnaireObjets = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (void)exporterEnXML
{
    NSXMLElement *elementRacine = [[NSXMLElement alloc] initWithName:@"Parcours"];
    [tableau exporterEnXML:elementRacine];

    NSXMLDocument *xmlRequest = [NSXMLDocument documentWithRootElement:elementRacine];
    NSData *xmlData = [xmlRequest XMLDataWithOptions:NSXMLNodePrettyPrint];
    
    NSSavePanel *savePanel = [NSSavePanel savePanel];
    [savePanel setAllowedFileTypes:[NSArray arrayWithObject:@"xml"]];
    if ([savePanel runModal] == NSOKButton)
    {
        NSURL *url = [savePanel URL];
        NSString *cheminNomFichier = [url path];       
       [xmlData writeToFile:cheminNomFichier atomically:YES];
    }
}

- (void) importerEnXML
{
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    if ([openPanel runModal] == NSOKButton)
    {
        [dictionnaireObjets removeAllObjects];
        tableau = nil;        
        
        NSURL *url = [openPanel URL];
        NSString *cheminNomFichier = [url path];     
        NSData *data = [NSData dataWithContentsOfFile:cheminNomFichier];
        NSXMLParser *parser = [[NSXMLParser alloc] initWithData:data];        
        [parser setDelegate:self];
        tableau = [[Tableau alloc] init];        
        [parser parse];
  
    }    
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName attributes:(NSDictionary *)attributeDict{
    
    if ([elementName isEqualToString:@"Triangle"] || [elementName isEqualToString:@"Carre"]) {

        CGPoint position = CGPointMake([[attributeDict valueForKey:@"Position_x"] floatValue], 
                                       [[attributeDict valueForKey:@"Position_y"] floatValue]);
        
        [self ajouterObjetSimple:(NSString *)elementName avecPosition:(CGPoint)position EtEchelle:[[attributeDict valueForKey:@"Echelle"] floatValue] EtAngle:[[attributeDict valueForKey:@"Angle"] floatValue]];
    }
}



- (void)awakeFromNib
{
    ratioVueOpenGL = vueOpenGL.frame.size.width / vueOpenGL.frame.size.height;
    fenetreVirtuelle = [[CadreAjustable alloc] initWithCoteGauche:0
                                                            droit:vueOpenGL.frame.size.width
                                                              bas:0
                                                             haut:vueOpenGL.frame.size.height 
                                                   etCadreMinimal:[[Cadre alloc] initWithCoteGauche:-FV_DEMI_LARGEUR_MIN
                                                                                              droit:FV_DEMI_LARGEUR_MIN
                                                                                                bas:-FV_DEMI_HAUTEUR_MIN
                                                                                               haut:FV_DEMI_HAUTEUR_MIN]                                                    
                                                   unCadreMaximal:[[Cadre alloc] initWithCoteGauche:-(FV_DEMI_LARGEUR_MAX)
                                                                                              droit:(FV_DEMI_LARGEUR_MAX)
                                                                                                bas:-(FV_DEMI_HAUTEUR_MAX)
                                                                                                haut:(FV_DEMI_HAUTEUR_MAX)]];
    
}

- (Noeud *)ajouterObjetSimple:(NSString *)nomClasseObjet avecPosition:(CGPoint)unePosition EtEchelle:(GLfloat)uneEchelle EtAngle:(GLfloat)unAngle
{
    Noeud *unObjet = [[NSClassFromString(nomClasseObjet) alloc] init];
    unObjet.position = unePosition;
    unObjet.echelle = uneEchelle;
    unObjet.angle = unAngle;
    [tableau ajouterNoeud:unObjet];
    [dictionnaireObjets setObject:unObjet forKey:[NSNumber numberWithUnsignedInt:unObjet.identifiant]];
    [vueOpenGL setNeedsDisplay:YES];
    return unObjet;
}

- (void)effacerObjet:(Noeud *) unObjet
{
    [tableau retirerNoeud:unObjet];
    [dictionnaireObjets removeObjectForKey:[NSNumber numberWithUnsignedInt:unObjet.identifiant]]; 
    [vueOpenGL setNeedsDisplay:YES];      
}

- (NSMutableSet *) obtenirObjetsSelectionnes
{
    Noeud *objet;
    NSEnumerator *enumerator = [dictionnaireObjets objectEnumerator];    
    NSMutableSet *objetSelectionnes = [[NSMutableSet alloc] init];
    while ((objet = [enumerator nextObject])) {
        if (objet.estSelectionne) {
            [objetSelectionnes addObject:objet];
        }
    }    
    return objetSelectionnes;
}

- (void) effacerSelection
{
    NSEnumerator *enumerator = [dictionnaireObjets objectEnumerator];
    Noeud *objet;
    NSMutableSet *objet_A_Effacer = [[NSMutableSet alloc] init];
    // Fait en 2 boucles pour ne pas modifier le dictionnaire pendant qu'on enumere son contenu.
    while ((objet = [enumerator nextObject])) {
        if (objet.estSelectionne) {
            [objet_A_Effacer addObject:objet];
        }
    }
    for (Noeud *objet in objet_A_Effacer)
    {
        // Effacer les objets.
        [self effacerObjet:objet];
    }
    
    [vueOpenGL setNeedsDisplay:YES];     
}


- (void) dupliquerSelection
{
    NSEnumerator *enumerator = [dictionnaireObjets objectEnumerator];
    Noeud *objet;
    NSMutableSet *objet_A_Dupliquer = [[NSMutableSet alloc] init];
    // Fait en 2 boucles pour ne pas modifier le dictionnaire pendant qu'on enumere son contenu.
    while ((objet = [enumerator nextObject])) {
        if (objet.estSelectionne) {
            [objet_A_Dupliquer addObject:objet];
        }
    }
    for (Noeud *objet in objet_A_Dupliquer)
    {
        [self ajouterObjetSimple:NSStringFromClass([objet class]) avecPosition:objet.position EtEchelle:objet.echelle EtAngle:objet.angle];
    }

    [vueOpenGL setNeedsDisplay:YES];     
}


- (CGPoint)convertirPositionVueOpenGLversPositionFenetreVirtuelle:(CGPoint)positionVueOpenGL
{
    Cadre *nv = self.fenetreVirtuelle;
    float x = nv.gauche + (self.fenetreVirtuelle.largeur * (positionVueOpenGL.x / vueOpenGL.frame.size.width));
    float y = nv.bas + (self.fenetreVirtuelle.hauteur * (positionVueOpenGL.y / vueOpenGL.frame.size.height));
    CGPoint test = CGPointMake(x,y);
    return test;
}

// x et y sont des déplacements en pixel dans la fenêtre. Il faut les convertir.
- (void)deplacerParSourisVueEnX:(GLfloat)x etY:(GLfloat)y
{
    [fenetreVirtuelle deplacerCentreAvecDeltaX:x etDeltaY:y];
    [vueOpenGL mettre_A_JourProjection];
    [vueOpenGL setNeedsDisplay:YES];
}

- (void)deplacerParClavierVueEnX:(GLshort)x
{
    GLfloat deplacement = x * DEPLACEMENT_POURCENTAGE_DEFAUT * self.fenetreVirtuelle.largeur;
    [fenetreVirtuelle deplacerCentreAvecDeltaX:deplacement etDeltaY:0.0];
    [vueOpenGL mettre_A_JourProjection];    
    [vueOpenGL setNeedsDisplay:YES];    
}

- (void)deplacerParClavierVueEnY:(GLshort)y
{
    GLfloat deplacement = y * DEPLACEMENT_POURCENTAGE_DEFAUT * self.fenetreVirtuelle.hauteur;
    [fenetreVirtuelle deplacerCentreAvecDeltaX:0.0 etDeltaY:deplacement];
    [vueOpenGL mettre_A_JourProjection];    
    [vueOpenGL setNeedsDisplay:YES];    
}

- (void)zoomInAvecCadreVirtuel:(Cadre *)cadreVirtuel
{
    // Deplacer le centre
    [fenetreVirtuelle deplacerCentreAvecDeltaX:fenetreVirtuelle.centre.x - cadreVirtuel.centre.x 
                                      etDeltaY:fenetreVirtuelle.centre.y - cadreVirtuel.centre.y];    
    // Mettre a echelle
    GLfloat facteur;
    if (cadreVirtuel.ratio < fenetreVirtuelle.ratio)
    {
        facteur = cadreVirtuel.hauteur / fenetreVirtuelle.hauteur;
    }
    else
    {
        facteur = cadreVirtuel.largeur / fenetreVirtuelle.largeur;        
    }
    if (facteur < FACTEUR_ZOOM_MIN)
        facteur = FACTEUR_ZOOM_MIN;
    [fenetreVirtuelle mettre_A_Echelle:facteur];
    [vueOpenGL mettre_A_JourProjection];    
    [vueOpenGL setNeedsDisplay:YES]; 
    return;
}

- (void)zoomOutAvecCadreVirtuel:(Cadre *)cadreVirtuel
{
    if ((int)cadreVirtuel.largeur == 0 && (int)cadreVirtuel.hauteur == 0)
        return;
    
    // Mettre a echelle
    GLfloat facteur;
    if (cadreVirtuel.ratio < fenetreVirtuelle.ratio)
    {
        facteur = fenetreVirtuelle.hauteur / cadreVirtuel.hauteur;
    }
    else
    {
        facteur = fenetreVirtuelle.largeur / cadreVirtuel.largeur;        
    }
    if (facteur > FACTEUR_ZOOM_MAX)
        facteur = FACTEUR_ZOOM_MAX;
    [fenetreVirtuelle mettre_A_Echelle:facteur];
    // Replacer le centre
    [fenetreVirtuelle deplacerCentreAvecDeltaX:facteur*(cadreVirtuel.centre.x - fenetreVirtuelle.centre.x) 
                                      etDeltaY:facteur*(cadreVirtuel.centre.y - fenetreVirtuelle.centre.y)];
    [vueOpenGL mettre_A_JourProjection];    
    [vueOpenGL setNeedsDisplay:YES]; 
    return;    
}

- (void)zoomAvecFacteur:(GLfloat)facteur
{
    [fenetreVirtuelle mettre_A_Echelle:facteur];
    [vueOpenGL mettre_A_JourProjection];    
    [vueOpenGL setNeedsDisplay:YES];
}


- (void)zoomInAvecFacteur:(GLfloat)facteur
{
    [fenetreVirtuelle mettre_A_Echelle:facteur];
    [vueOpenGL mettre_A_JourProjection];    
    [vueOpenGL setNeedsDisplay:YES];
}

- (void)zoomOutAvecFacteur:(GLfloat)facteur
{   
    [fenetreVirtuelle mettre_A_Echelle:facteur];
    [vueOpenGL mettre_A_JourProjection];    
    [vueOpenGL setNeedsDisplay:YES];
     
}

- (void)changerTailleFenetreVirtuelleAvecEcart:(CGSize)ecart
{
    GLfloat gauche = (self.fenetreVirtuelle.centre.x - (self.fenetreVirtuelle.largeur + (self.fenetreVirtuelle.largeur / vueOpenGL.frame.size.width * ecart.width)) / 2);
    GLfloat droite = (self.fenetreVirtuelle.centre.x + (self.fenetreVirtuelle.largeur + (self.fenetreVirtuelle.largeur / vueOpenGL.frame.size.width * ecart.width)) / 2);
    GLfloat bas = (self.fenetreVirtuelle.centre.y - (self.fenetreVirtuelle.hauteur + (self.fenetreVirtuelle.hauteur / vueOpenGL.frame.size.height * ecart.height)) / 2);
    GLfloat haut = (self.fenetreVirtuelle.centre.y + (self.fenetreVirtuelle.hauteur + (self.fenetreVirtuelle.hauteur / vueOpenGL.frame.size.height * ecart.height)) / 2);    
    [fenetreVirtuelle definirCoteGauche:gauche
                                  droit:droite 
                                    bas:bas 
                                   haut:haut];
}

- (void)boutonGaucheEnfonceDansVueOpenGLAvecPosition:(CGPoint)coordonneesDansVueOpenGL etModifierFlags:(NSUInteger)modifierFlags
{
    [[DonneesEditeur obtenirInstance].outil boutonGaucheEnfonceDansVueOpenGLAvecPosition:coordonneesDansVueOpenGL 
                                                                                etModifierFlags:modifierFlags
                                                                                etModele:self];
}

- (void)boutonGaucheDeplaceAvecPosition:(CGPoint)coordonneesDansVueOpenGL etModifierFlags:(NSUInteger)modifierFlags
{
    [[DonneesEditeur obtenirInstance].outil boutonGaucheDeplaceAvecPosition:coordonneesDansVueOpenGL 
                                                            etModifierFlags:modifierFlags
                                                                   etModele:self];   
}

- (void)boutonGaucheRelacheAvecPosition:(CGPoint)coordonneesDansVueOpenGL etModifierFlags:(NSUInteger)modifierFlags
{
    [[DonneesEditeur obtenirInstance].outil boutonGaucheRelacheAvecPosition:coordonneesDansVueOpenGL 
                                                            etModifierFlags:modifierFlags
                                                                   etModele:self];
    
}

- (void)rouletteSourisPivoteeAvecVariationEnY:(CGFloat)variationY etModifierFlags:(NSUInteger) modifierFlags
{
    [[DonneesEditeur obtenirInstance].outil rouletteSourisPivoteeAvecVariationEnY:variationY 
                                                                  etModifierFlags:modifierFlags
                                                                         etModele:self];
}



@end
