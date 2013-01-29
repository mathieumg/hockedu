#pragma once
#include "ObserverPaternDefinitions.h"
#include "..\Environnement\Evenements\EvenementSouris.h"

class MouseMoveSubject : public Subject<MouseMoveSubject>
{
public:
    /// Accessors of mEvent
    inline const EvenementSouris& getEvent() const { return mEvent; }
protected:
    EvenementSouris mEvent;
};

class MouseMoveObserver : public Observer<MouseMoveSubject>
{
};