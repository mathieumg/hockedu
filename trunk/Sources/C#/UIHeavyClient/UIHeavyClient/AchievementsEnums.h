#if !WIN32 && !__linux__ && !__APPLE__
#define CSHARP
#endif


#if CSHARP
namespace UIHeavyClient
{
#else
#pragma once
#endif


/// L'ordre des enums est tres important,
/// il s'agit de leur ID pour les reconnaitres over-time
/// donc ne pas jouer avec ceux-ci.
/// laisser un espace suffisant entre 2 achievements pour des nouveaux
#if CSHARP
public enum AchievementsType
#else
typedef enum
#endif
{
    ACHIEVEMENTS_START_APPLICATION = 100,

    ACHIEVEMENTS_GAME_WON_L1 = 200,
    ACHIEVEMENTS_GAME_WON_L2,
    ACHIEVEMENTS_GAME_WON_L3,
    ACHIEVEMENTS_GAME_WON_L4,
    ACHIEVEMENTS_GAME_WON_L5,

    ACHIEVEMENTS_AI_CREATION_L1 = 250,
    ACHIEVEMENTS_AI_CREATION_L2,
    ACHIEVEMENTS_AI_CREATION_L3,

    ACHIEVEMENTS_MUTE = 300,

    ACHIEVEMENTS_PORTAL_L1 = 350,
    ACHIEVEMENTS_PORTAL_L2,
    ACHIEVEMENTS_PORTAL_L3,
}
#if !CSHARP
AchievementsType
#endif
;

/// Ordre pas important.
/// Sert au programme d'indiquer un type d'�v�nement qui peut
/// possiblement etre �couter par un ou plusieurs achievement
#if CSHARP
public enum AchievementEvent
#else
typedef enum
#endif
{
    ACHIEVEMENT_EVENT_MAIN_SCREEN_LOADED,
    ACHIEVEMENT_EVENT_GAME_WON,
    ACHIEVEMENT_EVENT_GAME_LOOSE,
    ACHIEVEMENT_EVENT_AI_CREATED,
    ACHIEVEMENT_EVENT_MUTE,
    ACHIEVEMENT_EVENT_PORTAL,

    ACHIEVEMENT_EVENT_NONE, // When you need to specify an event, but there's no event
}
#if !CSHARP
AchievementEvent
#endif
    ;

#if CSHARP
}
#endif