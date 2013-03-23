///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategieAbstract.cpp
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyRotate.h"
#include "Terrain.h"
#include "VisiteurRotation.h"
#include "BoundingBox.h"
#include "NoeudAbstrait.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn int FieldModificationStrategyRotate::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
///
/// /*Description*/
///
/// @param[in] const FieldModificationStrategyEvent & event
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyRotate::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
{
    Vecteur2 deplacement = pEvent.mPosition - mOldPosition;
    if(!deplacement.estNul())
    {
        VisiteurRotation v(deplacement[VY]*5,mCenter);
        mField->visitSelectedNodes(v);
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyRotate::endStrategy()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyRotate::endStrategy()
{
    return mField->FixCollidingObjects();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyRotate::findRotationCenter()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FieldModificationStrategyRotate::findRotationCenter()
{
    BoundingBox aabb;
    auto selectedNodes = mField->getSelectedNodes();
    for(auto it = selectedNodes.begin(); it != selectedNodes.end(); ++it)
    {
        aabb += (*it)->getPosition();
    }
    mCenter = aabb.GetCenter();
}




///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////