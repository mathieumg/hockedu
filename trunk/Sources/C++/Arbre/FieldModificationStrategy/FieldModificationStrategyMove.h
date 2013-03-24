///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategyMove.h
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class FieldModificationStrategyMove
/// @brief Strategy to move a node
///
/// @author Michael Ferris
/// @date 2013-03-21
///////////////////////////////////////////////////////////////////////////
class FieldModificationStrategyMove : public FieldModificationStrategyAbstract
{
public:
    FieldModificationStrategyMove(FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS):
        FIELDMODIFICATIONSTRATEGYABSTRACT_INIT
    {

    }
protected:
    virtual int receivedEventSpecific(const FieldModificationStrategyEvent& pEvent);
    virtual int endStrategy();
    /// Ends the strategy early and remove modifications made
    virtual int cancelStratedy();
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////