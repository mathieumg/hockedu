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

#include "Enum_Declarations.h"

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
    virtual BonusModifierAbstract* createBonus() const = 0;
    virtual ~FactoryBonusModifier(){}

    static void ClearFactories();
    static inline const FactoryBonusModifier* getFactory(BonusType type)
    {
        return mFactories[type];
    }
private:
    static FactoryBonusModifier* mFactories[NB_BONUS_TYPE];

};

#define DEFAULT_BONUS_FACTORY_DECLARATION(FactoryName)      \
class Factory## FactoryName : public FactoryBonusModifier   \
{                                                           \
public:                                                     \
    virtual BonusModifierAbstract* createBonus() const;     \
};

#define DEFAULT_BONUS_FACTORY_IMPLEMENTATION(FactoryName)                \
    BonusModifierAbstract* Factory## FactoryName::createBonus() const    \
    {  return new FactoryName();  }


// don't forget to update enum and array when creating a new factory
DEFAULT_BONUS_FACTORY_DECLARATION(BonusModifierGoThroughWall);
DEFAULT_BONUS_FACTORY_DECLARATION(BonusModifierBlockGoal);




#endif // __BONUS_MODIFIER_FACTORY_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////