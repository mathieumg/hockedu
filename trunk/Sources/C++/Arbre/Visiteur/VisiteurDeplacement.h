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
/// @brief Visiteur pour les déplacement des objets
///
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
///////////////////////////////////////////////////////////////////////////
class VisiteurDeplacement :
	public VisiteurNoeud
{
public:
	/// Constructeur par paramètres
	VisiteurDeplacement(Vecteur2 deplacementVirtuel, bool ignoreSelection = false);
	/// Constructeur par paramètres
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
	/// Visitation d'un noeud accélérateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud );
    /// Visitation d'un noeud point de control
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );


private:
	/// Indique si on ignore la sélection pour faire le déplacement
	bool ignoreSelection_;
	/// Vecteur de déplacement pour les éléments
	Vecteur2 deplacement_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

