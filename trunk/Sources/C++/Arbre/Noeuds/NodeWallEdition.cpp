///////////////////////////////////////////////////////////////////////////////
/// @file NodeWallEdition.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NodeWallEdition.h"
#include "Utilitaire.h"
#include "NodeControlPoint.h"

#ifdef MIKE_BUILD
PRAGMA_DISABLE_OPTIMIZATION
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeWallEdition::NodeWallEdition( NoeudPoint* n1, NoeudPoint* n2 )
///
/// Constructeur par paramètre.
///
/// @param[in] NoeudPoint * n1 : premier point associé
/// @param[in] NoeudPoint * n2 : deuxieme point associé
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeWallEdition::NodeWallEdition( const std::string& type ):
    Super(type)
{
    for(int i=0; i<NB_CONTROL_POINTS; ++i)
    {
        mPoints[i] = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT,this);
        ajouter(mPoints[i]);
    }

    for(int i=0; i<NB_CONTROL_POINTS; ++i)
    {
        for(int j=0; j<NB_CONTROL_POINTS; ++j)
        {
            if(i != j)
            {
                mPoints[i]->addAssociatedPoint(mPoints[j]);
            }
        }
        mPoints[i]->assignerEstEnregistrable(false);
    }

    init(
        mPoints[0]->obtenirPositionRelative(),
        mPoints[1]->obtenirPositionRelative(),
        mPoints[0],
        mPoints[1]
        );

    
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeWallEdition::~NodeWallEdition()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeWallEdition::~NodeWallEdition()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::updateObserver( PositionSubject* pSubject )
///
/// /*Description*/
///
/// @param[in] PositionSubject * pSubject
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::updateObserver( PositionSubject* pSubject )
{
    // wall properties updated
    Super::updateObserver(pSubject);

//     // recalculates control points position from our new position
//     for(int i=0; i<NB_CONTROL_POINTS; ++i)
//     {
//         mPoints[i]->updateRelativePosition();
//     }
//     updateWallProperties();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::assignerParent( NoeudComposite* parent )
///
/// /*Description*/
///
/// @param[in] NoeudComposite * parent
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::assignerParent( NoeudComposite* parent )
{
    Super::assignerParent(parent);
}

#ifdef MIKE_BUILD
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////