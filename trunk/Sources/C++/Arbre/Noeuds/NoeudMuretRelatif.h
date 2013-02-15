///////////////////////////////////////////////////////////////////////////
/// @file NoeudMuretRelatif.h
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "NoeudMuret.h"
#include "NoeudPoint.h"
#include "PositionSubject.h"


class NoeudBut;

///////////////////////////////////////////////////////////////////////////
/// @class NoeudMuretRelatif
/// @brief Classe qui représente un muret relatif, donc ceux aux extrimites 
/// gauche et droite de la table.
///
/// @author Samuel Ledoux
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudMuretRelatif :
	public NodeWallAbstract, public PositionObserver
{
public:
    typedef NodeWallAbstract Super;
	/// Constructeurs par paramètres
	NoeudMuretRelatif(NoeudPoint* n1, NoeudPoint* n2);
	NoeudMuretRelatif(NoeudPoint* n, NoeudBut* but, bool haut);
	NoeudMuretRelatif(NoeudBut* but, NoeudPoint* n, bool haut);

	/// Accesseur du coin1
	virtual Vecteur3 obtenirCoin1();
	/// Accesseur du coin2
	virtual Vecteur3 obtenirCoin2();

    virtual void updateObserver( class PositionSubject* pSubject );

private:
	typedef Vecteur3D<float*> Vecteur3pd;
	Vecteur3pd coin1_,coin2_;
};

