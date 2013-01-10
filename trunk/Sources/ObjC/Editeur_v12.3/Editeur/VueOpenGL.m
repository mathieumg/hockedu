//
//  VueOpenGL.m
//  Editeur
//
//  Created by L Tremblay on 12-02-20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "VueOpenGL.h"
#import "GLUT/glut.h"
#import "AppDelegate.h"
#import "DonneesEditeur.h"
#import "DonneesFenetreEdition.h"
#import "ControleurFenetreEdition.h"
#import "Outil.h"
#import "CadreAjustable.h"
#import "Tableau.h"

@implementation VueOpenGL
@synthesize donneesFenetreEdition;


- (BOOL) acceptsFirstMouse:(NSEvent *)theEvent
{
    return YES;
}


- (void) dessineRectangleElastique
{
    CGRect rectangle = [DonneesEditeur obtenirInstance].outil.rectangleElastique;    
    glEnable(GL_COLOR_LOGIC_OP);
    glEnable(GL_LINE_STIPPLE);
    glLineWidth(1);
    glLineStipple(1, 13107);      
    glLogicOp(GL_XOR);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(rectangle.origin.x, rectangle.origin.y, rectangle.origin.x + rectangle.size.width, rectangle.origin.y + rectangle.size.height);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_COLOR_LOGIC_OP);
    glDisable(GL_LINE_STIPPLE);
    glFinish();      
}

- (void)mettre_A_JourProjection
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(donneesFenetreEdition.fenetreVirtuelle.gauche, 
               donneesFenetreEdition.fenetreVirtuelle.droite,
               donneesFenetreEdition.fenetreVirtuelle.bas,
               donneesFenetreEdition.fenetreVirtuelle.haut);    
}

- (void) effectuerSelection
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLuint buffer[512];
    for (int i = 0; i < 512; i++)
        buffer[i] = 0;
    GLint hits;
    
    glSelectBuffer(512, buffer);
    (void) glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(0,  0, 1, 1, viewport);
    gluOrtho2D(donneesFenetreEdition.fenetreVirtuelle.gauche, 
               donneesFenetreEdition.fenetreVirtuelle.droite,
               donneesFenetreEdition.fenetreVirtuelle.bas,
               donneesFenetreEdition.fenetreVirtuelle.haut);   
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);				// erase the screen

	glMatrixMode(GL_MODELVIEW);	// use model matrix 
	glLoadIdentity();			// reset matrix 
    
    [donneesFenetreEdition.tableau dessiner];
    
    glFinish();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    hits = glRenderMode(GL_RENDER);
}


- (void)dessineVue 
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);				// erase the screen
    //[self mettre_A_JourProjection];
	glMatrixMode(GL_MODELVIEW);	// use model matrix 
	glLoadIdentity();			// reset matrix 

    [donneesFenetreEdition.tableau dessiner];

    glFinish();
    
}





- (void) keyDown:(NSEvent *)theEvent
{
}


- (void)drawRect:(NSRect)dirtyRect
{
    [self dessineVue];
}


- (void) mouseDown:(NSEvent *)theEvent
{
    [controleurFenetreEdition boutonGaucheEnfonceDansVueOpenGL:theEvent];
    
}

- (void)rightMouseDown:(NSEvent *)theEvent
{  
}


@end

