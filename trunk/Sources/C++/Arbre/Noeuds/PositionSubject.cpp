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
PositionSubject::~PositionSubject()
{
    for ( std::set<PositionObserver*>::iterator
        it = mObservers.begin();
        it != mObservers.end();
    ++it )
    {
        (*it)->removeSubject(*this);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void PositionSubject::attach( PositionObserver& o )
///
/// /*Description*/
///
/// @param[in] PositionObserver & o
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PositionSubject::attach( PositionObserver& o )
{
    mObservers.insert( &o );
    o.addSubject(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void PositionSubject::detach( PositionObserver& o )
///
/// /*Description*/
///
/// @param[in] PositionObserver & o
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PositionSubject::detach( PositionObserver& o )
{
    mObservers.erase(
        mObservers.find( &o ) );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void PositionSubject::signalModification()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PositionSubject::signalModification()
{
    for ( std::set<PositionObserver*>::iterator
        it = mObservers.begin();
        it != mObservers.end();
    ++it )
    {
        (*it)->updatePosition(*this);
    }
}
