#pragma once
#include "ObserverPaternDefinitions.h"

class PositionSubject : public Subject<PositionSubject>
{
};

class PositionObserver : public Observer<PositionSubject>
{
};