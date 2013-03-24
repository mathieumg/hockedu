//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurDeplacement.h
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
/// @version 2.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"

///////////////////////////////////////////////////////////////////////////
/// @class VisiteurCollision
/// @brief Visiteur pour les déplacement des objets, n'effectue pas le parcours
///         de l'arbre, doit etre lance sur les noeuds que l'on veut deplacer uniquement
///
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
///////////////////////////////////////////////////////////////////////////
class VisiteurDeplacement :
	public VisiteurNoeud
{
public:
	/// Constructeur par paramètres
	VisiteurDeplacement(const Vecteur2& deplacementVirtuel);
	/// Destructeur
	virtual ~VisiteurDeplacement(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
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
	/// Visitation d'un noeud point
	virtual void visiterNoeudPoint( NoeudPoint* noeud );
	/// Visitation d'un noeud accélérateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud );
    /// Visitation d'un noeud point de control
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );


private:
	/// Vecteur de déplacement pour les éléments
	Vecteur2 deplacement_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

