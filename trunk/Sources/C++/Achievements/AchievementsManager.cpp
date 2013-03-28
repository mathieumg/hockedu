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
#include <iostream>
#include "sqlite3.h"
#include <sstream>
#include "XMLUtils.h"

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
AchievementsManager::AchievementsManager() : mAchievementUnlockedCallback(NULL)
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
/// @fn void AchievementsManager::CreateAchievements()
///
/// Creates the achievements list empty and fresh
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::CreateAchievements()
{
    mAchievementProgress[ACHIEVEMENTS_START_APPLICATION] = new AchievementStartApp();
    mAchievementProgress[ACHIEVEMENTS_GAME_WON_L1] = new AchievementGameWon();


    // Initialisation de base des niveaux
    for( auto it=mAchievementProgress.begin(); it!= mAchievementProgress.end(); ++it)
    {
        if(it->second)
        {
            it->second->InitFirstLevel();
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::InitialiseAchievements()
///
/// Creates the achievements list and load current user progress
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::InitialiseAchievements()
{
    CreateAchievements();
    LoadAchievementProgress();
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
    XmlDocument document ;
    if(XMLUtils::LoadDocument(document,"Achievements.xml"))
    {
        XmlElement* achievementRoot = XMLUtils::FirstChildElement(document.GetElem(),"Achievements");

        // �crire dans le fichier
        for( auto it=mAchievementProgress.begin(); it!= mAchievementProgress.end(); ++it)
        {
            if(it->second)
            {
                if(!it->second->LoadAchievementNode(achievementRoot))
                {
                    /// noeud n'a pas charger son noeud
                    /// comportement possible, mais checkf pour s'assurer que callback'est desire
                    std::cout << "Error loading achievement data " << it->second->GetXmlTag() << std::endl;
                }
            }
        }
    }

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
    XmlDocument document ;
    XMLUtils::CreateDocument(document);

    XmlElement* achievementRoot = XMLUtils::createNode("Achievements");
    XMLUtils::LinkEndChild(document,achievementRoot);

    // �crire dans le fichier
    for( auto it=mAchievementProgress.begin(); it!= mAchievementProgress.end(); ++it)
    {
        if(it->second)
        {
            auto elem = it->second->CreateAchievementNode();
            if(elem)
            {
                XMLUtils::LinkEndChild(achievementRoot,elem);
            }
        }
    }

    XMLUtils::SaveDocument(document,"Achievements.xml");
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
            auto callback = (*itA)->mCallBack;
            if(callback)
            {
                callback((*itA)->mAchievement,pEvent);
            }
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
void AchievementsManager::RegisterAchievementEventListener(const AchievementBinding& pBinding)
{
    if(pBinding.mBinding && pBinding.mBinding->mCallBack)
    {
        auto it = mEventListeners.find(pBinding.mEventBinded);
        if(it == mEventListeners.end())
        {
            mEventListeners[pBinding.mEventBinded] = new EventListenerList();
        }
        mEventListeners[pBinding.mEventBinded]->insert(pBinding.mBinding);
    }
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
void AchievementsManager::UnregisterAchievementEventListener( const AchievementBinding& pBinding )
{
    auto it = mEventListeners.find(pBinding.mEventBinded);
    /// look for the event list
    if(it != mEventListeners.end())
    {
        /// list found, search for the achievement
        EventListenerList* listernersList = it->second;
        auto a = listernersList->find(pBinding.mBinding);

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

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsManager::AchievementUnlocked( AchievementsType pType, const std::string& pAchievementName )
///
/// /*Description*/
///
/// @param[in] AchievementsType pType
/// @param[in] const std::string & pAchievementName
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsManager::AchievementUnlocked( AchievementsType pType, const std::string& pAchievementName )
{
    if(!mAchievementUnlockedCallback || !mAchievementUnlockedCallback(pType,(char*)pAchievementName.c_str()))
    {
        std::cout << "Achievement Unlocked : " << pAchievementName << std::endl;
    }
    SaveAchievementProgress();
}
