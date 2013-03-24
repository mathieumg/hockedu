#pragma once
#include "ObserverPaternDefinitions.h"
#include "EvenementSouris.h"

class MouseMoveSubject : public Subject<MouseMoveSubject>
{
public:
    /// Accessors of mEvent
    EvenementSouris mEvent;
};

class MouseMoveObserver : public Observer<MouseMoveSubject>
{
};