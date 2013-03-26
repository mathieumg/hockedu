#pragma once
#include "ObserverPaternDefinitions.h"

class ReplaySubject : public Subject<ReplaySubject>
{
public:
    ReplaySubject():mReplaying(false){}
    bool mReplaying;
};

class ReplayObserver : public Observer<ReplaySubject>
{
};