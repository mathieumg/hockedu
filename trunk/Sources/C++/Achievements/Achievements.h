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
    AbstractAchievement():mLevelUnlocked(0)
    {

    }
    virtual ~AbstractAchievement();
    bool LoadAchievementNode(const XmlElement* root);
    XmlElement* CreateAchievementNode() const;

    void InitFirstLevel() const;
    virtual std::string GetXmlTag()const = 0;
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
private:
    const XmlElement* GetXmlElement(const XmlElement* root) const;
    
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
    virtual std::string GetXmlTag()const{return "StartApp";}
protected:
    static void EventStartCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
};

class AchievementGameWon : public AbstractAchievement
{
public:
    AchievementGameWon();
    virtual std::string GetXmlTag()const{return "WinGame";}
protected:
    static void EventWinCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
    virtual void FillAchievementData(XmlElement* elem) const;
    virtual bool LoadAchievementData(const XmlElement* elem);
private:
    int mNbGameWon;

};

class AchievementAICreation : public AbstractAchievement
{
public:
    AchievementAICreation();
    virtual std::string GetXmlTag()const{return "AiCreation";}
protected:
    static void EventAICreatedCallBack(AbstractAchievement* pAchievement, AchievementEvent pEvent);
    virtual void FillAchievementData(XmlElement* elem) const;
    virtual bool LoadAchievementData(const XmlElement* elem);
private:
    int mNbAiCreated;
    static const int AI_CREATION_NEEDED[3];
    static const std::string AI_LEVEL_NAME[3];
};



















///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

