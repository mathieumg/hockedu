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
protected:
    void GoToNextLevel();
    void RegisterLevel(unsigned int level) const;
    void UnRegisterLevel(unsigned int level) const;
    

    /// Overload this method to add parameters to the achievement persistency
    virtual void FillAchievementData(XmlElement*) const {}
    virtual bool LoadAchievementData(const XmlElement*) { return true; }
    virtual std::string GetXmlTag()const = 0;

    /// Parametre a configurer dans le constructeur des achivements concrets puis ne plus les modifiers !
    std::vector<AchievementLevelDefinition*> mLevelDefinitions;

    unsigned int mLevelUnlocked;
private:
    const XmlElement* GetXmlElement(const XmlElement* root) const;
    
};

///////////////////////////////////////////////////////////////////////////
/// @class AbstractAchievement
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
    static void EventStartCallBack(AbstractAchievement*,AchievementEvent);
    virtual std::string GetXmlTag()const{return "StartApp";}
};



















///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

