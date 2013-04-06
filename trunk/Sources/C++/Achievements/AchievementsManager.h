//////////////////////////////////////////////////////////////////////////////
/// @file AchievementsManager.h
/// @author Michael Ferris
/// @date 2013-03-24
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include <map>
#include "AchievementsEnums.h"
#include <set>
#include <string>
#include "Achievements.h"

class AbstractAchievement;

typedef int (*AchievementUnlockCallBack)(AchievementsType pType, char* pMessage);
struct AchievementBinding;
typedef void (*AchievementEventReceived)(AbstractAchievement*,AchievementEvent);

class AchievementsTests;
///////////////////////////////////////////////////////////////////////////
/// @class AchievementsManager
/// @brief Classe effectuant la gestion des achievements. 
///
/// Point d'entré pour lancé des événements qui peuvent débloquer des achievements
/// Si un achievement est débloquer, il se détache de l'événement.
/// Les méthodes InitialiseAchievements() et LoadAchievementProgress() permettre d'initialiser 
/// les achievements et donc le gestionnaire n'est rien sans ces appels.
///
///
/// @author Michael Ferris
/// @date 2013-03-24
///////////////////////////////////////////////////////////////////////////
class AchievementsManager : public Singleton<AchievementsManager>
{
    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(AchievementsManager);
    AchievementsManager();
    ~AchievementsManager();


public:
    friend AchievementsTests;
    void InitialiseAchievements();


    void LoadAchievementProgress();
    void SaveAchievementProgress();

    void LaunchEvent(AchievementEvent);
    void RegisterAchievementEventListener(const AchievementBinding&);
    void UnregisterAchievementEventListener(const AchievementBinding&);

    inline void setAchievementUnlockedCallback( AchievementUnlockCallBack pVal) { mAchievementUnlockedCallback = pVal; }
    void AchievementUnlocked(AchievementsType pType, const std::string& pAchievementName);

    /// Fonction pour des tests
    void ResetAchievements();

private:
    typedef std::set<Binding*> EventListenerList;
    std::map<AchievementsType,AbstractAchievement*> mAchievementProgress;
    std::map<AchievementEvent,EventListenerList*> mEventListeners;
    AchievementUnlockCallBack mAchievementUnlockedCallback;

    void CreateAchievements();
    void ClearMemory();
};



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

