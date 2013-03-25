#include "Achievements.h"
#include "AchievementsManager.h"
#include "XMLUtils.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  AbstractAchievement::~AbstractAchievement()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AbstractAchievement::~AbstractAchievement()
{
    for(int i=0; i<(int)mLevelDefinitions.size(); ++i)
    {
        if(mLevelDefinitions[i])
        {
            delete mLevelDefinitions[i];
        }
    }
    mLevelDefinitions.clear();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void AbstractAchievement::InitFirstLevel()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AbstractAchievement::InitFirstLevel() const
{
    RegisterLevel(0);
}


void AbstractAchievement::RegisterLevel(unsigned int level) const
{
    if((unsigned int)mLevelDefinitions.size() > level)
    {
        auto currentLevel = mLevelDefinitions[level];
        if(currentLevel)
        {
            const AchievementLevelDefinition::LevelBindings& bindings = currentLevel->mBindings;
            for(auto itLevel = bindings.begin(); itLevel != bindings.end(); ++itLevel)
            {
                auto currentBinding = *itLevel;
                if(currentBinding)
                {
                    AchievementsManager::obtenirInstance()->RegisterAchievementEventListener(*currentBinding);
                }
            }
        }
    }
}


void AbstractAchievement::UnRegisterLevel(unsigned int level) const
{
    if((unsigned int)mLevelDefinitions.size() > level)
    {
        auto currentLevel = mLevelDefinitions[level];
        if(currentLevel)
        {
            const AchievementLevelDefinition::LevelBindings& bindings = currentLevel->mBindings;
            for(auto itLevel = bindings.begin(); itLevel != bindings.end(); ++itLevel)
            {
                auto currentBinding = *itLevel;
                if(currentBinding)
                {
                    AchievementsManager::obtenirInstance()->UnregisterAchievementEventListener(*currentBinding);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AbstractAchievement::GoToNextLevel()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AbstractAchievement::GoToNextLevel()
{
    UnRegisterLevel(mLevelUnlocked);
    ++mLevelUnlocked;
    RegisterLevel(mLevelUnlocked);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* AbstractAchievement::CreateAchievementNode()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* AbstractAchievement::CreateAchievementNode() const
{
    XmlElement* elem = XMLUtils::createNode(GetXmlTag().c_str());
    XMLUtils::writeAttribute(elem,"levelUnlocked",(int&)mLevelUnlocked);
    FillAchievementData(elem);
    return elem;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool AbstractAchievement::LoadAchievementNode( XmlElement* root )
///
/// /*Description*/
///
/// @param[in] XmlElement * root
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool AbstractAchievement::LoadAchievementNode( const XmlElement* root )
{
    const XmlElement* elem = GetXmlElement(root);
    if(elem)
    {
        if(!XMLUtils::readAttribute(elem,"levelUnlocked",(int&)mLevelUnlocked))
        {
            return false;
        }
        if(mLevelUnlocked != 0)
        {
            UnRegisterLevel(0);
            RegisterLevel(mLevelUnlocked);
        }

        return LoadAchievementData(elem);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* AbstractAchievement::GetXmlElement()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
const XmlElement* AbstractAchievement::GetXmlElement(const XmlElement* root) const
{
    return XMLUtils::FirstChildElement(root,GetXmlTag().c_str());
}



#define ADD_NEW_LEVEL(code)                                               \
{                                                                         \
    AchievementLevelDefinition* level = new AchievementLevelDefinition(); \
    code                                                                  \
    mLevelDefinitions.push_back(level);                                   \
}

#define APPEND_BINDING(event,callback) \
    level->mBindings.push_back(new AchievementBinding(event,new Binding(this,callback)));



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
    ADD_NEW_LEVEL
    (
        APPEND_BINDING(ACHIEVEMENT_EVENT_MAIN_SCREEN_LOADED,EventStartCallBack)
    );
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
void AchievementStartApp::EventStartCallBack( AbstractAchievement* pAchievement ,AchievementEvent pEvent )
{
    auto achievement = (AchievementStartApp*)pAchievement;
    achievement->GoToNextLevel();
    AchievementsManager::obtenirInstance()->AchievementUnlocked(ACHIEVEMENTS_START_APPLICATION,"Start Application");
}




