///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategieAbstract.cpp
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyMove.h"
#include "Terrain.h"
#include "VisiteurDeplacement.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn int FieldModificationStrategyMove::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
///
/// /*Description*/
///
/// @param[in] const FieldModificationStrategyEvent & event
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyMove::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
{
    Vecteur2 deplacement = pEvent.mPosition - mOldPosition;
    if(!deplacement.estNul())
    {
        VisiteurDeplacement visiteurDeplacement(deplacement);
        mField->acceptVisitor(visiteurDeplacement);
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyMove::endStrategy()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyMove::endStrategy()
{
    return mField->FixCollidingObjects();
}




///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////