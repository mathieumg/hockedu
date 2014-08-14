#pragma once

#include "RenderOpenGL2.h"

class RenderOpenGL2Table : public RenderOpenGL2
{
    GLuint* mListRenderCorners;
public:
    RenderOpenGL2Table();
    ~RenderOpenGL2Table();
    virtual void render() const;
    virtual void updateComponent();
};