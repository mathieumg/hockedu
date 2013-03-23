//////////////////////////////////////////////////////////////////////////////
/// @file VisitorSetProperties.h
/// @author Michael Ferris
/// @date 2013-03-19
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"
#include "Enum_Declarations.h"
#include "Flags.h"
#include "VisitorGatherProperties.h"
#include "BoundingBox.h"

///////////////////////////////////////////////////////////////////////////
/// @class VisitorSetProperties
/// @brief Visiteur pour assigner les proprietes des noeuds,
///         ce visiteur ignore la selection des noeuds et
///         ne visite pas le noeuds enfants, il faut donc
///         l'envoyer manuellement sur les noeuds selectionné!!!
///
/// @author Michael Ferris
/// @date 2013-03-19
///////////////////////////////////////////////////////////////////////////
class VisitorSetProperties :
	public VisiteurNoeud
{
public:
	/// Destructeur
    VisitorSetProperties(FullProperties* properties, const BoundingBox& selectedNodesAABB);
    ~VisitorSetProperties(void);

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
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );
    virtual void visiterNodeBonus( NodeBonus* noeud );
private:
    void SetPos(NoeudAbstrait* noeud);
    void SetAngle(NoeudAbstrait* noeud);
    void SetScale(NoeudAbstrait* noeud, float ratio);

    /// Bounding box des noeuds selectionné calculé au préalable
    const BoundingBox& mSelectedNodesAABB;
    FullProperties* mProperties;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

