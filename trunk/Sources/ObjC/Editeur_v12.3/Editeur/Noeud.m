//
//  Noeud.m
//  Editeur
//
//  Created by L Tremblay on 12-02-29.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Noeud.h"
#import "GLUT/glut.h"

// Identifiant pour fin de debug
static GLuint identifiantUnique = 1;

@implementation Noeud
@synthesize position;
@synthesize nbPointsModele;
@synthesize modele;
@synthesize estSelectionne;
@synthesize identifiant;
@synthesize echelle;
@synthesize angle;

- (Noeud *) init
{
    self = [super init];
    if (self)
    {
        echelle = 1.0;
        estSelectionne = NO;
        identifiant = identifiantUnique++;
        angle = 0.0;
    }
    return self;
}

- (id)copyWithZone:(NSZone *)zone
{
    Noeud *uneCopie = [[[self class] allocWithZone:zone] init];
    return uneCopie;
}

- (void) exporterEnXML:(NSXMLElement *)parent
{
    
}


- (void) ajouterNoeud:(Noeud*)unNoeud
{
    NSLog(@"Ajouter un Noeud");
}

- (void) retirerNoeud:(Noeud*)unNoeud;
{
    NSLog(@"Retirer un Noeud");    
}

- (void) afficheInformations
{
    NSLog(@"%@ : %u", NSStringFromClass([self class]), identifiant) ;
}

- (void) dessiner
{
}


@end
