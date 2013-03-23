///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategyRotate.h
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class FieldModificationStrategyRotate
/// @brief Strategy to rotate nodes
///
/// @author Michael Ferris
/// @date 2013-03-21
///////////////////////////////////////////////////////////////////////////
class FieldModificationStrategyRotate : public FieldModificationStrategyAbstract
{
public:
    FieldModificationStrategyRotate(FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS):
        FIELDMODIFICATIONSTRATEGYABSTRACT_INIT
    {
        mCenter = pEvent.mPosition;
        //findRotationCenter();
    }


protected:
    virtual int receivedEventSpecific(const FieldModificationStrategyEvent& pEvent);
    virtual int endStrategy();
    void findRotationCenter();
    Vecteur2 mCenter;
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////