#pragma once
class NoeudAbstrait;

class IRenderComponent
{
public:
    virtual ~IRenderComponent(){}
    virtual void render() const = 0;
    virtual void updateComponent() = 0;
    inline void setNode( NoeudAbstrait* n ){mNode = n;}
protected:
    class NoeudAbstrait* mNode;
};

