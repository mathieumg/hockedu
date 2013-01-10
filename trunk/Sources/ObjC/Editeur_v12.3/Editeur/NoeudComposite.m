//
//  NoeudComposite.m
//  Editeur
//
//  Created by L Tremblay on 12-02-29.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "NoeudComposite.h"
#import "GLUT/glut.h"
@implementation NoeudComposite

@synthesize enfants;

- (NoeudComposite *) init
{
    return [self initWithCapacity:1];    
}

- (NoeudComposite *) initWithCapacity:(NSUInteger) capacite
{
    self = [super init];
    if (self) 
    {
        enfants = [[NSMutableArray alloc] initWithCapacity:capacite];
    }
    return self;
}

- (id)copyWithZone:(NSZone *)zone
{
    NoeudComposite *uneCopie = [super copyWithZone:zone];
    // Pour chaque noeud enfant, on fait une copie
    for (Noeud* enfant in enfants)
    {
        [uneCopie.enfants addObject:[enfant copy]];
    }    
    return uneCopie;
}

- (void) ajouterNoeud:(Noeud *)unNoeud
{
    NSLog(@"Le noeud composite %u a ajoute l'enfant %u", identifiant, unNoeud->identifiant);
    [enfants addObject:unNoeud];
}

- (void) retirerNoeud:(Noeud *)unNoeud
{
    [enfants removeObject:unNoeud];
}

- (void) afficheInformations
{
    [super afficheInformations];
    // On demande aux enfants du noeud composite de faire la même chose.
    [enfants makeObjectsPerformSelector:@selector(afficheInformations)];
}

- (void) exporterEnXML:(NSXMLElement *)parent
{
    [enfants makeObjectsPerformSelector:@selector(exporterEnXML:) withObject:(id)parent];
}

- (void) dessiner
{
    // On demande aux enfants du noeud composite de faire la même chose.
    [enfants makeObjectsPerformSelector:@selector(dessiner)];    
}


@end
