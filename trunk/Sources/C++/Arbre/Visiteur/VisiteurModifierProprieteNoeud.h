//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurModifierProprieteNoeud.h
/// @author Samuel Ledoux
/// @date 2012-02-17
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"
#include <jni.h>


enum Panneaux{POSITION, ROTATION, ECHELLE, REBOND, ACCELERATION, FRICTION, NBPANNEAUX};

///////////////////////////////////////////////////////////////////////////
/// @class VisiteurModifierProprieteNoeud
/// @brief Visiteur pour assigner les proprietes au noeud voulu
///
/// @author Samuel Ledoux
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class VisiteurModifierProprieteNoeud :
	public VisiteurNoeud
{
public:
	/// Constructeur par paramètre
	VisiteurModifierProprieteNoeud(JNIEnv* env, jobject& modificateur);
	/// Destructeur
	~VisiteurModifierProprieteNoeud(void);

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
	/// Visite d'un noeud "neutre"
	virtual void visiterNoeudNeutre(NoeudAbstrait* noeud);

private:
	/// Coefficient de friction de la table
	float	coefFriction_; 
	/// Coefficient de rebond des murets
	float	coefRebond_;  
	/// Coefficient d'accélération des bonus accélérateurs
	float	bonusAccel_; 
	/// Position de l'élément (s'applique à tout sauf au noeud de la table)
	Vecteur2 position_; 
	/// Grosseur de l'élément (s'applique à tout sauf au noeud de la table)
	float echelle_;
	/// Angle de rotation de l'élément (s'applique à tout sauf au noeud de la table)
	int rotation_;
	/// Indique si une seul objet doit être sélectionné pour gérer les modifications
	bool unSeulSelect_;
	/// Les dimensions de la zone d'édition
	float longueurZoneEdition_;
	float hauteurZoneEdition_;

	// Tableau contenant les coef de rebond pour les 8 bandes
	float coefRebondBandes_[8];


};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

