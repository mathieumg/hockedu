//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurDeplacement.h
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"

///////////////////////////////////////////////////////////////////////////
/// @class VisiteurCollision
/// @brief Visiteur pour les d�placement des objets
///
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
///////////////////////////////////////////////////////////////////////////
class VisiteurDeplacement :
	public VisiteurNoeud
{
public:
	/// Constructeur par param�tres
	VisiteurDeplacement(Vecteur2 deplacementVirtuel, bool ignoreSelection = false);
	/// Constructeur par param�tres
	VisiteurDeplacement(Vecteur3 deplacementVirtuel, bool ignoreSelection = false);
	/// Destructeur
	~VisiteurDeplacement(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Visitation d'un noeud muret
	virtual void visiterNoeudMuret( NodeWallAbstract* noeud );
	/// Visitation d'un noeud but
	virtual void visiterNoeudBut( NoeudBut* noeud );
	/// Visitation d'un noeud maillet
	virtual void visiterNoeudMaillet( NoeudMaillet* noeud );
	/// Visitation d'un noeud portail
	virtual void visiterNoeudPortail( NoeudPortail* noeud );
	/// Visitation d'un noeud rondelle
	virtual void visiterNoeudRondelle( NoeudRondelle* noeud );
	/// Visitation d'un noeud table
	virtual void visiterNoeudTable( NoeudTable* noeud );
	/// Visitation d'un noeud point
	virtual void visiterNoeudPoint( NoeudPoint* noeud );
	/// Visitation d'un noeud acc�l�rateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud );
    /// Visitation d'un noeud point de control
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );


private:
	/// Indique si on ignore la s�lection pour faire le d�placement
	bool ignoreSelection_;
	/// Vecteur de d�placement pour les �l�ments
	Vecteur2 deplacement_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

