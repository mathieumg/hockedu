#pragma once
#include <set>

class PositionObserver;

class PositionSubject
{
public:
    virtual ~PositionSubject();
    void attach(PositionObserver& o);
    void detach(PositionObserver& o);
    void signalModification();
private:
    std::set<PositionObserver*> mObservers;
};

