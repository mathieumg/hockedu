///////////////////////////////////////////////////////////////////////////
/// @file NoeudMuretRelatif.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "PositionSubject.h"
#include "NodeWallAbstract.h"


///////////////////////////////////////////////////////////////////////////
/// @class NoeudMuretRelatif
/// @brief Classe qui représente un muret relatif, donc ceux aux extrimites 
/// gauche et droite de la table.
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NoeudMuretRelatif :
	public NodeWallAbstract, public PositionObserver
{
public:
    typedef NodeWallAbstract Super;

    NoeudMuretRelatif(const std::string& type);
    virtual ~NoeudMuretRelatif() = 0;

	/// Accesseur du coin1
	virtual Vecteur3 obtenirCoin1();
	/// Accesseur du coin2
	virtual Vecteur3 obtenirCoin2();

    virtual void updateObserver( PositionSubject* pSubject );

protected:
    void init( const Vecteur3& pCorner1, const Vecteur3& pCorner2,PositionSubject* s1,PositionSubject* s2);
private:
	typedef Vecteur3D<const float*> Vecteur3pd;
	Vecteur3pd coin1_,coin2_;
};

