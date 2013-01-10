//
//  NoeudSimple.m
//  Editeur
//
//  Created by L Tremblay on 12-02-29.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "NoeudSimple.h"
#import "GLUT/glut.h"

@implementation NoeudSimple

- (id)copyWithZone:(NSZone *)zone
{
    NoeudSimple *uneCopie = [super copyWithZone:zone];
    return uneCopie;
}

- (void) dessiner
{
    // Utilisation de l'identifiant unique pour name. Permettra d'identifier
    // les objets touchés par un clic dans la vue.
    glLoadName(identifiant);
    glPushMatrix();
    
    // Si l'objet est sélectionné, on dessine juste le contour.
    if (self.estSelectionne)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // On applique les transformations.
    glTranslatef(self.position.x, self.position.y, 0.0f);    
    glRotated(self.angle, 0, 0, 1);
    glScalef(echelle, echelle, echelle);    
    
    // On envoie les vertex du modèle.
	glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f,0.0f,0.0f);
    int valeur = self.nbPointsModele;
    for (int i = 0; i < valeur; i++)
        glVertex3i(self.modele[3*i], self.modele[3*i+1], self.modele[3*i+2]);    
	glEnd();    
    
    glPopMatrix();
}

@end
