///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategyAddWall.h
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class FieldModificationStrategyAddWall
/// @brief Strategy to add a wall on the field
///
/// @author Michael Ferris
/// @date 2013-03-21
///////////////////////////////////////////////////////////////////////////
class FieldModificationStrategyAddWall : public FieldModificationStrategyAbstract
{
public:
    FieldModificationStrategyAddWall(FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS);
    ~FieldModificationStrategyAddWall();
protected:
    virtual int receivedEventSpecific(const FieldModificationStrategyEvent& pEvent);
    virtual int endStrategy();
    /// Ends the strategy early and remove modifications made
    virtual int cancelStratedy() = 0;

    void createNewNode();
    class NodeWallEdition* mNewNode;
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////