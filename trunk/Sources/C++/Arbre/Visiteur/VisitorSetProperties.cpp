//////////////////////////////////////////////////////////////////////////////
/// @file VisitorSetProperties.cpp
/// @author Samuel Ledoux
/// @date 2012-02-17
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisitorSetProperties.h"
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
#include "VisiteurDeplacement.h"
#include "VisiteurRotation.h"
#include "VisiteurEchelle.h"

void VisitorSetProperties::SetPos(NoeudAbstrait* noeud)
{
    Vecteur2 deplacement = Vecteur2(mProperties->mPositionX,mProperties->mPositionY) - mSelectedNodesAABB.GetCenter();
    if(!deplacement.estNul())
    {
        VisiteurDeplacement v(deplacement);
        noeud->acceptVisitor(v);
    }
}

void VisitorSetProperties::SetAngle(NoeudAbstrait* noeud)
{
    float angle = mProperties->mAngle;
    if(angle != 0)
    {
        Vecteur2 center = mSelectedNodesAABB.GetCenter();
        VisiteurRotation v(angle,center);
        noeud->acceptVisitor(v);
    }
}

void VisitorSetProperties::SetScale(NoeudAbstrait* noeud, float ratio)
{
    if(ratio != 1)
    {
        VisiteurEchelle v(ratio);
        noeud->acceptVisitor(v);
    }
}




////////////////////////////////////////////////////////////////////////
///
/// @fn  VisitorSetProperties::VisitorSetProperties( FullProperties* properties )
///
/// /*Description*/
///
/// @param[in] FullProperties * properties
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisitorSetProperties::VisitorSetProperties( FullProperties* properties, const BoundingBox& selectedNodesAABB ):
    mProperties(properties),mSelectedNodesAABB(selectedNodesAABB)
{

}



////////////////////////////////////////////////////////////////////////
///
/// @fn  VisitorSetProperties::~VisitorSetProperties( void )
///
/// Destructeur desallouant le pointeur sur l'environnement JNI.
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
VisitorSetProperties::~VisitorSetProperties( void )
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Visiteur du noeud abstrait.
///
/// @param[in] NoeudAbstrait * noeud : le noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visiteur du noeud composite.
///
/// @param[in] NoeudComposite * noeud : le noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudComposite( NoeudComposite* noeud )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visiteur du noeud muret.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudMuret( NodeWallAbstract* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SetScale(noeud,mProperties->mScale/scale[VY]);
    SetAngle(noeud);
    noeud->setReboundRatio(mProperties->mRebound);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudBut( NoeudBut* noeud )
///
/// Visiteur du noeud but.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudBut( NoeudBut* noeud )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Visiteur du noeud maillet.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudMaillet( NoeudMaillet* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SetScale(noeud,mProperties->mScale/scale[VX]);
    SetAngle(noeud);
    SetPos(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Visiteur du noeud portail.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudPortail( NoeudPortail* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SetScale(noeud,mProperties->mScale/scale[VX]);
    SetAngle(noeud);
    SetPos(noeud);
    noeud->setAttractionForce(mProperties->mAttraction);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Visiteur du noeud rondelle.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudRondelle( NoeudRondelle* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SetScale(noeud,mProperties->mScale/scale[VX]);
    SetAngle(noeud);
    SetPos(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudTable( NoeudTable* noeud )
///
/// Visiteur du noeud table.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudTable( NoeudTable* noeud )
{
    noeud->modifierCoefFriction(mProperties->mFriction);
    auto field = noeud->getField();
    if(field)
    {
        auto zone = field->getZoneEdition();
        if(zone)
        {
            zone->modifierLimiteExtY(mProperties->mZoneEditionY);
            zone->modifierLimiteExtX(mProperties->mZoneEditionX);
        }
        field->setBonusesMinTimeSpawn(mProperties->mMinBonusSpawnTime);
        field->setBonusesMaxTimeSpawn(mProperties->mMaxBonusSpawnTime);
    }

    noeud->modifierCoefRebond(0,mProperties->mRinkRebound1);
    noeud->modifierCoefRebond(1,mProperties->mRinkRebound2);
    noeud->modifierCoefRebond(2,mProperties->mRinkRebound3);
    noeud->modifierCoefRebond(3,mProperties->mRinkRebound4);
    noeud->modifierCoefRebond(4,mProperties->mRinkRebound5);
    noeud->modifierCoefRebond(5,mProperties->mRinkRebound6);
    noeud->modifierCoefRebond(6,mProperties->mRinkRebound7);
    noeud->modifierCoefRebond(7,mProperties->mRinkRebound8);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visiteur du noeud point.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudPoint( NoeudPoint* noeud )
{
    SetPos(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visiteur du noeud accelerateur.
///
/// @param[in] NodeWallAbstract * noeud :  noeud a visiter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SetScale(noeud,mProperties->mScale/scale[VX]);
    SetAngle(noeud);
    SetPos(noeud);
    noeud->modifierBonusAccel(mProperties->mAcceleration);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    SetPos(noeud);
    NoeudAbstrait* n = dynamic_cast<NoeudAbstrait*>(noeud->getLinkedObject());
    if(n)
    {
        n->acceptVisitor(*this);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisitorSetProperties::visiterNodeBonus( NodeBonus* noeud )
///
/// /*Description*/
///
/// @param[in] NodeBonus * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisitorSetProperties::visiterNodeBonus( NodeBonus* noeud )
{
    Vecteur3 scale;
    noeud->getScale(scale);
    SetScale(noeud,mProperties->mScale/scale[VX]);
    SetAngle(noeud);
    SetPos(noeud);
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



