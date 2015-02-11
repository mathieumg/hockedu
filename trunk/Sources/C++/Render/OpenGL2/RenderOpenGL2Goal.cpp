#include <RenderOpenGL2Goal.h>
#include "GestionnaireModeles.h"
#include <NoeudAbstrait.h>
#include "FacadeModele.h"
#include "NoeudBut.h"
#include "Utilitaire.h"

void RenderOpenGL2Goal::render() const
{
    NoeudBut* goal = (NoeudBut*)this->mNode;
#if WIN32
    GLuint liste = NULL;
    GestionnaireModeles::obtenirInstance()->obtenirListe( goal->getSkinKey(), liste );

    if( liste == 0 )
    {
        return;
    }

    GLint renderMode;
    glGetIntegerv( GL_RENDER_MODE, &renderMode );
    if( renderMode == GL_SELECT )
    {
        // dont draw goals model when selecting
        return;
    }
#endif
    if( goal->isVisible() )
    {
        const Vecteur3& positionPoint = goal->getPosition();
        glTranslatef( positionPoint[VX], positionPoint[VY], 0 );

        Vecteur3 posBas = goal->getBottomPosition() - positionPoint;
        Vecteur3 posHaut = goal->getTopPosition() - positionPoint;

#if WIN32
        float translateX = 0;
        if( goal->obtenirJoueur() == 1 )
        {
            translateX = utilitaire::borneSuperieure( posBas[VX], posHaut[VX] );
        }
        else
        {
            translateX = utilitaire::borneInferieure( posBas[VX], posHaut[VX] );
        }
        glPushMatrix();
        glTranslatef( translateX, 0, 0 );
        // Renders all the modifiers present on the node
        for( auto it = goal->mModifiers.begin(); it != mModifiers.end(); ++it )
        {
            ( *it )->render();
        }

        glPopMatrix();
#endif

        // Dessin de la partie ajustable en bas
        glPushMatrix();
        glTranslatef( posBas[VX], posBas[VY], 0 );
        glRotatef( goal->mBottomAngle, 0, 0, 1 );
        glMultMatrixf( mTransformationMatrix );
        if( goal->obtenirJoueur() == 1 )
        {
            glScalef( 1, -1, 1 );
        }
        glPushAttrib( GL_ALL_ATTRIB_BITS );
#if WIN32
        glCallList( liste );
#else
        auto field = getField();
        if(!field || !field->renderAppleNode(getSkinKey()))
        {
            glDisable(GL_BLEND);
            renderOpenGLES();
            glEnable(GL_BLEND);
        }
#endif

        glPopAttrib();
        glPopMatrix();

        // Dessin de la partie ajustable en haut
        glPushMatrix();
        glTranslatef( posHaut[VX], posHaut[VY], 0 );
        glRotatef( goal->mTopAngle, 0, 0, 1 );
        glMultMatrixf( mTransformationMatrix );
        if( goal->obtenirJoueur() == 2 )
        {
            glScalef( 1., -1., 1. );
        }
        glPushAttrib( GL_ALL_ATTRIB_BITS );
#if WIN32
        glCallList( liste );
#else
        if( !field || !field->renderAppleNode( getSkinKey() ) )
        {
            glDisable(GL_BLEND);
            renderOpenGLES();
            glEnable(GL_BLEND);
        }
#endif

        glPopAttrib();
        glPopMatrix();
    }
}


void RenderOpenGL2Goal::updateComponent()
{
    const float angle = mNode->getAngle();
    const float radius = mNode->getDefaultRadius()*1.5f;
    Vecteur3 scale;
    mNode->getScale(scale);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glRotatef( angle, 0.0, 0.0, 1.0 );
    const float angleX = 45;
    const float angleY = 35;
    glRotatef( angleX, 1, 0, 0 );
    glRotatef( angleY, 0, 1, 0 );
    glScalef( scale[VX], scale[VY], scale[VZ] );
    glScalef( radius, radius, radius );

    glGetFloatv( GL_MODELVIEW_MATRIX, mTransformationMatrix ); // Savegarde de la matrice courante dans le noeud

    glPopMatrix();
}

