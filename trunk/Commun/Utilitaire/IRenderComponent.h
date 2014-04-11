#pragma once
class NoeudAbstrait;

class IRenderComponent
{
public:
    // Interface
    virtual void render() const = 0;
    virtual void updateComponent() = 0;

public:
    virtual ~IRenderComponent(){}
    inline void setNode( NoeudAbstrait* n ){mNode = n;}
protected:
    const NoeudAbstrait* mNode;
};

