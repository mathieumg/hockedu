///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategyAddWall.h
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "FieldModificationStrategyAddNode.h"

///////////////////////////////////////////////////////////////////////////
/// @class FieldModificationStrategyAddWall
/// @brief Strategy to add a wall on the field
///
/// @author Michael Ferris
/// @date 2013-03-21
///////////////////////////////////////////////////////////////////////////
class FieldModificationStrategyAddWall : public FieldModificationStrategyAddNode
{
public:
    FieldModificationStrategyAddWall(FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS,const std::string& type);
    ~FieldModificationStrategyAddWall();
protected:
    virtual int receivedEventSpecific(const FieldModificationStrategyEvent& pEvent);
    bool createNextControlPoint();
    class NodeControlPoint* mCurrentPoint;
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////