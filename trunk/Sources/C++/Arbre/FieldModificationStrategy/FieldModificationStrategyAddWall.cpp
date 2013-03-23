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
FieldModificationStrategyAddWall::FieldModificationStrategyAddWall( FIELDMODIFICATIONSTRATEGYABSTRACT_PARAMETERS ) :
    FIELDMODIFICATIONSTRATEGYABSTRACT_INIT, mNewNode(NULL)
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
    Vecteur2 deplacement = pEvent.mPosition - mOldPosition;
    if(!deplacement.estNul())
    {
        VisiteurDeplacement visiteurDeplacement(deplacement);
        mField->visitSelectedNodes(visiteurDeplacement);
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyAddWall::endStrategy()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyAddWall::endStrategy()
{
    return mField->FixCollidingObjects();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyAddWall::createNewNode()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FieldModificationStrategyAddWall::createNewNode()
{

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
    if(mNewNode)
    {
        mNewNode->deleteThis();
    }
}






///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////