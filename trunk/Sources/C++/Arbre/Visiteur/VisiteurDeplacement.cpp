//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurDeplacement.cpp
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
/// @version 2.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurDeplacement.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "NodeControlPoint.h"
#include "Terrain.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::VisiteurDeplacement()
///
///
/// @param[in]  deplacementVirtuel  : deplacement a effectuer en coord virtuelles
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurDeplacement::VisiteurDeplacement( const Vecteur2& deplacementVirtuel):
    deplacement_(deplacementVirtuel)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::~VisiteurDeplacement()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurDeplacement::~VisiteurDeplacement(void)
{
    
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Déplacement d'un NoeudAbstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
    Vecteur3 pos = noeud->getPosition() + deplacement_;
    auto field = noeud->getField();
    if(field)
    {
        auto zone = field->getZoneEdition();
        if(zone)
        {
            auto aabb = zone->getAABBExt();
            if(!aabb.IsInside(pos))
            {
                pos = aabb.GetClosestPointTo(pos);
            }
        }
    }
    noeud->setPosition(pos);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Déplacement d'un NodeWallAbstract
///
/// @param[in] NodeWallAbstract * n : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudMuret( NodeWallAbstract* noeud )
{
    // assume ici qu'un muret relatif ne peut etre selectionné
    NoeudMuret* muret = (NoeudMuret*)noeud;
    Vecteur3 pos1 = noeud->obtenirCoin1() + deplacement_;
    Vecteur3 pos2 = noeud->obtenirCoin2() + deplacement_;
    auto field = noeud->getField();
    if(field)
    {
        auto zone = field->getZoneEdition();
        if(zone)
        {
            auto aabb = zone->getAABBExt();
            if(!aabb.IsInside(pos1))
            {
                pos1 = aabb.GetClosestPointTo(pos1);
            }
            if(!aabb.IsInside(pos2))
            {
                pos2 = aabb.GetClosestPointTo(pos2);
            }
        }
    }
    muret->assignerPositionCoin(1, pos1);
    muret->assignerPositionCoin(2, pos2);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudBut( NoeudBut* noeud )
///
/// Déplacement d'un NoeudBut
///
/// @param[in] NoeudBut * n : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudBut( NoeudBut* noeud )
{
    visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudMaillet( NoeudMaillet* noeud ) 
///
/// Déplacement d'un noeudMaillet
///
/// @param[in] NoeudMaillet * n : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudMaillet( NoeudMaillet* noeud )
{
    visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Déplacement d'un NoeudPortail
///
/// @param[in] NoeudPortail * n : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudPortail( NoeudPortail* noeud )
{   
    visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Déplacement d'un NoeudRondelle
///
/// @param[in] NoeudRondelle * n : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudRondelle( NoeudRondelle* noeud )
{
    visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Déplacement d'un NoeudPoint
///
/// @param[in] NoeudTable * noeud : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudPoint( NoeudPoint* noeud )
{
    const Vecteur3& posRelative = noeud->getPosition();
    TypePosPoint type = noeud->obtenirTypePosNoeud();

    // check pour les noeuds Haut et bas milieu
    if(type == POSITION_HAUT_MILIEU || type == POSITION_BAS_MILIEU)
    {
        if(noeud->validerDeplacement(posRelative, deplacement_,VY))
        {
            noeud->setPosition(posRelative+Vecteur3(0, deplacement_[VY], 0));
            noeud->obtenirPointSym()->setPosition(noeud->obtenirPointSym()->getPosition()+Vecteur3(0, -deplacement_[VY], 0));
        }
    }
    else if(type == POSITION_MILIEU_GAUCHE || type == POSITION_MILIEU_DROITE)
    {
        if(noeud->validerDeplacement(posRelative, deplacement_,VX))
        {
            noeud->setPosition(posRelative+Vecteur3(deplacement_[VX], 0, 0));
            noeud->obtenirPointSym()->setPosition(noeud->obtenirPointSym()->getPosition()+Vecteur3(-deplacement_[VX],0, 0));            
        }
    }

    else if(type == POSITION_HAUT_GAUCHE || type == POSITION_HAUT_DROITE || type == POSITION_BAS_DROITE || type == POSITION_BAS_GAUCHE)
    {
        if(noeud->validerDeplacement(posRelative, deplacement_,VY) && noeud->validerDeplacement(posRelative, deplacement_,VX))
        {
            noeud->setPosition(posRelative+deplacement_);
            deplacement_[VX]*=-1;
            noeud->obtenirPointSym()->setPosition(noeud->obtenirPointSym()->getPosition()+deplacement_);
            // revert change since other node might want the modif
            deplacement_[VX]*=-1;
        }
    }

    // Recalcul de la longueur des buts 
    auto field = noeud->getField();
    checkf(field);
    if(field)
    {
        NoeudTable* table = field->getTable();
        checkf(table);
        if(table)
        {
            table->obtenirBut(1)->updateLongueur();
            table->obtenirBut(2)->updateLongueur();
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurDeplacement::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Déplacement d'un NoeudAccélérateur
///
/// @param[in] NoeudAccelerateur * noeud : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
    visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurDeplacement::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// Visitation d'un noeud point de control
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    visiterNoeudAbstrait(noeud);
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
