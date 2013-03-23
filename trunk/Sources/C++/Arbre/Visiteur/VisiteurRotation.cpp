//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurRotation.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////


#include "VisiteurRotation.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "NodeWallEdition.h"
#include "NodeControlPoint.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurRotation::VisiteurRotation()
///
///
/// @param[in]	angle		: angle de rotation a appliquer (en degres)
///				centreRot	: centre autour duquel la rotation doit etre effectuee
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurRotation::VisiteurRotation( float angle, const Vecteur2& centreRot )
{
	angleRot_ = (int)angle;
	centreRot_ = centreRot;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurRotation::~VisiteurRotation()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurRotation::~VisiteurRotation(void)
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Rotation d'un NoeudAbstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
    rotateNode(noeud);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Rotation d'un NoeudComposite
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudComposite( NoeudComposite* noeud )
{
    visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Rotation d'un NodeWallAbstract
///
/// @param[in] NodeWallAbstract * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudMuret( NodeWallAbstract* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudBut( NoeudBut* noeud )
///
/// Rotation d'un NoeudBut
///
/// @param[in] NoeudBut * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudBut( NoeudBut* noeud )
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Rotation d'un noeudMaillet
///
/// @param[in] NoeudMaillet * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Rotation d'un NoeudPortail
///
/// @param[in] NoeudPortail * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudPortail( NoeudPortail* noeud )
{	
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Rotation d'un NoeudRondelle
///
/// @param[in] NoeudRondelle * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudTable( NoeudTable* noeud )
///
/// Rotation d'un NoeudTable
///
/// @param[in] NoeudTable * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudTable( NoeudTable* noeud )
{
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Rotation d'un NoeudPoint
///
/// @param[in] NoeudPoint * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudPoint( NoeudPoint* noeud )
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Rotation d'un NoeudAccélérateur
///
/// @param[in] NoeudAccelerateur * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudMuretEdition( NodeWallEdition* noeud )
///
/// /*Description*/
///
/// @param[in] NodeWallEdition * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudMuretEdition( NodeWallEdition* noeud )
{
    rotateNode(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::rotateNode( NoeudAbstrait* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::rotateNode( NoeudAbstrait* noeud )
{
    const Vecteur3& positionNoeud = noeud->getPosition();

    float	deltaX = positionNoeud[VX]-centreRot_[VX],
        deltaY = positionNoeud[VY]-centreRot_[VY];

    float rayon = sqrt(deltaX*deltaX+deltaY*deltaY);
    float angleCourantRad = atan2(deltaY, deltaX);

    noeud->setAngle(noeud->getAngle()-angleRot_);

    Vecteur2 varDeplacement;
    varDeplacement[VX] = centreRot_[VX]+rayon*cos((angleCourantRad-(angleRot_*(float)M_PI/180)));
    varDeplacement[VY] = centreRot_[VY]+rayon*sin((angleCourantRad-(angleRot_*(float)M_PI/180)));

    noeud->setPosition(varDeplacement);

    // Il faut recalculer la matrice de transformation
    noeud->updateMatrice();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    NoeudAbstrait* n = dynamic_cast<NoeudAbstrait*>(noeud->getLinkedObject());
    if(n)
    {
        n->acceptVisitor(*this);
    }
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


