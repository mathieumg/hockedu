#include "GestionnaireModeles.h"
#include <NoeudAbstrait.h>
#include "FacadeModele.h"
#include "NoeudTable.h"
#include <RenderOpenGL2Table.h>

RenderOpenGL2Table::RenderOpenGL2Table() : mListRenderCorners(nullptr)
{

}

RenderOpenGL2Table::~RenderOpenGL2Table()
{
    if( mListRenderCorners )
    {
        glDeleteLists( *mListRenderCorners, NoeudTable::TABLE_CORNER_COUNT );
    }
    delete[] mListRenderCorners;
}


void RenderOpenGL2Table::render() const
{
    if( mNode->isVisible() )
    {
#if WIN32 && 0
        GLint renderMode;
        glGetIntegerv( GL_RENDER_MODE, &renderMode );
        if( renderMode == GL_SELECT )
        {
            // dont draw table when selecting
            return;
        }

        Modele3D* pModel = mNode->getModel();
        if( pModel )
        {
            // render corners
            if( mListRenderCorners )
            {
                checkf( NoeudTable::TABLE_CORNER_COUNT == 4 );
                glPushMatrix();
                glPushAttrib( GL_ALL_ATTRIB_BITS );
                if( pModel->appliquerMateriau( "GreenTeam" ) )
                {
                    glPushMatrix();
                    glCallList( mListRenderCorners[1] );
                    glPopMatrix();

                    glPushMatrix();
                    glCallList( mListRenderCorners[2] );
                    glPopMatrix();

                    // car la fonction appliquer materiau fait un push matrix dans le mode Texture
                    glMatrixMode( GL_TEXTURE );
                    glPopMatrix();
                    glMatrixMode( GL_MODELVIEW );
                }
                if( pModel->appliquerMateriau( "RedTeam" ) )
                {
                    glPushMatrix();
                    glCallList( mListRenderCorners[0] );
                    glPopMatrix();
                    glPushMatrix();
                    glCallList( mListRenderCorners[3] );
                    glPopMatrix();

                    // car la fonction appliquer materiau fait un push matrix dans le mode Texture
                    glMatrixMode( GL_TEXTURE );
                    glPopMatrix();
                    glMatrixMode( GL_MODELVIEW );
                }
                glPopAttrib();
                glPopMatrix();
            }

            glPushMatrix();
            glPushAttrib( GL_ALL_ATTRIB_BITS );
            pModel->dessiner( true );
            glPopAttrib();
            glPopMatrix();
        }

        glColor4f( 1.0f, 0.0f, 1.0f, 1.0f );

        // États de la lumière 
        GLboolean lighting_state;
        // Désactiver l'éclairage
        glGetBooleanv( GL_LIGHTING, &lighting_state );
        glDisable( GL_LIGHTING );

        FacadeModele::getInstance()->DeActivateShaders();
        {
            static const float moitieLargeurLigne = 1.0f;
            glPushAttrib( GL_ALL_ATTRIB_BITS );
            glPushMatrix();
            glColor3f( 0.0, 0.0, 1.0 );
            glBegin( GL_QUADS );

            NoeudTable* table = (NoeudTable*)mNode;
            const Vecteur3** tableVertices = table->GetTableVertices();
            // dessin des lignes verticals dans la table
            for( int i = 1; i < NB_HORIZONTAL_VERTICES - 1; ++i )
            {
                for( int j = 0; j < NB_VERTICAL_VERTICES - 1; ++j )
                {
                    // Dessin des lignes verticales de la table
                    const Vecteur3& cur = tableVertices[i][j];
                    const Vecteur3& down = tableVertices[i][j + 1];
                    glVertex3f( cur[VX] + moitieLargeurLigne , cur[VY] , NoeudTable::Z_HEIGHT_TABLE_SURFACE + 1 );
                    glVertex3f( cur[VX] - moitieLargeurLigne , cur[VY] , NoeudTable::Z_HEIGHT_TABLE_SURFACE + 1 );
                    glVertex3f( down[VX] - moitieLargeurLigne, down[VY], NoeudTable::Z_HEIGHT_TABLE_SURFACE + 1 );
                    glVertex3f( down[VX] + moitieLargeurLigne, down[VY], NoeudTable::Z_HEIGHT_TABLE_SURFACE + 1 );
                }
            }

            glEnd();
            glPopMatrix();
            GLUquadric* cercleCentre_ = gluNewQuadric();
            GLUquadric* centre_ = gluNewQuadric();
            // Dessin du cercle au centre de la table

            glPushMatrix();
            glColor3f( 1.0, 0.0, 0.0 );
            gluDisk( cercleCentre_, rayonCercleCentre_ - 1, rayonCercleCentre_ + 1, 32, 32 );
            gluDisk( centre_, 0, 2, 32, 32 );
            glPopMatrix();

            glPopAttrib();
            gluDeleteQuadric( centre_ );
            gluDeleteQuadric( cercleCentre_ );
        }
        FacadeModele::getInstance()->ActivateShaders();

        // Réactiver l'éclairage et (s'il y a lieu)
        if( lighting_state == GL_TRUE )
        {
            glEnable( GL_LIGHTING );
        }
#else
        //renderOpenGLES();
#endif
    }
}


void RenderOpenGL2Table::updateComponent()
{

    /// here assumes the array above is not empty
    if( mListRenderCorners )
    {
        glDeleteLists( *mListRenderCorners, NoeudTable::TABLE_CORNER_COUNT );
    }
    else
    {
        mListRenderCorners = new GLuint[NoeudTable::TABLE_CORNER_COUNT];
    }
    const GLuint listRange = glGenLists( NoeudTable::TABLE_CORNER_COUNT );

    NoeudTable* table = (NoeudTable*)mNode;
    /// Create round corner for the table
    for( int i = 0; i < NoeudTable::TABLE_CORNER_COUNT; ++i )
    {
        mListRenderCorners[i] = listRange + (GLuint)i;
        glNewList( mListRenderCorners[i], GL_COMPILE );
        Vecteur2f* locations;
        int count;
        table->GetCornerArcLocations( i, locations, count );
        if( count )
        {
            glBegin( GL_QUAD_STRIP );
            for( int i = 1; i < count; ++i )
            {
                Vecteur2f pos = locations[i];
                glVertex3f( pos.x, pos.y, NoeudTable::Z_HEIGHT_TABLE_BOARDS );
                glVertex3f( pos.x, pos.y, NoeudTable::Z_HEIGHT_TABLE_SURFACE );
            }
            glEnd();

            glBegin( GL_TRIANGLE_FAN );
            for( int i = 0; i < count; ++i )
            {
                Vecteur2f pos = locations[i];
                glVertex3f( pos.x, pos.y, NoeudTable::Z_HEIGHT_TABLE_BOARDS );
            }
            glEnd();
        }
        glEndList();
    }
}

