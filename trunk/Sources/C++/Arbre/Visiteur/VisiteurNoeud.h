//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurNoeud.h
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vecteur.h"

class NoeudAbstrait;
class NoeudComposite;
class NodeWallAbstract;
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
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Visitation d'un noeud muret
	virtual void visiterNoeudMuret( NodeWallAbstract* noeud ) {visiterNoeudComposite((NoeudComposite*)noeud);}
	/// Visitation d'un noeud but
	virtual void visiterNoeudBut( NoeudBut* noeud ) {visiterNoeudComposite((NoeudComposite*)noeud);}
	/// Visitation d'un noeud maillet
	virtual void visiterNoeudMaillet( NoeudMaillet* noeud ) {visiterNoeudAbstrait((NoeudAbstrait*)noeud);}
	/// Visitation d'un noeud portail
	virtual void visiterNoeudPortail( NoeudPortail* noeud ) {visiterNoeudAbstrait((NoeudAbstrait*)noeud);}
	/// Visitation d'un noeud rondelle
	virtual void visiterNoeudRondelle( NoeudRondelle* noeud ) {visiterNoeudAbstrait((NoeudAbstrait*)noeud);}
	/// Visitation d'un noeud table
	virtual void visiterNoeudTable( NoeudTable* noeud ) {visiterNoeudComposite((NoeudComposite*)noeud);}
	/// Visitation d'un noeud point
	virtual void visiterNoeudPoint( NoeudPoint* noeud ) {visiterNoeudComposite((NoeudComposite*)noeud);}
	/// Visitation d'un noeud accélérateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud ) {visiterNoeudAbstrait((NoeudAbstrait*)noeud);}
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////