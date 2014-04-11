#pragma once

#include "IRenderComponent.h"
#include "Vecteur.h"

class RenderOpenGL2 : public IRenderComponent
{
public:
    RenderOpenGL2();
    virtual void render() const;
    virtual void updateComponent( );

protected:
    /// Polygon draw type, constant for now since we dont need to change it
    const unsigned int  mModePolygones;
    /// Matrice de transformation.
    float mTransformationMatrix[16];

    void renderNormalNode() const;

private:
    unsigned int mGlId;
    static unsigned int mIdGlCounter;
};