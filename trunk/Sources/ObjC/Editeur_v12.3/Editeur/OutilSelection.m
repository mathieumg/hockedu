//
//  OutilSelection.m
//  Editeur
//
//  Created by L Tremblay on 12-05-11.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "OutilSelection.h"
#import "DonneesFenetreEdition.h"
#import "CadreAjustable.h"
#import "GLUT/glut.h"
#import "Tableau.h"

@implementation OutilSelection

- (void)boutonGaucheEnfonceDansVueOpenGLAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    [super boutonGaucheEnfonceDansVueOpenGLAvecPosition:position etModifierFlags:modifierFlags etModele:donneesFenetreEdition];
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLuint buffer[TAILLE_BUFFER_SELECTION];
    for (int i = 0; i < TAILLE_BUFFER_SELECTION; i++)
        buffer[i] = 0;

    
    glSelectBuffer(TAILLE_BUFFER_SELECTION, buffer);
    (void) glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(position.x,  position.y, 1, 1, viewport);
    gluOrtho2D(donneesFenetreEdition.fenetreVirtuelle.gauche, 
               donneesFenetreEdition.fenetreVirtuelle.droite,
               donneesFenetreEdition.fenetreVirtuelle.bas,
               donneesFenetreEdition.fenetreVirtuelle.haut);   
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    [donneesFenetreEdition.tableau dessiner];
    
    glFinish();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    GLint nbObjets = glRenderMode(GL_RENDER);
    
    // Examiner le buffer de selection
    for (int i = 0; i < nbObjets; i++)
    {
        Noeud *unNoeud = ((Noeud *)([donneesFenetreEdition.dictionnaireObjets 
                                     objectForKey:[NSNumber numberWithUnsignedInteger:buffer[i * 4 + 3]]]));
        unNoeud.estSelectionne = !unNoeud.estSelectionne;
    }
    
    [donneesFenetreEdition.vueOpenGL setNeedsDisplay:YES]; 
    
}

- (void)boutonGaucheDeplaceAvecPosition:(CGPoint)position etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition
{
    if (boutonGaucheEstEnfonceEtDepartDansVueOpenGL)
    {
        CGPoint coordonneesDansFenetreVirtuelle = [donneesFenetreEdition convertirPositionVueOpenGLversPositionFenetreVirtuelle:position];
        CGSize deplacement = CGSizeMake(coordonneesDansFenetreVirtuelle.x - boutonGaucheEnfoncePosition.x, coordonneesDansFenetreVirtuelle.y - boutonGaucheEnfoncePosition.y);
        
        NSMutableSet *objetsSelectionnes = [donneesFenetreEdition obtenirObjetsSelectionnes];        
        NSEnumerator *enumerator = [objetsSelectionnes objectEnumerator];
        
        Noeud *objet;
        
        // Si la touche alt est tenue enfoncée, effectuer une rotation de la sélection.
        if (modifierFlags & NSAlternateKeyMask)
        {
            GLuint nbObjets = 0;            
            GLfloat xMin = 0.0, xMax = 0.0, yMin = 0.0, yMax = 0.0;
            // Trouver les extremes.
            if ((objet = [enumerator nextObject]))
            {
                nbObjets++;
                xMin = objet.position.x;
                xMax = xMin;
                yMin = objet.position.y;
                yMax = yMin;
            }
            while ((objet = [enumerator nextObject])) 
            {
                nbObjets++;
                
                if (objet.position.x < xMin)
                    xMin = objet.position.x;
                else if (objet.position.x > xMax)
                    xMax = objet.position.x;
                
                if (objet.position.y < yMin)
                    yMin = objet.position.y;
                else if (objet.position.y > yMax)
                    yMax = objet.position.y;
            }        
            GLfloat xCentre = xMin + ((xMax - xMin) / 2);
            GLfloat yCentre = yMin + ((yMax - yMin) / 2);

            GLfloat qteRotation = 0.1f;
            if (deplacement.height > 0)
                qteRotation = -0.1f;            

            enumerator = [objetsSelectionnes objectEnumerator];
            
            if (nbObjets == 1)
            {
                objet = [enumerator nextObject];
                objet.angle += qteRotation * (180 / M_PI);
            }
            else
            {
                while ((objet = [enumerator nextObject])) 
                {
             
                    GLfloat xRecentre = objet.position.x - xCentre;
                    GLfloat yRecentre = objet.position.y - yCentre;

                    GLfloat rayon = sqrt((pow(xRecentre, 2) + pow(yRecentre, 2)));
                    GLfloat angle = atanf(ABS(yRecentre / xRecentre));
                    
                    // Calcule les nouvelles positions
                    GLfloat xNouveau = 1.0;
                    GLfloat yNouveau = 1.0;
                    
                    
                    if (xRecentre > 0 && yRecentre > 0)
                    {
                        angle += qteRotation;
                    }
                    else if (xRecentre < 0 && yRecentre > 0)
                    {
                        angle -= qteRotation;
                        xNouveau = -1.0f;
                    }
                    else if (xRecentre < 0 && yRecentre < 0)
                    {
                        angle += qteRotation;                    
                        xNouveau = -1.0f;
                        yNouveau = -1.0f;
                    }
                    else if (xRecentre > 0 && yRecentre < 0)
                    {
                        angle -= qteRotation;
                        yNouveau = -1.0f;
                    }
                    
                    // Calcule les nouvelles positions
                    xNouveau *= (rayon * cosf(angle));
                    yNouveau *= (rayon * sinf(angle));                              
                    
                    xNouveau += xCentre;
                    yNouveau += yCentre;
                                            
                    objet.position = CGPointMake(xNouveau, yNouveau);
                    
                    objet.angle += qteRotation * (180 / M_PI);
                    
                }             
                
            }
            
            
        }
        else 
        {
            while ((objet = [enumerator nextObject]))
                objet.position = CGPointMake(objet.position.x + deplacement.width, objet.position.y + deplacement.height);                    
        }        
        
        [donneesFenetreEdition.vueOpenGL setNeedsDisplay:YES];

        [super boutonGaucheDeplaceAvecPosition:position etModifierFlags:modifierFlags etModele:donneesFenetreEdition];        

    }
}


- (void)rouletteSourisPivoteeAvecVariationEnY:(CGFloat)variationY etModifierFlags:(NSUInteger)modifierFlags etModele:(DonneesFenetreEdition *)donneesFenetreEdition;
{
    NSEnumerator *enumerator = [donneesFenetreEdition.dictionnaireObjets objectEnumerator];
    Noeud *objet;
    while ((objet = [enumerator nextObject])) {
        if (objet.estSelectionne) {
            if (variationY > 0) {
                if (objet.echelle <= FACTEUR_ECHELLE_MAX)
                    objet.echelle = objet.echelle += MODIF_FACTEUR_ECHELLE;
            }
            else {
                if (objet.echelle >= FACTEUR_ECHELLE_MIN)
                    objet.echelle = objet.echelle -= MODIF_FACTEUR_ECHELLE;
            }

        }
    }        
    [donneesFenetreEdition.vueOpenGL setNeedsDisplay:YES];    
    
    
}

@end
