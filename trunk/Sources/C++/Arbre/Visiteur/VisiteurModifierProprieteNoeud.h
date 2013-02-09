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
	/// Constructeur par param�tre
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
	/// Visitation d'un noeud acc�l�rateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud );
	/// Visite d'un noeud "neutre"
	virtual void visiterNoeudNeutre(NoeudAbstrait* noeud);

private:
	/// Coefficient de friction de la table
	float	coefFriction_; 
	/// Coefficient de rebond des murets
	float	coefRebond_;  
	/// Coefficient d'acc�l�ration des bonus acc�l�rateurs
	float	bonusAccel_; 
	/// Position de l'�l�ment (s'applique � tout sauf au noeud de la table)
	Vecteur2 position_; 
	/// Grosseur de l'�l�ment (s'applique � tout sauf au noeud de la table)
	float echelle_;
	/// Angle de rotation de l'�l�ment (s'applique � tout sauf au noeud de la table)
	int rotation_;
	/// Indique si une seul objet doit �tre s�lectionn� pour g�rer les modifications
	bool unSeulSelect_;
	/// Les dimensions de la zone d'�dition
	float longueurZoneEdition_;
	float hauteurZoneEdition_;

	// Tableau contenant les coef de rebond pour les 8 bandes
	float coefRebondBandes_[8];


};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

