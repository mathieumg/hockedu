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
class NodeControlPoint;

class VisiteurNoeudTest;

#define VisitParent(NodeClass) noeud->##NodeClass##::Super::acceptVisitor(*this);

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

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////