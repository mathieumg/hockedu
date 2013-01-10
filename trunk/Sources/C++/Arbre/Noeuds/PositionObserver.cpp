#pragma once
#include "PositionSubject.h"
#include "PositionObserver.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  PositionObserver::~PositionObserver()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
PositionObserver::~PositionObserver()
{
    for ( std::set<PositionSubject*>::iterator
        it = mSubjects.begin();
        it != mSubjects.end();
    ++it )
    {
        (*it)->detach(*this);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void PositionObserver::removeSubject( class PositionSubject& pSubject )
///
/// /*Description*/
///
/// @param[in] class PositionSubject & pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PositionObserver::removeSubject( class PositionSubject& pSubject )
{
    mSubjects.erase(
        mSubjects.find( &pSubject ) );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void PositionObserver::addSubject( class PositionSubject& pSubject )
///
/// /*Description*/
///
/// @param[in] class PositionSubject & pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PositionObserver::addSubject( class PositionSubject& pSubject )
{
    mSubjects.insert(&pSubject);
}
