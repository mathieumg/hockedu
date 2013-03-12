#pragma once
#include "visiteurnoeud.h"

typedef void(*VisiteurFunctionFunction)(NoeudAbstrait*, void*);
#define VISITEUR_FUNC_FUNC_DECLARATION(name) void name(NoeudAbstrait* pNoeud, void* pUserData)
class VisiteurFunction :
    public VisiteurNoeud
{
public:
    VisiteurFunction(VisiteurFunctionFunction pFunc, void * pUserData = NULL);
    ~VisiteurFunction(void);

    /// Visitation d'un noeud abstrait
    virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );

private:
    VisiteurFunctionFunction mFunction;
    void * mUserData;
};

