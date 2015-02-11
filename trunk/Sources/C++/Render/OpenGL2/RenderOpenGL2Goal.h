#pragma once

#include "RenderOpenGL2.h"

class RenderOpenGL2Goal : public RenderOpenGL2
{
public:
    virtual void render() const;
    virtual void updateComponent();
};