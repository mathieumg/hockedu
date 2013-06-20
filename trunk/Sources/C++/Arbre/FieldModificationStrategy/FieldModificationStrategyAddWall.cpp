///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategieAbstract.cpp
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyAddWall.h"
#include "Terrain.h"
#include "NodeWallEdition.h"
#include "NodeControlPoint.h"
#include "RazerGameTree.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  FieldModificationStrategyAddWall::FieldModificationStrategyAddWall( FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS )
///
/// /*Description*/
///
/// @param[in] FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
FieldModificationStrategyAddWall::FieldModificationStrategyAddWall( FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS,const std::string& type ) :
    FieldModificationStrategyAddNode(field,pEvent,type), mCurrentPoint(NULL)
{
    if(!createNextControlPoint())
    {
        if(mNewNode)
        {
            mNewNode->deleteThis();
            mNewNode = NULL;
        }
        if(mCurrentPoint)
        {
            delete mCurrentPoint;
            mCurrentPoint = NULL;
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  FieldModificationStrategyAddWall::~FieldModificationStrategyAddWall()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
FieldModificationStrategyAddWall::~FieldModificationStrategyAddWall()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn int FieldModificationStrategyAddWall::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
///
/// /*Description*/
///
/// @param[in] const FieldModificationStrategyEvent & event
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyAddWall::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
{
    if(mNewNode && mCurrentPoint)
    {
        if(!mNewNode->getParent())
        {
            // le noeud a perdu son parent,
            // le terrain a surement ete reinitialisé, ou undid
            const Vecteur2 pos = mNewNode->getPosition().convertir<2>();
            mNewNode->deleteThis();
            mNewNode = NULL;
            mCurrentPoint = NULL;
            createNewNode(pos);
            createNextControlPoint();
            if(!mNewNode || !mCurrentPoint)
            {
                return 0;
            }
        }

        mCurrentPoint->setPosition(pEvent.mPosition);
        bool validPos = mField->IsNodeAtValidEditionPosition(mNewNode,true);
        if(pEvent.mType == FIELD_MODIFICATION_EVENT_CLICK)
        {
            if(validPos)
            {
                if(!createNextControlPoint())
                {
                    mField->transferNode(mNewNode);
                    mField->pushUndoState();
                    createNewNode(pEvent.mPosition);

                    /// gestion de cas d'erreur pour un nouveau noeud ajouté
                    if(!createNextControlPoint())
                    {
                        if(mNewNode)
                        {
                            mNewNode->deleteThis();
                            mNewNode = NULL;
                        }
                        if(mCurrentPoint)
                        {
                            delete mCurrentPoint;
                            mCurrentPoint = NULL;
                        }
                    }
                }
            }
        }
        return validPos;
    }
    return 1;
}

bool FieldModificationStrategyAddWall::createNextControlPoint()
{
    ControlPointMutableAbstract* controlNode = dynamic_cast<ControlPointMutableAbstract*>(mNewNode);
    if(controlNode)
    {
        if(controlNode->getMaxControlPoints() == 0)
        {
            mNewNode->deleteThis();
            mNewNode = NULL;
            return false;
        }
        if(controlNode->getNBControlPoint() < controlNode->getMaxControlPoints())
        {
            Vecteur3 pos;
            if(mCurrentPoint)
            {
                pos = mCurrentPoint->getPosition();
            }
            else
            {
                // cas du premier control point ajouté
                pos = mNewNode->getPosition();
            }
            mCurrentPoint = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT);
            mCurrentPoint->setPosition(pos);
            bool res = mNewNode->add(mCurrentPoint);
            checkf(res);
            return res;
        }
    }
    return false;
}







///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////