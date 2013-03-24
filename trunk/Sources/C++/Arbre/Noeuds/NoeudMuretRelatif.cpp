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

#if MIKE_DEBUG_
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
const Vecteur3& NoeudMuretRelatif::obtenirCoin1() const
{
	return coins_[0] ? *coins_[0] : getPosition();
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
const Vecteur3& NoeudMuretRelatif::obtenirCoin2() const
{
    return coins_[1] ? *coins_[1] : obtenirCoin1();
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
void NoeudMuretRelatif::updateObserver( const  PositionSubject* pSubject )
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
    coins_[0] = &pCorner1;
    coins_[1] = &pCorner2;

    // observe les deplacement de ces noeuds
    s1->attach(this);
    s2->attach(this);

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
    coins_[0] = NULL;
    coins_[1] = NULL;
    setCanBeSelected(false);
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

#if MIKE_DEBUG_
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////