#include <RenderOpenGL2.h>
#include "glew.h"
#include "GestionnaireModeles.h"
#include <NoeudAbstrait.h>
#include "AideGL.h"
#include <map>

GLuint RenderOpenGL2::mIdGlCounter = 1;

RenderOpenGL2::RenderOpenGL2() :
mModePolygones(GL_FILL)
, mGlId( mIdGlCounter++ )
{
    // On charge la matrice identitee lors de la construction
    // On garde en memoire la matrice d'origine (devrait etre NULL)
    glPushMatrix();
    glLoadIdentity();
    glGetFloatv( GL_MODELVIEW_MATRIX, mTransformationMatrix );
    // On remet en memoire la matrice d'origine
    glPopMatrix();
}

void RenderOpenGL2::render() const
{
    if( mNode->isVisible() )
    {
        renderNormalNode();
    }
}

void RenderOpenGL2::updateComponent()
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    const float angle = mNode->getAngle();
    Vecteur3 scale;
    mNode->getScale(scale);
    glRotatef( angle, 0.0, 0.0, 1.0 );
    glScalef( scale[VX], scale[VY], scale[VZ] );

    glGetFloatv( GL_MODELVIEW_MATRIX, mTransformationMatrix ); // Savegarde de la matrice courante dans le noeud

    glPopMatrix();
}

void RenderOpenGL2::renderNormalNode( ) const
{
    // Assignation du mode d'affichage des polygones
    //glPolygonMode( GL_FRONT_AND_BACK, mModePolygones );
    GLuint liste;
    GestionnaireModeles::obtenirInstance( )->obtenirListe( this->mNode->getSkinKey( ), liste );
    // Si aucune liste n'est trouvé, on sort de la fonction.
    if( liste == NULL )
        return;

    glPushMatrix();
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    const Vecteur3 position = this->mNode->getPosition( );
    glTranslatef( position[0], position[1], position[2] );

    // Effectue les mise a l'echelle et les rotations
    glMultMatrixf( this->mTransformationMatrix );
    const bool invColor = this->mNode->isHighlighted( ) || this->mNode->IsSelected( );
    if( invColor )
    {
        glEnable( GL_COLOR_LOGIC_OP );
        glLogicOp( GL_COPY_INVERTED );
    }

    GLint renderMode;
    glGetIntegerv( GL_RENDER_MODE, &renderMode );
    if( renderMode == GL_SELECT && this->mNode->canBeSelected( ) )
    {
        // Push du id du noeud sur la pile de nom
        glPushName( this->mGlId );
        // Push du id du type du noeud sur la pile de nom
        glPushName( (GLuint)this->mNode->getKey( ) );
        glCallList( liste ); // Dessin de l'objet avec les textures
        glPopName();
        glPopName();
    }
    else
    {
        glCallList( liste ); // Dessin de l'objet avec les textures
    }

    if( invColor )
    {
        glDisable( GL_COLOR_LOGIC_OP );
    }
    // Restauration
    glPopAttrib();
    glPopMatrix();
}


