///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierFactory.h
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __BONUS_MODIFIER_FACTORY_H__
#define __BONUS_MODIFIER_FACTORY_H__

class BonusModifierAbstract;
class NodeBonus;
#include "Enum_Declarations.h"
#include <string>

///////////////////////////////////////////////////////////////////////////
/// @class BonusModifierFactory
/// @brief Classe de base abstraite des usines qui seront utilis�es pour
///        cr�er les diff�rents bonus 
///
/// @author Michael Ferris
/// @date 2013-03-14
///////////////////////////////////////////////////////////////////////////
class FactoryBonusModifier
{
public:
    /// Function to overload
    virtual BonusModifierAbstract* createBonus(NodeBonus* creator) const = 0;
    virtual ~FactoryBonusModifier(){}

    static void ClearFactories();
    static inline const FactoryBonusModifier* getFactory(BonusType type)
    {
        return BonusFactories[type];
    }
private:
    

};

#define DEFAULT_BONUS_FACTORY_DECLARATION(FactoryName)      \
class Factory## FactoryName : public FactoryBonusModifier   \
{                                                           \
public:                                                     \
    virtual BonusModifierAbstract* createBonus(NodeBonus* creator) const;     \
};

#define DEFAULT_BONUS_FACTORY_IMPLEMENTATION(FactoryName)                \
    BonusModifierAbstract* Factory## FactoryName::createBonus(NodeBonus* creator) const    \
    {  BonusModifierAbstract* newModifier = new FactoryName(); newModifier->Init(creator); return newModifier; }


// don't forget to update enum and array when creating a new factory
DEFAULT_BONUS_FACTORY_DECLARATION(BonusModifierGoThroughWall);
DEFAULT_BONUS_FACTORY_DECLARATION(BonusModifierGoThroughWallMallet);
DEFAULT_BONUS_FACTORY_DECLARATION(BonusModifierBlockGoal);
DEFAULT_BONUS_FACTORY_DECLARATION(BonusModifierFan);
DEFAULT_BONUS_FACTORY_DECLARATION(BonusModifierChangeZone);

// extern pour s'assurer d'allouer la memoire a 1 seule place et une seule fois
extern const std::string BonusNamesArray[NB_BONUS_TYPE];
extern FactoryBonusModifier* BonusFactories[NB_BONUS_TYPE];

#endif // __BONUS_MODIFIER_FACTORY_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
