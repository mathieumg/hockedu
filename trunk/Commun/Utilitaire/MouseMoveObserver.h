#pragma once
#include "ObserverPaternDefinitions.h"
#include "Enum_Declarations.h"
#include "Vecteur.h"

class MouseMoveSubject : public Subject<MouseMoveSubject>
{
public:
    Vecteur3 mVirtualPos;
    BoutonSouris mButton;
};

class MouseMoveObserver : public Observer<MouseMoveSubject>
{
};