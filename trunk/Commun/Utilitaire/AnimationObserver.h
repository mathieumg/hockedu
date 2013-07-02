#pragma once
#include "ObserverPaternDefinitions.h"

class AnimationSubject : public Subject<AnimationSubject>
{
};

class AnimationObserver : public Observer<AnimationSubject>
{
};

