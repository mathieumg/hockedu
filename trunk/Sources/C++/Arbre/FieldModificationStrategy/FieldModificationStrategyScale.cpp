///////////////////////////////////////////////////////////////////////////////
/// @file FieldModificationStrategieAbstract.cpp
/// @author Michael Ferris
/// @date 2013-03-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "FieldModificationStrategyScale.h"
#include "Terrain.h"
#include "VisiteurEchelle.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn int FieldModificationStrategyScale::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
///
/// /*Description*/
///
/// @param[in] const FieldModificationStrategyEvent & event
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyScale::receivedEventSpecific( const FieldModificationStrategyEvent& pEvent )
{
    Vecteur2 deplacement = pEvent.mPosition - mOldPosition;
    if(deplacement[VY] != 0)
    {
        float facteur = 0.95f;
        if(deplacement[VY] > 0)
        {
            facteur = 1.05f;
        }
        VisiteurEchelle v(facteur);
        mField->visitSelectedNodes(v);
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FieldModificationStrategyScale::endStrategy()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int FieldModificationStrategyScale::endStrategy()
{
    return mField->FixCollidingObjects();
}




///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////