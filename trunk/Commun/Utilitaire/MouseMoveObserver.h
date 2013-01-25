#pragma once
#include <set>
#include "ObserverPaternMacros.h"
#include "..\Environnement\Evenements\EvenementSouris.h"

class MouseMoveSubject
{
    SUBJECT_DECLARATION(MouseMoveSubject,MouseMoveObserver)
public:
    EvenementSouris mEvent;
};

class MouseMoveObserver
{
    OBSERVER_DECLARATION(MouseMoveObserver,MouseMoveSubject)
};