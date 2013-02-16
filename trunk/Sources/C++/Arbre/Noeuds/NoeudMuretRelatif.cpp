///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMuretRelatif.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NoeudMuretRelatif.h"
#include "NoeudPoint.h"
#include "NoeudBut.h"
#include "Utilitaire.h"

#ifdef MIKE_BUILD
PRAGMA_DISABLE_OPTIMIZATION
#endif

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

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuretRelatif::init( const Vecteur3& pCorner1,const Vecteur3& pCorner2,PositionSubject* s1,PositionSubject* s2 )
///
/// Initialise les attributs necessaire pour le bon fonctionnement d'un muret relatif
///
/// @param[in] Vecteur3 & pCorner1
/// @param[in] Vecteur3 & pCorner2
/// @param[in] PositionSubject * s1
/// @param[in] PositionSubject * s2
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuretRelatif::init( const Vecteur3& pCorner1, const Vecteur3& pCorner2,PositionSubject* s1,PositionSubject* s2 )
{
    coin1_[VX] = &pCorner1[VX];
    coin1_[VY] = &pCorner1[VY];
    coin1_[VZ] = &pCorner1[VZ];
    coin2_[VX] = &pCorner2[VX];
    coin2_[VY] = &pCorner2[VY];
    coin2_[VZ] = &pCorner2[VZ];

    // observe les deplacement de ces noeuds
    s1->attach(this);
    s2->attach(this);

    assignerEstSelectionnable(false);
    updateWallProperties();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::NoeudMuretRelatif( std::string type )
///
/// constructeur
///
/// @param[in] std::string type
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::NoeudMuretRelatif( const std::string& type ):
    Super(type)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuretRelatif::~NoeudMuretRelatif()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NoeudMuretRelatif::~NoeudMuretRelatif()
{

}

#ifdef MIKE_BUILD
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////