#if !WIN32 && !__linux__ && !__APPLE__
namespace UIHeavyClient
{
#else
#pragma once
#endif


/// L'ordre des enums est tres important,
/// il s'agit de leur ID pour les reconnaitres over-time
/// donc ne pas jouer avec ceux-ci.
/// laisser un espace suffisant entre 2 achievements pour des nouveaux
#if !WIN32 && !__linux__ && !__APPLE__
public
#endif
enum AchievementsType
{
    ACHIEVEMENTS_START_APPLICATION = 100,








};

/// Ordre pas important.
/// Sert au programme d'indiquer un type d'événement qui peut
/// possiblement etre écouter par un ou plusieurs achievement
#if !WIN32 && !__linux__ && !__APPLE__
public
#endif
enum AchievementEvent
{
    ACHIEVEMENT_EVENT_MAIN_SCREEN_LOADED,




};

#if !WIN32 && !__linux__ && !__APPLE__
}
#endif