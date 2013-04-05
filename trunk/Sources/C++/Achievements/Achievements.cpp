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



const int NbGameWinNeeded[] =
{
    1,5,20,50,100
};
const std::string achievementName[] =
{
    "Welcome to the victor's club",
    "Game won L2 ( 5 Games )",
    "Game won L3 ( 20 Games )",
    "Game won L4 ( 50 Games )",
    "Game won L5 ( 100 Games )",
};



////////////////////////////////////////////////////////////////////////
///
/// @fn  AchievementGameWon::AchievementGameWon()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
AchievementGameWon::AchievementGameWon():
    mNbGameWon(0)
{
    ADD_NEW_LEVEL
        (
        APPEND_BINDING(ACHIEVEMENT_EVENT_GAME_WON,EventWinCallBack)
        );

    ADD_NEW_LEVEL
        (
        APPEND_BINDING(ACHIEVEMENT_EVENT_GAME_WON,EventWinCallBack)
        );

    ADD_NEW_LEVEL
        (
        APPEND_BINDING(ACHIEVEMENT_EVENT_GAME_WON,EventWinCallBack)
        );

    ADD_NEW_LEVEL
        (
        APPEND_BINDING(ACHIEVEMENT_EVENT_GAME_WON,EventWinCallBack)
        );

    ADD_NEW_LEVEL
        (
        APPEND_BINDING(ACHIEVEMENT_EVENT_GAME_WON,EventWinCallBack)
        );
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementGameWon::EventWinCallBack( AbstractAchievement*,AchievementEvent )
///
/// /*Description*/
///
/// @param[in] AbstractAchievement *
/// @param[in] AchievementEvent
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementGameWon::EventWinCallBack( AbstractAchievement* pAchievement, AchievementEvent pEvent )
{

    auto achievement = (AchievementGameWon*)pAchievement;
    int n = ++achievement->mNbGameWon;
    
    int level = achievement->mLevelUnlocked;
    if(level < ARRAY_COUNT(NbGameWinNeeded) && n >= NbGameWinNeeded[level])
    {
        achievement->GoToNextLevel();
        AchievementsManager::obtenirInstance()->AchievementUnlocked(AchievementsType(ACHIEVEMENTS_GAME_WON_L1+level),achievementName[level]);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void AchievementGameWon::FillAchievementData( XmlElement* )
///
/// /*Description*/
///
/// @param[in] XmlElement *
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void AchievementGameWon::FillAchievementData( XmlElement* elem ) const
{
    XMLUtils::writeAttribute(elem,"GameWon",mNbGameWon);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool AchievementGameWon::LoadAchievementData( const XmlElement* )
///
/// /*Description*/
///
/// @param[in] const XmlElement *
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool AchievementGameWon::LoadAchievementData( const XmlElement* elem)
{
    if(!XMLUtils::readAttribute(elem,"GameWon",mNbGameWon))
    {
        if(mLevelUnlocked < ARRAY_COUNT(NbGameWinNeeded))
        {
            mNbGameWon = NbGameWinNeeded[mLevelUnlocked];
        }
    }
    return true;
}






const int AchievementAICreation::AI_CREATION_NEEDED[3] = {3,10,20};
const std::string AchievementAICreation::AI_LEVEL_NAME[3] =
{
    "Need some friends",
    "Need more friends",
    "I'm really forever alone",
};

AchievementAICreation::AchievementAICreation(): mNbAiCreated(0)
{
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_AI_CREATED,EventAICreatedCallBack)
        );
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_AI_CREATED,EventAICreatedCallBack)
        );
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_AI_CREATED,EventAICreatedCallBack)
        );
}

void AchievementAICreation::EventAICreatedCallBack( AbstractAchievement* pAchievement, AchievementEvent pEvent )
{
    auto wThis = (AchievementAICreation*)pAchievement;
    auto val = ++wThis->mNbAiCreated;

    int level = wThis->mLevelUnlocked;
    if(level < ARRAY_COUNT(AchievementAICreation::AI_CREATION_NEEDED) && val >= AchievementAICreation::AI_CREATION_NEEDED[level])
    {
        wThis->GoToNextLevel();
        AchievementsManager::obtenirInstance()->AchievementUnlocked(AchievementsType(ACHIEVEMENTS_AI_CREATION_L1+level),AchievementAICreation::AI_LEVEL_NAME[level]);
    }
}

void AchievementAICreation::FillAchievementData( XmlElement* elem ) const
{
    XMLUtils::writeAttribute(elem,"AICreated",mNbAiCreated);
}

bool AchievementAICreation::LoadAchievementData( const XmlElement* elem )
{
    if(!XMLUtils::readAttribute(elem,"AICreated",mNbAiCreated))
    {
        if(mLevelUnlocked < ARRAY_COUNT(AI_CREATION_NEEDED))
        {
            mNbAiCreated = AI_CREATION_NEEDED[mLevelUnlocked];
        }
    }
    return true;
}





const std::string AchievementMute::MUTE_LEVEL_NAME[1] =
{
    "Stopping noise pollution"
};

AchievementMute::AchievementMute() : mMuted(false)
{
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_MUTE,EventMuteCallBack)
        );
}

void AchievementMute::EventMuteCallBack( AbstractAchievement* pAchievement, AchievementEvent pEvent )
{
    auto wThis = (AchievementMute*)pAchievement;
    wThis->mMuted = true;

    int level = wThis->mLevelUnlocked;
    if(level < 1)
    {
        wThis->GoToNextLevel();
        AchievementsManager::obtenirInstance()->AchievementUnlocked(AchievementsType(ACHIEVEMENT_EVENT_MUTE+level),AchievementMute::MUTE_LEVEL_NAME[level]);
    }
}

void AchievementMute::FillAchievementData( XmlElement* elem ) const
{
    XMLUtils::writeAttribute(elem,"MuteSound",mMuted);
}

bool AchievementMute::LoadAchievementData( const XmlElement* elem )
{
    if(!XMLUtils::readAttribute(elem,"MuteSound",mMuted))
    {
         mMuted = false;
    }
    return true;
}




const int AchievementPortal::PORTAL_NEEDED[4] = 
{
    1, 10, 20, 50
};

const std::string AchievementPortal::PORTAL_LEVEL_NAME[4] = 
{
    "Welcome to Aperture Science",
    "I suppose you want some cake?",
    "Now your thinking with portals",
    "The cake is a lie"
};

AchievementPortal::AchievementPortal() : mNbrPortalCreated(0)
{
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_PORTAL,EventPortalCallBack)
        );
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_PORTAL,EventPortalCallBack)
        );
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_PORTAL,EventPortalCallBack)
        );
    ADD_NEW_LEVEL(
        APPEND_BINDING(ACHIEVEMENT_EVENT_PORTAL,EventPortalCallBack)
        );
}

void AchievementPortal::EventPortalCallBack( AbstractAchievement* pAchievement, AchievementEvent pEvent )
{
    auto wThis = (AchievementPortal*)pAchievement;
    auto val = ++wThis->mNbrPortalCreated;

    int level = wThis->mLevelUnlocked;
    if(level < ARRAY_COUNT(AchievementPortal::PORTAL_NEEDED) && val >= AchievementPortal::PORTAL_NEEDED[level])
    {
        wThis->GoToNextLevel();
        AchievementsManager::obtenirInstance()->AchievementUnlocked(AchievementsType(ACHIEVEMENTS_PORTAL_L1+level),AchievementPortal::PORTAL_LEVEL_NAME[level]);
    }
}

void AchievementPortal::FillAchievementData( XmlElement* elem ) const
{
    XMLUtils::writeAttribute(elem,"PortalCreated",mNbrPortalCreated);
}

bool AchievementPortal::LoadAchievementData( const XmlElement* elem )
{
    if(!XMLUtils::readAttribute(elem,"PortalCreated",mNbrPortalCreated))
    {
        if(mLevelUnlocked < ARRAY_COUNT(PORTAL_NEEDED))
        {
            mNbrPortalCreated = PORTAL_NEEDED[mLevelUnlocked];
        }
    }
    return true;
}


