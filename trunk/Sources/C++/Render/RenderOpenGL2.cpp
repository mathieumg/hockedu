#include <RenderOpenGL2.h>
#include "GestionnaireModeles.h"
#include <NoeudAbstrait.h>
#include "AideGL.h"
#include <map>

GLuint RenderOpenGL2::mIdGlCounter = 1;


bool renderFunctionInitialized = false;
std::map<RazerKey, OpenGL2RenderFunction> renderFunctions;
void PopulateRenderFunctionMap();

RenderOpenGL2::RenderOpenGL2() :
  mScale( 1, 1, 1 )
, mModePolygones(GL_FILL)
, mGlId( mIdGlCounter++ )
{
    if( !renderFunctionInitialized )
    {
        renderFunctionInitialized = true;
        PopulateRenderFunctionMap();
    }

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
        renderNormalNode( this );
    }
}

void RenderOpenGL2::updateComponent()
{

}


void RenderOpenGL2::renderNormalNode( const RenderOpenGL2* pThis )
{
    // Assignation du mode d'affichage des polygones
    //glPolygonMode( GL_FRONT_AND_BACK, mModePolygones );
    GLuint liste;
    GestionnaireModeles::obtenirInstance( )->obtenirListe( pThis->mNode->getSkinKey( ), liste );
    // Si aucune liste n'est trouvé, on sort de la fonction.
    if( liste == NULL )
        return;

    glPushMatrix();
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    const Vecteur3 position = pThis->mNode->getPosition( );
    glTranslatef( position[0], position[1], position[2] );

    // Effectue les mise a l'echelle et les rotations
    glMultMatrixf( pThis->mTransformationMatrix );
    const bool invColor = pThis->mNode->isHighlighted( ) || pThis->mNode->IsSelected( );
    if( invColor )
    {
        glEnable( GL_COLOR_LOGIC_OP );
        glLogicOp( GL_COPY_INVERTED );
    }

    GLint renderMode;
    glGetIntegerv( GL_RENDER_MODE, &renderMode );
    if( renderMode == GL_SELECT && pThis->mNode->canBeSelected( ) )
    {
        // Push du id du noeud sur la pile de nom
        glPushName( pThis->mGlId );
        // Push du id du type du noeud sur la pile de nom
        glPushName( (GLuint)pThis->mNode->getKey( ) );
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



void PopulateRenderFunctionMap()
{

}