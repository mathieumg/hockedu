#pragma once
#include "ObserverPaternMacros.h"

class PositionSubject : public Subject<PositionSubject>
{
};

class PositionObserver : public Observer<PositionSubject>
{
};