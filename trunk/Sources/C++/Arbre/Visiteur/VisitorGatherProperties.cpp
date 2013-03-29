//////////////////////////////////////////////////////////////////////////////
/// @file VisitorGatherProperties.cpp
/// @author Samuel Ledoux
/// @date 2012-02-17
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisitorGatherProperties.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "NoeudPortail.h"
#include "ZoneEdition.h"
#include "Terrain.h"
#include "Utilitaire.h"
#include "NodeControlPoint.h"
#include "ControlPointMutableAbstract.h"
#include "NodeBonus.h"

#define SET_PROPERTY(Flag,src,dest)                                          \
    if(!mFlags.IsFlagSet(INVALID_##Flag))                                 \
    {                                                                     \
        if(mFlags.IsFlagSet(ASSIGNED_##Flag) && src != dest)              \
        {                                                                 \
            mFlags.SetFlag(true,INVALID_##Flag);                          \
        }                                                                 \
        else                                                              \
        {                                                                 \
            dest = src;                                                   \
            mFlags.SetFlag(true,ASSIGNED_##Flag);                         \
        }                                                                 \
        mProperties->mPropertyFlagAssignment = mFlags.mValue;         \
    }

void VisitorGatherProperties::GetPos(NoeudAbstrait* noeud)
{
    auto pos = noeud->getPosition();

    // Moyenne des positions
    if(mFlags.IsFlagSet(ASSIGNED_POSITIONX))
    {
        mProperties->mPositionX += pos[VX];
        mProperties->mPositionX /= 2.0f;
    }
    else
    {
        mProperties->mPositionX = pos[VX];
    }
    if(mFlags.IsFlagSet(ASSIGNED_POSITIONY))
    {
        mProperties->mPositionY += pos[VY];
        mProperties->mPositionY /= 2.0f;
    }
    else
    {
        mProperties->mPositionY = pos[VY];
    }
    mFlags.SetFlag(true,ASSIGNED_POSITIONX);
    mFlags.SetFlag(true,ASSIGNED_POSITIONY);
    mProperties->mPropertyFlagAssignment = mFlags.mValue;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  VisitorGatherProperties::VisitorGatherProperties( FullProperties* properties )
///
/// /*Description*/
///
/// @param[in] FullProperties * properties
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisitorGatherProperties::VisitorGatherProperties( FullProperties* properties ):
    mProperties(properties),mFlags(0)
{

}



////////////////////////////////////////////////////////////////////////
///
/// @fn  VisitorGatherProperties::~VisitorGatherProperties( void )
///
/// Destructeur desallouant le pointeur sur l'environnement JNI.
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
VisitorGatherProperties::~VisitorGatherProperties( void )
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Visiteur du noeud abstrait.
///
/// @param[in] NoeudAbstrait * noeud : le noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visiteur du noeud composite.
///
/// @param[in] NoeudComposite * noeud : le noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudComposite( NoeudComposite* noeud )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visiteur du noeud muret.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudMuret( NodeWallAbstract* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    float angle = noeud->getAngle();
    float rebound = noeud->getReboundRatio();

    SET_PROPERTY(SCALE,scale[VY],mProperties->mScale);
    SET_PROPERTY(ANGLE,angle,mProperties->mAngle);
    SET_PROPERTY(REBOUND,rebound,mProperties->mRebound);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudBut( NoeudBut* noeud )
///
/// Visiteur du noeud but.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudBut( NoeudBut* noeud )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Visiteur du noeud maillet.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudMaillet( NoeudMaillet* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SET_PROPERTY(SCALE,scale[VX],mProperties->mScale);
    GetPos(noeud);

    float angle = noeud->getAngle();
    SET_PROPERTY(ANGLE,angle,mProperties->mAngle);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Visiteur du noeud portail.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudPortail( NoeudPortail* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SET_PROPERTY(SCALE,scale[VX],mProperties->mScale);
    GetPos(noeud);
    auto force = noeud->getAttractionForce();
    SET_PROPERTY(ATTRACTION,force,mProperties->mAttraction);

    float angle = noeud->getAngle();
    SET_PROPERTY(ANGLE,angle,mProperties->mAngle);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Visiteur du noeud rondelle.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudRondelle( NoeudRondelle* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SET_PROPERTY(SCALE,scale[VX],mProperties->mScale);
    GetPos(noeud);

    float angle = noeud->getAngle();
    SET_PROPERTY(ANGLE,angle,mProperties->mAngle);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudTable( NoeudTable* noeud )
///
/// Visiteur du noeud table.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudTable( NoeudTable* noeud )
{
    auto friction = noeud->obtenirCoefFriction();
    SET_PROPERTY(FRICTION,friction,mProperties->mFriction);
    auto field = noeud->getField();
    if(field)
    {
        auto zone = field->getZoneEdition();
        if(zone)
        {
            auto y = zone->obtenirLimiteExtY();
            auto x = zone->obtenirLimiteExtX();
            SET_PROPERTY(ZONE_X,x,mProperties->mZoneEditionX);
            SET_PROPERTY(ZONE_Y,y,mProperties->mZoneEditionY);
        }
        // todo:::
        SET_PROPERTY(BONUS_MIN,0,mProperties->mMinBonusSpawnTime);
        SET_PROPERTY(BONUS_MAX,0,mProperties->mMaxBonusSpawnTime);
    }
    mProperties->mRinkRebound1 = noeud->obtenirCoefRebond(0);
    mProperties->mRinkRebound2 = noeud->obtenirCoefRebond(1);
    mProperties->mRinkRebound3 = noeud->obtenirCoefRebond(2);
    mProperties->mRinkRebound4 = noeud->obtenirCoefRebond(3);
    mProperties->mRinkRebound5 = noeud->obtenirCoefRebond(4);
    mProperties->mRinkRebound6 = noeud->obtenirCoefRebond(5);
    mProperties->mRinkRebound7 = noeud->obtenirCoefRebond(6);
    mProperties->mRinkRebound8 = noeud->obtenirCoefRebond(7);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visiteur du noeud point.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudPoint( NoeudPoint* noeud )
{
    GetPos(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visiteur du noeud accelerateur.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SET_PROPERTY(SCALE,scale[VX],mProperties->mScale);
    GetPos(noeud);
    auto accel = noeud->obtenirBonusAccel();
    SET_PROPERTY(ACCELERATION,accel,mProperties->mAcceleration);

    float angle = noeud->getAngle();
    SET_PROPERTY(ANGLE,angle,mProperties->mAngle);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    GetPos(noeud);
    NoeudAbstrait* n = dynamic_cast<NoeudAbstrait*>(noeud->getLinkedObject());
    if(n)
    {
        n->acceptVisitor(*this);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorGatherProperties::visiterNodeBonus( NodeBonus* noeud )
///
/// /*Description*/
///
/// @param[in] NodeBonus * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorGatherProperties::visiterNodeBonus( NodeBonus* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SET_PROPERTY(SCALE,scale[VY],mProperties->mScale);
    GetPos(noeud);

    float angle = noeud->getAngle();
    SET_PROPERTY(ANGLE,angle,mProperties->mAngle);
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



