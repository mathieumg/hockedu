///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategieAbstract.cpp
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyAddNode.h"
#include "Terrain.h"
#include "NodeWallEdition.h"
#include "RazerGameTree.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  FieldModificationStrategyAddNode::FieldModificationStrategyAddNode( FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS )
///
/// /*Description*/
///
/// @param[in] FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
FieldModificationStrategyAddNode::FieldModificationStrategyAddNode( FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS, const std::string& type ) :
    FIELDMODIFICATIONSTRATEGYABSTRACT_INIT, mNewNode(NULL), mType(type)
{
    createNewNode(pEvent.mPosition);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  FieldModificationStrategyAddNode::~FieldModificationStrategyAddNode()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
FieldModificationStrategyAddNode::~FieldModificationStrategyAddNode()
{
    if(mNewNode)
    {
        mNewNode->deleteThis();
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn int FieldModificationStrategyAddNode::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
///
/// /*Description*/
///
/// @param[in] const FieldModificationStrategyEvent & event
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyAddNode::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
{
    if(mNewNode)
    {
        if(!mNewNode->getParent())
        {
            // le noeud a perdu son parent,
            // le terrain a surement ete reinitialis�, ou undid
            mField->ajouterNoeudTemp(mNewNode);
        }
        mNewNode->setPosition(pEvent.mPosition);
        bool validPos = mField->IsNodeAtValidEditionPosition(mNewNode,true);
        if(pEvent.mType == FIELD_MODIFICATION_EVENT_CLICK)
        {
            if(validPos)
            {
                mField->transfererNoeud(mNewNode);
                mField->pushUndoState();
                createNewNode(pEvent.mPosition);
            }
        }
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyAddNode::endStrategy()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyAddNode::endStrategy()
{
    return mField->FixCollidingObjects();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyAddNode::createNewNode()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FieldModificationStrategyAddNode::createNewNode(const Vecteur2& position)
{
    auto tree = mField->getLogicTree();
    if(tree)
    {
        mNewNode = tree->creerNoeud(mType);
        if(mNewNode)
        {
            mNewNode->setPosition(position);
            mField->ajouterNoeudTemp(mNewNode);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn int FieldModificationStrategyAddNode::cancelStratedy()
///
/// Ends the strategy early and remove modifications made
///
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyAddNode::cancelStratedy()
{
    return mField->FixCollidingObjects();
}






///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////