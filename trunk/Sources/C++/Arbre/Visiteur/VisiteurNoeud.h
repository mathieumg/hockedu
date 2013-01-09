//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurNoeud.h
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vecteur.h"

class NoeudAbstrait;
class NoeudComposite;
class NoeudMuret;
class NoeudBut;
class NoeudMaillet;
class NoeudPortail;
class NoeudRondelle;
class NoeudTable;
class NoeudPoint;
class NoeudAccelerateur;

class VisiteurNoeudTest;

////////////////////////////////////////////////////////////////////////////
/// @class VisiteurNoeud
/// @brief Interface des visiteur pour l'arbre de rendu
/// 
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
////////////////////////////////////////////////////////////////////////////
class VisiteurNoeud
{
public:

	/// Constructeur de base
	VisiteurNoeud(void);
	/// Destructeur virtuel pure
	virtual ~VisiteurNoeud(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud ) = 0;
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud ) = 0;
	/// Visitation d'un noeud muret
	virtual void visiterNoeudMuret( NoeudMuret* noeud ) = 0;
	/// Visitation d'un noeud but
	virtual void visiterNoeudBut( NoeudBut* noeud ) = 0;
	/// Visitation d'un noeud maillet
	virtual void visiterNoeudMaillet( NoeudMaillet* noeud ) = 0;
	/// Visitation d'un noeud portail
	virtual void visiterNoeudPortail( NoeudPortail* noeud ) = 0;
	/// Visitation d'un noeud rondelle
	virtual void visiterNoeudRondelle( NoeudRondelle* noeud ) = 0;
	/// Visitation d'un noeud table
	virtual void visiterNoeudTable( NoeudTable* noeud ) = 0;
	/// Visitation d'un noeud point
	virtual void visiterNoeudPoint( NoeudPoint* noeud ) = 0;
	/// Visitation d'un noeud accélérateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////