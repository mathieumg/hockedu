//
//  DonneesEditeur.m
//  Editeur
//
//  Created by L Tremblay on 12-02-22.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

// Singleton pour gérer les données communes lorsqu'il y a plusieurs fenêtres
// d'édition d'ouvertes.

#import "DonneesEditeur.h"
#import "Outil.h"
#import "OutilZoom.h"
#import "VueBoiteOutils.h"
#import "OutilAjoutObjetSimple.h"

@implementation DonneesEditeur
@synthesize outil;
@synthesize vueBoiteOutils;

+ (DonneesEditeur *) obtenirInstance
{
    static DonneesEditeur *instancePartagee = nil;
    static dispatch_once_t jeton;
    dispatch_once(&jeton, ^{instancePartagee = [[DonneesEditeur alloc] init];});
    return instancePartagee;
}

- (DonneesEditeur*) init
{
    self = [super init];
    if (self != nil) {
        // Outil par defaut
        outil = [[Outil alloc] init];
    }
    return self;
}

- (void)selectionnerOutil:(NSString *)nomOutil avecSousType:(NSString *)sousType
{
    outil = [[NSClassFromString(nomOutil) alloc] init];
    
    if ([outil respondsToSelector:@selector(definirSousType:)])
    {
        [(id)outil definirSousType:sousType];
    }
    [vueBoiteOutils setNeedsDisplay:YES];
}

- (void)boutonGaucheEnfonce:(CGPoint)coordonneesVueOpenGL avecModele:(DonneesFenetreEdition *)donneesFenetreEdition
{
}

@end
