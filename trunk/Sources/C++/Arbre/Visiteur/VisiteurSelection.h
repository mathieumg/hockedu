//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurSelection.h
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"

////////////////////////////////////////////////////////////////////////////
/// @class VisiteurSelection
/// @brief Visiteur pour la selection
///			S'assurer d'appeller la m�thode faireSelection() pour que la s�lection
///			s'effectue suite au passage du visiteur
/// 
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
////////////////////////////////////////////////////////////////////////////
class VisiteurSelection :
	public VisiteurNoeud
{
public:
	/// Classe amis pour faire les test
	friend VisiteurNoeudTest;

	/// Destructeur
	~VisiteurSelection(void);

	/// M�thode pour effectuer la selection sur l'objet le plus pr�s de l'�cran
	void faireSelection();

	/// Constructeur par param�tre
	VisiteurSelection(Vecteur2 positionSouris, Vecteur2 positionMax);

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
	/// Visitation d'un noeud acc�l�rateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud );
	/// Algortihme de selection des noeuds
	void visiterNoeudAffichable( NoeudAbstrait* noeud );

private:
	/// Position du premier clique de souris pour la s�lection
	Vecteur2 positionClicMin_;
	/// Position du deuxi�me clique de souris pour la s�lection
	Vecteur2 positionClicMax_;

	typedef std::pair<NoeudAbstrait*, double> NoeudZBuf;
	/// Conservation du noeud le plus pr�s de l'�cran
	NoeudZBuf aSelectionner_;

	/// Indique si la s�lection est fait par un rectangle �lastique
	bool avecRectangle;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

