//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurCollision.h
/// @author Samuel Ledoux, Charles Étienne Lalonde
/// @date 2012-02-13
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "VisiteurNoeud.h"

class RazerGameTree;

///////////////////////////////////////////////////////////////////////////
/// @class VisiteurDupliquer
/// @brief Visiteur pour la duplication des objets
///
/// @author Samuel Ledoux, Charles Étienne Lalonde
/// @date 2012-02-13
///////////////////////////////////////////////////////////////////////////
class VisiteurDupliquer: public VisiteurNoeud
{
public:
	/// Constructeur par paramètre
	VisiteurDupliquer(RazerGameTree* arbre);
	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Visitation d'un noeud muret
	virtual void visiterNoeudMuret( NoeudMuret* noeud );
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
	/// Algortihme de duplication d'un noeud
	void dupliquerNoeud(NoeudAbstrait* noeud);
	/// Visitation des enfants d'un noeud composite
	void visiterEnfants(NoeudComposite* noeud);

private:
	/// Racine de l'arbre à visiter
	RazerGameTree* arbre_;
};