//////////////////////////////////////////////////////////////////////////////
/// @file Achievements.h
/// @author Michael Ferris
/// @date 2013-03-24
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "AbstractAchievement.h"
#include <vector>

typedef class TiXmlElement XmlElement;
class AbstractAchievement;

typedef void (*AchievementEventReceived)(AbstractAchievement*,AchievementEvent);


struct Binding
{
    Binding(AbstractAchievement* achievement,AchievementEventReceived callBack):
        mAchievement(achievement),mCallBack(callBack)
    {

    }
    AbstractAchievement* mAchievement;
    AchievementEventReceived mCallBack;
};
/// Structure pour le binding d'un evenement avec une callback pour la gestion de cet evenement par l'achievement
struct AchievementBinding
{
    AchievementBinding(AchievementEvent eventBinded, Binding* binding):
        mBinding(binding),mEventBinded(eventBinded)
    {

    }
    AchievementBinding()
    {

    }
    ~AchievementBinding()
    {
        if(mBinding)
        {
            delete mBinding;
        }
    }
    Binding* mBinding;
    AchievementEvent mEventBinded;
};



struct AchievementLevelDefinition
{
    AchievementLevelDefinition()
    {

    }
    ~AchievementLevelDefinition()
    {
        for(int i=0; i<(int)mBindings.size(); ++i)
        {
            if(mBindings[i])
            {
                delete mBindings[i];
            }
        }
        mBindings.clear();
    }
    typedef std::vector<AchievementBinding*> LevelBindings;
    LevelBindings mBindings;
};

///////////////////////////////////////////////////////////////////////////
/// @class AbstractAchievement
/// @brief Classe représentant l'interface et les éléments de base d'un achievement.
///
///
/// @author Michael Ferris
/// @date 2013-03-24
///////////////////////////////////////////////////////////////////////////
class AbstractAchievement
{
public:
    AbstractAchievement():mLevelUnlocked(0), mHasProgressed(false)
    {

    }
    virtual ~AbstractAchievement();
    bool LoadAchievementNode(const XmlElement* root);
    void CreateAchievementNode(XmlElement*) const;

    void InitFirstLevel();
    virtual AchievementsType GetFirstType() const = 0;
protected:
    void GoToNextLevel();
    void RegisterLevel(unsigned int level) const;
    void UnRegisterLevel(unsigned int level) const;


    /// Overload this method to add parameters to the achievement persistency
    virtual void FillAchievementData(XmlElement* elem) const {}
    virtual bool LoadAchievementData(const XmlElement* elem) { return true; }

    /// Parametre a configurer dans le constructeur des achivements concrets puis ne plus les modifiers !
    std::vector<AchievementLevelDefinition*> mLevelDefinitions;

    unsigned int mLevelUnlocked;
    unsigned int mNbLevels;

    bool mHasProgressed;
    
};

///////////////////////////////////////////////////////////////////////////
/// @class AchievementStartApp
/// @brief Classe représentant l'interface et les éléments de base d'un achievement.
///
///
/// @author Michael Ferris
/// @date 2013-03-24
///////////////////////////////////////////////////////////////////////////
class AchievementStartApp : public AbstractAchievement
{
public:
    AchievementStartApp();
protected:
    static void EventStartCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
    virtual AchievementsType GetFirstType() const{return ACHIEVEMENTS_START_APPLICATION;}
};

///////////////////////////////////////////////////////////////////////////
/// @class AchievementGameWon
/// @brief Achievement based on game wining.
///
///
/// @author Michael Ferris
/// @date 2013-03-24
///////////////////////////////////////////////////////////////////////////
class AchievementGameWon : public AbstractAchievement
{
public:
    AchievementGameWon();
protected:
    static void EventWinCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
    virtual void FillAchievementData(XmlElement* elem) const;
    virtual bool LoadAchievementData(const XmlElement* elem);
    virtual AchievementsType GetFirstType() const{return ACHIEVEMENTS_GAME_WON_L1;}
private:
    int mNbGameWon;

};

///////////////////////////////////////////////////////////////////////////
/// @class AchievementAICreation
/// @brief Achievement based on AI profiles creation.
///
///
/// @author Michael Ferris
/// @date 2013-03-24
///////////////////////////////////////////////////////////////////////////
class AchievementAICreation : public AbstractAchievement
{
public:
    AchievementAICreation();
protected:
    static void EventAICreatedCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
    virtual void FillAchievementData(XmlElement* elem) const;
    virtual bool LoadAchievementData(const XmlElement* elem);
    virtual AchievementsType GetFirstType() const{return ACHIEVEMENTS_AI_CREATION_L1;}
private:
    int mNbAiCreated;
    static const int AI_CREATION_NEEDED[3];
    static const std::string AI_LEVEL_NAME[3];
};

///////////////////////////////////////////////////////////////////////////
/// @class AchievementMute
/// @brief Achievement obtained by muting music.
///
///
/// @author Vincent Lemire
/// @date 2013-04-05
///////////////////////////////////////////////////////////////////////////
class AchievementMute : public AbstractAchievement
{
public:
    AchievementMute();
protected:
    static void EventMuteCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
    virtual void FillAchievementData(XmlElement* elem) const;
    virtual bool LoadAchievementData(const XmlElement* elem);
    virtual AchievementsType GetFirstType() const{return ACHIEVEMENTS_MUTE;}
private:
    static const std::string MUTE_LEVEL_NAME[1];
    bool mMuted;
};

///////////////////////////////////////////////////////////////////////////
/// @class AchievementPortal
/// @brief Achievement obtained by using portal.
///
///
/// @author Vincent Lemire
/// @date 2013-04-05
///////////////////////////////////////////////////////////////////////////
class AchievementPortal : public AbstractAchievement
{
public:
    AchievementPortal();
protected:
    static void EventPortalCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
    virtual void FillAchievementData(XmlElement* elem) const;
    virtual bool LoadAchievementData(const XmlElement* elem);
    virtual AchievementsType GetFirstType() const{return ACHIEVEMENTS_PORTAL_L1;}
private:
    static const std::string PORTAL_LEVEL_NAME[4];
    static const int PORTAL_NEEDED[4];
    int mNbrPortalCreated;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

