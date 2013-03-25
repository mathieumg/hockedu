#include "LaunchAchievementLite.h"
#include "AchievementsManager.h"

namespace Achievements
{
    void LaunchEvent(AchievementEvent pEvent)
    {
        if(AchievementsManager::Exists())
        {
            AchievementsManager::obtenirInstance()->LaunchEvent(pEvent);
        }
    }
}