//
//  Noeud.h
//  Editeur
//
//  Created by L Tremblay on 12-02-29.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Noeud : NSObject <NSCopying>
{
    GLuint identifiant;
    GLint nbPointsModele;
    GLfloat *modele;
    GLfloat echelle;
}

@property CGPoint position;
@property (readonly) GLint nbPointsModele;
@property (readonly) GLfloat *modele;
@property BOOL estSelectionne;
@property GLuint identifiant;
@property GLfloat echelle;
@property GLfloat angle;

- (void) ajouterNoeud:(Noeud *)unNoeud;
- (void) retirerNoeud:(Noeud *)unNoeud;
- (void) afficheInformations;
- (void) dessiner;
- (void) exporterEnXML:(NSXMLElement *)parent;

@end
