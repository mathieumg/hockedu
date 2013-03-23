///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategyAbstract.h
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Vecteur.h"

class Terrain;

enum FieldModificationStrategyEventType
{
    FIELD_MODIFICATION_EVENT_CLICK,
    FIELD_MODIFICATION_EVENT_MOVE,
};

enum FieldModificationStrategyType
{
    FIELD_MODIFICATION_MOVE,
    FIELD_MODIFICATION_ROTATE,
    FIELD_MODIFICATION_SCALE,
};

struct FieldModificationStrategyEvent
{
    /// position dans les coordonnées du terrain
    Vecteur2 mPosition;
    FieldModificationStrategyEventType mType;
};

#define FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS Terrain* field, const FieldModificationStrategyEvent& pEvent
#define FIELDMODIFICATIONSTRATEGYABSTRACT_INIT FieldModificationStrategyAbstract(field,pEvent)
///////////////////////////////////////////////////////////////////////////
/// @class FieldModificationStrategyAbstract
/// @brief Interface pour les strategie de modification du terrain
///
/// @author Michael Ferris
/// @date 2013-03-21
///////////////////////////////////////////////////////////////////////////
class FieldModificationStrategyAbstract
{
public:
    /// Initialise the strategie with basic information
    FieldModificationStrategyAbstract(Terrain* field, const FieldModificationStrategyEvent& pEvent)
    {
        mOldPosition = pEvent.mPosition;
        mField = field;

        checkf(pEvent.mType == FIELD_MODIFICATION_EVENT_CLICK, "Not starting a strategy from a click event");
        checkf(mField);
    }
    /// Entry point when an event is sent to this strategy, return not used
    int receivedEvent(const FieldModificationStrategyEvent& pEvent)
    {
        checkf(mField);
        const int a = receivedEventSpecific(pEvent);
        mOldPosition = pEvent.mPosition;
        return a;
    }
    /// Specific behavior when strategy is over
    virtual int endStrategy() = 0;
    /// Ends the strategy early and remove modifications made
    virtual int cancelStratedy() = 0;
protected:
    virtual int receivedEventSpecific(const FieldModificationStrategyEvent& pEvent) = 0;

    Vecteur2 mOldPosition;
    Terrain* mField;
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////