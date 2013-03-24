//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurRotation.h
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"

////////////////////////////////////////////////////////////////////////////
/// @class VisiteurRotation
/// @brief Visiteur pour la rotation des noeuds
/// 
/// @author Mathieu Parent
/// @date 2012-02-06
////////////////////////////////////////////////////////////////////////////
class VisiteurRotation :
	public VisiteurNoeud
{
public:
	/// Constructeur par paramètre
	VisiteurRotation(float angle, const Vecteur2& centreRot);
	/// Destrcuteur
	virtual ~VisiteurRotation(void);

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
    virtual void visiterNoeudMuretEdition( NodeWallEdition* noeud );
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );

    /// Applies the rotation algorithm
    void rotateNode( NoeudAbstrait* noeud );



private:
	/// Angle de rotation des éléments
	int angleRot_;
	/// Conserve le centre de rotation 
	Vecteur2 centreRot_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

