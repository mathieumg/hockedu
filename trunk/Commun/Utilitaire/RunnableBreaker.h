#pragma once
#include "ObserverPaternDefinitions.h"

class RunnableBreaker : public Subject<RunnableBreaker>
{
};

class RunnableBreakerObserver : public Observer<RunnableBreaker>
{
};