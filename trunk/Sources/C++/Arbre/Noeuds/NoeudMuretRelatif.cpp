///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMuretRelatif.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NoeudMuretRelatif.h"
#include "NoeudPoint.h"
#include "NoeudBut.h"
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n1, NoeudPoint* n2 )
///
/// Constructeur par paramètre.
///
/// @param[in] NoeudPoint * n1 : premier point associé
/// @param[in] NoeudPoint * n2 : deuxieme point associé
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n1, NoeudPoint* n2 ):
Super(RazerGameUtilities::NOM_MURET_RELATIF)
{
	coin1_[VX] = &n1->positionRelative_[VX];
	coin1_[VY] = &n1->positionRelative_[VY];
	coin1_[VZ] = &n1->positionRelative_[VZ];
	coin2_[VX] = &n2->positionRelative_[VX];
	coin2_[VY] = &n2->positionRelative_[VY];
	coin2_[VZ] = &n2->positionRelative_[VZ];

    // observe les deplacement de ces noeuds
    n1->attach(this);
    n2->attach(this);

	assignerEstSelectionnable(false);
	assignerEstEnregistrable(false);
	updateWallProperties();
	assignerAffiche(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n, NoeudBut* but, bool haut )
///
/// constructeur par paramètre
///
/// @param[in] NoeudPoint * n : point relié
/// @param[in] NoeudBut * but : But relié
/// @param[in] bool haut : indique si on ce lie à la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::NoeudMuretRelatif( NoeudPoint* n, NoeudBut* but, bool haut ):
Super(RazerGameUtilities::NOM_MURET_RELATIF)
{
	coin1_[VX] = &n->positionRelative_[VX];
	coin1_[VY] = &n->positionRelative_[VY];
	coin1_[VZ] = &n->positionRelative_[VZ];
	if(haut)
	{
		coin2_[VX] = &but->mTopPosition[VX];
		coin2_[VY] = &but->mTopPosition[VY];
		coin2_[VZ] = &but->mTopPosition[VZ];
	}
	else
	{
		coin2_[VX] = &but->mBottomPosition[VX];
		coin2_[VY] = &but->mBottomPosition[VY];
		coin2_[VZ] = &but->mBottomPosition[VZ];
	}

    // observe les deplacement de ces noeuds
    n->attach(this);
    but->attach(this);

	assignerEstSelectionnable(false);
	assignerEstEnregistrable(false);
	updateWallProperties();
	assignerAffiche(false);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::NoeudMuretRelatif( NoeudBut* but, NoeudPoint* n, bool haut )
///
/// constructeur par paramètre
///
/// @param[in] NoeudPoint * n : point relié
/// @param[in] NoeudBut * but : But relié
/// @param[in] bool haut : indique si on ce lie à la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::NoeudMuretRelatif( NoeudBut* but, NoeudPoint* n, bool haut ):
Super(RazerGameUtilities::NOM_MURET_RELATIF)
{
	coin2_[VX] = &n->positionRelative_[VX];
	coin2_[VY] = &n->positionRelative_[VY];
	coin2_[VZ] = &n->positionRelative_[VZ];
	if(haut)
	{
		coin1_[VX] = &but->mTopPosition[VX];
		coin1_[VY] = &but->mTopPosition[VY];
		coin1_[VZ] = &but->mTopPosition[VZ];
	}
	else
	{
		coin1_[VX] = &but->mBottomPosition[VX];
		coin1_[VY] = &but->mBottomPosition[VY];
		coin1_[VZ] = &but->mBottomPosition[VZ];
	}

    // observe les deplacement de ces noeuds
    n->attach(this);
    but->attach(this);

	assignerEstSelectionnable(false);
	assignerEstEnregistrable(false);
	updateWallProperties();
	assignerAffiche(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudMuretRelatif::obtenirCoin1()
///
/// Accesseur du coin1.
///
///
/// @return Vecteur3 : la position du coin1.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuretRelatif::obtenirCoin1()
{
	return Vecteur3(*coin1_[VX],*coin1_[VY],0);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudMuretRelatif::obtenirCoin2()
///
/// Accesseur du coin2.
///
///
/// @return Vecteur3 : la position du coin2.
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudMuretRelatif::obtenirCoin2()
{
	return Vecteur3(*coin2_[VX],*coin2_[VY],0);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::updatePosition( class PositionSubject& pSubject )
///
/// /*Description*/
///
/// @param[in] class PositionSubject & pSubject
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
void NoeudMuretRelatif::updateObserver( PositionSubject* pSubject )
{
    updateWallProperties();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////