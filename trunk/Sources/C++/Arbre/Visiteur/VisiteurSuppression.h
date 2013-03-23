//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurSuppression.h
/// @author Michael Ferris
/// @date 2012-02-11
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "VisiteurNoeud.h"

////////////////////////////////////////////////////////////////////////////
/// @class VisiteurSuppression
/// @brief Visiteur pour la suppression de noeud sélectionné
/// 
/// @author Michael Ferris
/// @date 2012-02-11
////////////////////////////////////////////////////////////////////////////
class VisiteurSuppression :
	public VisiteurNoeud
{
public:
	/// Constructeur par défaut
	VisiteurSuppression(void);
	/// Destructeur
	virtual ~VisiteurSuppression(void);

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
    /// Visitation d'un noeud muret d'edition ( avec point de controle)
    virtual void visiterNoeudMuretEdition( NodeWallEdition* noeud );

	/// Algorithme de suppression des noeuds
	bool effacerSiSelectionne(NoeudAbstrait* noeud);

	/// Algorithme de visitation des enfants
	void visiterEnfants(NoeudComposite* noeud);
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );

};

