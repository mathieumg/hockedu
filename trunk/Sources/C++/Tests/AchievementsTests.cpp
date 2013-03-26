////////////////////////////////////////////////////////////////////////////////////
/// @file AchievementsTests.cpp
/// @author Michael Ferris
/// @date 2013-03-25
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "AchievementsTests.h"
#include "AchievementsManager.h"
#include "LaunchAchievementLite.h"
#include <algorithm>

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( AchievementsTests );

std::vector<AchievementsType> AchievementsTests::mAchievementUnlocked;
#define IsAchievementUnlocked(x)  find(mAchievementUnlocked.begin(),mAchievementUnlocked.end(),x) != mAchievementUnlocked.end()

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsTests::AchievementUnlocked( AchievementsType pType, char* pMessage )
///
/// /*Description*/
///
/// @param[in] AchievementsType pType
/// @param[in] char * pMessage
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int AchievementsTests::AchievementUnlocked( AchievementsType pType, char* pMessage )
{
    mAchievementUnlocked.push_back(pType);
    return 1;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsTests::setUp()
///
/// Effectue l'initialisation pr�alable � l'ex�cution de l'ensemble des
/// cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets doivent �tre construits, il est conseill� de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void AchievementsTests::setUp()
{
    AchievementsManager::obtenirInstance()->CreateAchievements();
    AchievementsManager::obtenirInstance()->setAchievementUnlockedCallback(AchievementUnlocked);
    mAchievementUnlocked.clear();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsTests::tearDown()
///
/// Effectue les op�rations de finalisation n�cessaires suite � l'ex�cution
/// de l'ensemble des cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets ont �t� allou�s � l'initialisation, ils doivent �tre
/// d�sallou�s, et il est conseill� de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void AchievementsTests::tearDown()
{
    AchievementsManager::libererInstance();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsTests::testAchievementStartApp()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsTests::testAchievementStartApp()
{
    Achievements::LaunchEvent(ACHIEVEMENT_EVENT_MAIN_SCREEN_LOADED);
    CPPUNIT_ASSERT(IsAchievementUnlocked(ACHIEVEMENTS_START_APPLICATION));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementsTests::testAchievementGameWon()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementsTests::testAchievementGameWon()
{
    Achievements::LaunchEvent(ACHIEVEMENT_EVENT_GAME_WON);
    Achievements::LaunchEvent(ACHIEVEMENT_EVENT_GAME_WON);
    Achievements::LaunchEvent(ACHIEVEMENT_EVENT_GAME_WON);
    Achievements::LaunchEvent(ACHIEVEMENT_EVENT_GAME_WON);
    Achievements::LaunchEvent(ACHIEVEMENT_EVENT_GAME_WON);

    CPPUNIT_ASSERT(IsAchievementUnlocked(ACHIEVEMENTS_GAME_WON_L1));
    CPPUNIT_ASSERT(IsAchievementUnlocked(ACHIEVEMENTS_GAME_WON_L2));
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


