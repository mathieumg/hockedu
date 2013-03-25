//////////////////////////////////////////////////////////////////////////////
/// @file AchievementsManager.cpp
/// @author Michael Ferris
/// @date 2013-03-24
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "AchievementsManager.h"
#include "AbstractAchievement.h"
#include "Achievements.h"

SINGLETON_DECLARATION_CPP(AchievementsManager);


////////////////////////////////////////////////////////////////////////
///
/// @fn  AchievementsManager::AchievementsManager()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AchievementsManager::AchievementsManager()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  AchievementsManager::~AchievementsManager()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AchievementsManager::~AchievementsManager()
{
    for( auto it=mEventListeners.begin(); it!= mEventListeners.end(); ++it)
    {
        if(it->second)
        {
            delete it->second;
        }
    }
    mEventListeners.clear();

    for( auto it=mAchievementProgress.begin(); it!= mAchievementProgress.end(); ++it)
    {
        if(it->second)
        {
            delete it->second;
        }
    }
    mAchievementProgress.clear();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::InitialiseAchievements()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::InitialiseAchievements()
{
    mAchievementProgress[ACHIEVEMENTS_START_APPLICATION] = new AchievementStartApp();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::LoadAchievementProgress()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::LoadAchievementProgress()
{
    /// Todo::
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::SaveAchievementProgress()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::SaveAchievementProgress()
{
    /// Todo::
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::LaunchEvent( AchievementEvent )
///
/// /*Description*/
///
/// @param[in] AchievementEvent
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::LaunchEvent( AchievementEvent pEvent )
{
    auto it = mEventListeners.find(pEvent);
    /// look for the event list
    if(it != mEventListeners.end())
    {
        // create local list of listeners, because achievements
        // can request to unregister while iterating
        const EventListenerList listernersList = *it->second;
        for(auto itA=listernersList.begin(); itA!= listernersList.end(); ++itA)
        {
            (*itA)->EventReceived(pEvent);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::RegisterAchievementEventListener( AchievementEvent,AbstractAchievement* )
///
/// Registers an achivement to a specific event type
///
/// @param[in] AchievementEvent : event type concerned
/// @param[in] AbstractAchievement * : achievement requesting
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::RegisterAchievementEventListener( AchievementEvent pEvent,AbstractAchievement* pAchievement)
{
    auto it = mEventListeners.find(pEvent);
    if(it == mEventListeners.end())
    {
        mEventListeners[pEvent] = new EventListenerList();
    }
    mEventListeners[pEvent]->insert(pAchievement);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::UnregisterAchievementEventListener( AchievementEvent,AbstractAchievement* )
///
/// Removes an achievement from the list of event lister for this event
///
/// @param[in] AchievementEvent
/// @param[in] AbstractAchievement *
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::UnregisterAchievementEventListener( AchievementEvent pEvent,AbstractAchievement* pAchievement )
{
    auto it = mEventListeners.find(pEvent);
    /// look for the event list
    if(it != mEventListeners.end())
    {
        /// list found, search for the achievement
        EventListenerList* listernersList = it->second;
        auto a = listernersList->find(pAchievement);

        if(a != listernersList->end())
        {
            /// achievement found, remove it
            listernersList->erase(a);
        }

        if(listernersList->empty())
        {
            /// no more listeners in the list, remove it
            delete listernersList;
            mEventListeners.erase(it);
        }
    }
}

