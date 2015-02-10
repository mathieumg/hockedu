#pragma once

#include "RenderOpenGL2.h"

class RenderOpenGL2Table : public RenderOpenGL2
{
    uint mListRenderCorners;
    uint mTableLinesList;
public:
    RenderOpenGL2Table();
    ~RenderOpenGL2Table();
    virtual void render() const;
    virtual void updateComponent();
};