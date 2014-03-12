#pragma once

#include "IRenderComponent.h"
#include "Vecteur.h"
#include "glew.h"

typedef void( *OpenGL2RenderFunction )( RenderOpenGL2* );
class RenderOpenGL2 : public IRenderComponent
{
public:
    RenderOpenGL2();
    virtual void render() const;
    virtual void updateComponent( );
private:
    Vecteur3 mScale;
    /// Polygon draw type, constant for now since we dont need to change it
    const GLenum  mModePolygones;
    /// Matrice de transformation.
    GLfloat mTransformationMatrix[16];
    GLuint mGlId;
    static GLuint mIdGlCounter;
    OpenGL2RenderFunction mRenderFunction;
private:
    static void renderNormalNode( RenderOpenGL2* );
};