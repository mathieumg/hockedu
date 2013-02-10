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
#include "VisiteurDeplacement.h"


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
VisiteurRotation::VisiteurRotation( float angle, Vecteur2 centreRot )
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
	
	// N'applique pas la rotation si le noeud n'est pas selectionne
	if(!noeud->estSelectionne())
		return;
	
	glPushMatrix(); // Sauvegarde de la matrice d'origine
	
	Vecteur3 positionNoeud = noeud->obtenirPositionAbsolue();

	float	deltaX = positionNoeud[VX]-centreRot_[VX],
			deltaY = positionNoeud[VY]-centreRot_[VY];

	float rayon = sqrt(deltaX*deltaX+deltaY*deltaY);

	float angleCourantRad = atan2(deltaY, deltaX);


	float centreRotRel[2];
	NoeudAbstrait* parent = noeud->obtenirParent();
	if(parent!=0)
	{
		centreRotRel[VX] = centreRot_[VX];
		centreRotRel[VY] = centreRot_[VY];
	}
	else
	{
		centreRotRel[VX] = centreRot_[VX]-static_cast<NoeudComposite*>(parent)->obtenirPositionAbsolue()[VX];
		centreRotRel[VY] = centreRot_[VY]-static_cast<NoeudComposite*>(parent)->obtenirPositionAbsolue()[VY];
	}

	

	noeud->assignerAngle(noeud->obtenirAngle()-angleRot_);

	Vecteur2 varDeplacement;
	varDeplacement[VX] = centreRotRel[VX]+rayon*cos((angleCourantRad-(angleRot_*(float)M_PI/180)));
	varDeplacement[VY] = centreRotRel[VY]+rayon*sin((angleCourantRad-(angleRot_*(float)M_PI/180)));
	
	Vecteur2 posInverse(-noeud->obtenirPositionRelative()[VX], -noeud->obtenirPositionRelative()[VY]);
	VisiteurDeplacement visiteurDeplacement(posInverse);
	noeud->acceptVisitor(visiteurDeplacement);
	noeud->assignerPositionRelative(Vecteur3(varDeplacement[VX], varDeplacement[VY], 0));


	// Il faut recalculer la matrice de transformation
	noeud->updateMatrice();
	
	
	glPopMatrix(); // Recuperation de la matrice d'origine


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
	// On effectue la rotation sur les enfants egalement
	
	for (unsigned int i=0; i<noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->acceptVisitor(*this);
	}

	
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurRotation::visiterNoeudMuret( NoeudMuret* noeud )
///
/// Rotation d'un NoeudMuret
///
/// @param[in] NoeudMuret * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurRotation::visiterNoeudMuret( NoeudMuret* noeud )
{
	noeud->assignerAngle(noeud->obtenirAngle()-angleRot_);
	visiterNoeudAbstrait(noeud);
	noeud->majPosCoins();
	visiterNoeudComposite(noeud);
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
	// Pas de rotation permise
	visiterNoeudComposite(noeud);
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
	// Pas de rotation permise
	visiterNoeudComposite(noeud);
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




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


