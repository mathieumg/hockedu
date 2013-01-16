#pragma once
#include "visiteurnoeud.h"

#include <functional>

class VisiteurFunction :
    public VisiteurNoeud
{
public:
    VisiteurFunction(std::function<void (NoeudAbstrait*)>);
    ~VisiteurFunction(void);

    /// Visitation d'un noeud abstrait
    virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );

private:
    std::function<void (NoeudAbstrait*)> mFunction;


};

