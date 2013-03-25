#include "Achievements.h"
#include "AchievementsManager.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////
///
/// @fn  AchievementStartApp::AchievementStartApp()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AchievementStartApp::AchievementStartApp()
{
    AchievementsManager::obtenirInstance()->RegisterAchievementEventListener(ACHIEVEMENT_EVENT_MAIN_SCREEN_LOADED,this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementStartApp::EventReceived( AchievementEvent pEvent )
///
/// /*Description*/
///
/// @param[in] AchievementEvent pEvent
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementStartApp::EventReceived( AchievementEvent pEvent )
{
    std::cout << "Achievement Unlocked : Start Application";
    AchievementsManager::obtenirInstance()->UnregisterAchievementEventListener(ACHIEVEMENT_EVENT_MAIN_SCREEN_LOADED,this);
}

