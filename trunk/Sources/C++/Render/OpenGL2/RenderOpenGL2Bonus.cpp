#include <RenderOpenGL2Bonus.h>
#include "GestionnaireModeles.h"
#include <NoeudAbstrait.h>
#include "FacadeModele.h"
#include "NodeBonus.h"

void RenderOpenGL2Bonus::render() const
{
    if( mNode->isVisible() )
    {
        NodeBonus* bonus = ( NodeBonus* )this->mNode;
        if( bonus->containsModifiers() )
        {
            return;
        }

        glDisable( GL_LIGHTING );
        FacadeModele::getInstance()->DeActivateShaders();
        // static to make all bonus hover at the same height
        const float zTranslated = bonus->getRadius()*1.5f + sin( bonus->getHeightAngle() )*3.f;
        glPushMatrix();
        glTranslatef( 0, 0, zTranslated );

        GLboolean blendEnabled = glIsEnabled( GL_BLEND );
        glEnable( GL_BLEND );
        renderNormalNode();
        if( !blendEnabled )
        {
            glDisable( GL_BLEND );
        }
        glPopMatrix();

        glEnable( GL_LIGHTING );
        FacadeModele::getInstance()->ActivateShaders();

        GLint renderMode;
        glGetIntegerv( GL_RENDER_MODE, &renderMode );
        if( renderMode == GL_SELECT )
        {
            // dont draw emptyBonus model when selecting
            return;
        }

        GLuint liste;
        GestionnaireModeles::obtenirInstance()->obtenirListe( RAZER_KEY_EMPTY_BONUS, liste );
        // Si aucune liste n'est trouvé, on sort de la fonction.
        if( liste == NULL )
            return;

        glPushMatrix();
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        Vecteur3 pos = bonus->getPosition();
        // Descend la platforme pour ne voir que la surface
        glTranslatef( pos[0], pos[1], pos[2] );
        glCallList( liste );

        glPopAttrib();
        glPopMatrix();
    }
}


void RenderOpenGL2Bonus::updateComponent()
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

