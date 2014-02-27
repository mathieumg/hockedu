///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategyAddNode.h
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "FieldModificationStrategyAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class FieldModificationStrategyAddNode
/// @brief Strategy to add a wall on the field
///
/// @author Michael Ferris
/// @date 2013-03-21
///////////////////////////////////////////////////////////////////////////
class FieldModificationStrategyAddNode : public FieldModificationStrategyAbstract
{
public:
    FieldModificationStrategyAddNode( FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS, const RazerKey );
    virtual ~FieldModificationStrategyAddNode();
protected:
    virtual int receivedEventSpecific(const FieldModificationStrategyEvent& pEvent);
    virtual int endStrategy();
    /// Ends the strategy early and remove modifications made
    virtual int cancelStratedy();

    void createNewNode(const Vecteur2& position);
    class NoeudAbstrait* mNewNode;
    const RazerKey mNodeKey;

};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////