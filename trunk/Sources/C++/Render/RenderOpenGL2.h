#pragma once

#include "IRenderComponent.h"
#include "Vecteur.h"
#include "glew.h"

typedef void( *OpenGL2RenderFunction )( const class RenderOpenGL2* );
class RenderOpenGL2 : public IRenderComponent
{
public:
    RenderOpenGL2();
    virtual void render() const;
    virtual void updateComponent( );
private:
    /// Polygon draw type, constant for now since we dont need to change it
    const GLenum  mModePolygones;
    /// Matrice de transformation.
    GLfloat mTransformationMatrix[16];
    GLuint mGlId;
    static GLuint mIdGlCounter;

    void updateMatrice();
private:
    static void PopulateRenderFunctionMap();
    static void renderNormalNode( const RenderOpenGL2* );
    static void renderBonus( const RenderOpenGL2* );
};