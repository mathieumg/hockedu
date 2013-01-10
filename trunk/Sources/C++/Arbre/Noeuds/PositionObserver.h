#pragma once
#include <set>

class PositionObserver
{
public:
    virtual ~PositionObserver();
    virtual void updatePosition(class PositionSubject& pSubject) = 0;
    void addSubject(class PositionSubject& pSubject);
    void removeSubject(class PositionSubject& pSubject);
private:
    std::set<class PositionSubject*> mSubjects;
};

