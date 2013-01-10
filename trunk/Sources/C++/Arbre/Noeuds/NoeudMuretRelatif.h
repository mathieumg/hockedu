///////////////////////////////////////////////////////////////////////////
/// @file NoeudMuretRelatif.h
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "NoeudMuret.h"
#include "NoeudPoint.h"
#include "PositionObserver.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudMuretRelatif
/// @brief Classe qui représente un muret relatif, donc ceux aux extrimites 
/// gauche et droite de la table.
///
/// @author Samuel Ledoux
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudMuretRelatif :
	public NoeudMuret, public PositionObserver
{
public:
	/// Constructeurs par paramètres
	NoeudMuretRelatif(NoeudPoint* n1, NoeudPoint* n2		);
	NoeudMuretRelatif(NoeudPoint* n, NoeudBut* but, bool haut);
	NoeudMuretRelatif(NoeudBut* but, NoeudPoint* n, bool haut);
	/// Destructeur
	~NoeudMuretRelatif(void);

	/// Affiche le muret.
	virtual void afficherConcret() const;
	/// Accesseur du coin1
	virtual Vecteur3 obtenirCoin1();
	/// Accesseur du coin2
	virtual Vecteur3 obtenirCoin2();
	/// Effectue l'animation
	virtual void animer( const float& temps );
	/// Retourne le rayon du noeud
	virtual double obtenirRayon() const;
	/// Initialise les deux valeurs pour l'utilisation d'un modele 3D
	virtual void mettreAJourEchelleRotation();

    virtual void updatePosition( class PositionSubject& pSubject );

private:
	typedef Vecteur3D<double*> Vecteur3pd;
	Vecteur3pd coin1_,coin2_;

};

