#include <RenderOpenGL2.h>
#include "GestionnaireModeles.h"
#include <NoeudAbstrait.h>
#include "AideGL.h"
#include <map>
#include "FacadeModele.h"
#include "NodeBonus.h"

GLuint RenderOpenGL2::mIdGlCounter = 1;

bool renderFunctionInitialized = false;
std::map<RazerKey, OpenGL2RenderFunction> renderFunctions;

void RenderOpenGL2::PopulateRenderFunctionMap()
{
    renderFunctions[RAZER_KEY_BONUS] = RenderOpenGL2::renderBonus;
}


RenderOpenGL2::RenderOpenGL2() :
mModePolygones(GL_FILL)
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
        auto it = renderFunctions.find(mNode->getKey());
        if( it != renderFunctions.cend() )
        {
            it->second( this );
        }
        else
        {
            renderNormalNode( this );
        }
    }
}

void RenderOpenGL2::updateComponent()
{
    updateMatrice();
}

void RenderOpenGL2::updateMatrice( )
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



void RenderOpenGL2::renderBonus( const RenderOpenGL2* pThis )
{
    NodeBonus* bonus = (NodeBonus*)pThis->mNode;
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
    renderNormalNode( pThis );
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
