//
//  DonneesEditeur.h
//  Editeur
//
//  Created by L Tremblay on 12-02-22.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

// Singleton pour gérer les données communes lorsqu'il y a plusieurs fenêtres
// d'édition d'ouvertes.

#import <Foundation/Foundation.h>

@class Outil;
@class VueBoiteOutils;
@class DonneesFenetreEdition;

@interface DonneesEditeur : NSObject
{
}

@property(strong, nonatomic) Outil *outil;
@property(strong, nonatomic) VueBoiteOutils *vueBoiteOutils;

+ (DonneesEditeur *) obtenirInstance;
- (void)selectionnerOutil:(NSString *)nomOutil avecSousType:(NSString *)sousType;
- (void)boutonGaucheEnfonce:(CGPoint)coordonneesVueOpenGL avecModele:(DonneesFenetreEdition *)donneesFenetreEdition;

@end
