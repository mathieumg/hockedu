///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategyScale.h
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class FieldModificationStrategyScale
/// @brief Strategy to scale nodes
///
/// @author Michael Ferris
/// @date 2013-03-21
///////////////////////////////////////////////////////////////////////////
class FieldModificationStrategyScale : public FieldModificationStrategyAbstract
{
public:
    FieldModificationStrategyScale(FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS):
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