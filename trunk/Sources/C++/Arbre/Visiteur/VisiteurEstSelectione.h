//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurEstSelectione.h
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"
#include "INF2990TypeDef.h"

////////////////////////////////////////////////////////////////////////////
/// @class VisiteurEstSelectione
/// @brief Visiteur pour connaitre quels noeuds sont selectionne dans l'arbre
///			S'assurer d'utiliser la liste de selection dans le meme scope que le visiteur
///			Sinon il faut en faire une copie avant de le renvoye
///			
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
////////////////////////////////////////////////////////////////////////////
class VisiteurEstSelectione :
	public VisiteurNoeud
{
public:
	/// Classe amis pour faire les test
	friend VisiteurNoeudTest;

	/// Destructeur
	~VisiteurEstSelectione(void);


	/// Constructeur par paramêtre
	VisiteurEstSelectione();

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
	/// Accesseur de listeNoeuds_
	ConteneurNoeuds* obtenirListeNoeuds() const;

private:
	/// L'ensemble des noeuds sélectionnés
	ConteneurNoeuds* listeNoeuds_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

