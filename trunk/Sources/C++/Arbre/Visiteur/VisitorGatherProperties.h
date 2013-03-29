//////////////////////////////////////////////////////////////////////////////
/// @file VisitorGatherProperties.h
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

class BonusProperties
{
public:
    char* mName;
    bool mEnabled;
    float mDuration;
    int mType;
};

class FullProperties
{
public:
    float mFriction;
    float mZoneEditionX;
    float mZoneEditionY;
    float mScale;
    float mAcceleration;
    float mPositionX;
    float mPositionY;
    float mAttraction;
    float mAngle;
    float mRebound;
    float mMinBonusSpawnTime;
    float mMaxBonusSpawnTime;
    float mRinkRebound1;
    float mRinkRebound2;
    float mRinkRebound3;
    float mRinkRebound4;
    float mRinkRebound5;
    float mRinkRebound6;
    float mRinkRebound7;
    float mRinkRebound8;
    //BonusProperties* mBonusProperties;
    int mPropertyFlagAssignment;
};


///////////////////////////////////////////////////////////////////////////
/// @class VisitorGatherProperties
/// @brief Visiteur pour recuperer les proprietes des noeuds,
///         ce visiteur ignore la selection des noeuds et
///         ne visite pas le noeuds enfants, il faut donc
///         l'envoyer manuellement sur les noeuds selectionné!!!
///
/// @author Michael Ferris
/// @date 2013-03-19
///////////////////////////////////////////////////////////////////////////
class VisitorGatherProperties :
	public VisiteurNoeud
{
public:
	/// Destructeur
    VisitorGatherProperties(FullProperties* properties);
    virtual ~VisitorGatherProperties(void);

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
    void GetPos(NoeudAbstrait* noeud);
    FullProperties* mProperties;
    Flags<int,NB_PROPERTYASSIGNMENTVALIDATION> mFlags;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

