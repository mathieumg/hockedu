//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurDeplacement.cpp
/// @author Mathieu Parent, Michael Ferris
/// @date 2012-02-06
/// @version 1.0 
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
#include "FacadeModele.h"
#include "NoeudAccelerateur.h"
#include "aiScene.h"
#include "Modele3D.h"
#include "..\Noeuds\NodeControlPoint.h"
#include "ExceptionJeu.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::VisiteurDeplacement()
///
///
/// @param[in]	deplacementVirtuel	: deplacement a effectuer en coord virtuelles
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurDeplacement::VisiteurDeplacement( Vecteur2 deplacementVirtuel, bool ignoreSelection ):
ignoreSelection_(ignoreSelection)
{
	deplacement_ = deplacementVirtuel;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::VisiteurDeplacement()
///
///
/// @param[in]	angle		: angle de rotation a appliquer (en degres)
///				centreRot	: centre autour duquel la rotation doit etre effectuee
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurDeplacement::VisiteurDeplacement( Vecteur3 deplacementVirtuel, bool ignoreSelection ):
ignoreSelection_(ignoreSelection)
{
	deplacement_[VX] = deplacementVirtuel[VX];
	deplacement_[VY] = deplacementVirtuel[VY];
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
	
	// N'applique pas le deplacement si le noeud n'est pas selectionne
	if(!ignoreSelection_ && !noeud->estSelectionne())
		return;

	noeud->setPosition(noeud->getPosition()+Vecteur3(-deplacement_[VX], deplacement_[VY], 0));
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurDeplacement::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Déplacement d'un NoeudComposite
///
/// @param[in] NoeudComposite * n : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudComposite( NoeudComposite* noeud )
{
	// On effectue le deplacement sur les enfants egalement
	
	//visiterNoeudAbstrait(noeud);
	if(!ignoreSelection_ && !noeud->estSelectionne())
	{
		if(noeud->obtenirNombreEnfants() != 0)
		{
			for (ConteneurNoeuds::iterator it = noeud->obtenirEnfants().begin(); it != noeud->obtenirEnfants().end(); it++) {

				NoeudComposite* enfant = static_cast<NoeudComposite*> (*it);
				visiterNoeudComposite(noeud);
			}
		}
	}

	
	
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
    if(!ignoreSelection_ && !noeud->estSelectionne())
        return;
    // assume ici qu'un muret relatif ne peut etre selectionné
    NoeudMuret* muret = (NoeudMuret*)noeud;
    muret->assignerPositionCoin(1, noeud->obtenirCoin1()+Vecteur3(-deplacement_[VX], deplacement_[VY], 0));
	muret->assignerPositionCoin(2, noeud->obtenirCoin2()+Vecteur3(-deplacement_[VX], deplacement_[VY], 0));
    visiterNoeudComposite(noeud);
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
/// @fn VisiteurDeplacement::visiterNoeudTable( NoeudTable* noeud )
///
/// Déplacement d'un NoeudTable
///
/// @param[in] NoeudTable * n : noeud à visiter
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurDeplacement::visiterNoeudTable( NoeudTable* noeud )
{
	// Pas de deplacement permis
	visiterNoeudComposite(noeud);
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
	if(!ignoreSelection_ && !noeud->estSelectionne())
		return;
	
	//const GroupeTripleAdresseFloat* liste = noeud->obtenirListePointsAChanger();
	//const GroupeTripleAdresseFloat* listeSym = noeud->obtenirPointSym()->obtenirListePointsAChanger();

	const Vecteur3& posRelative = noeud->getPosition();
    TypePosPoint type = noeud->obtenirTypePosNoeud();
    // check pour les noeuds Haut et bas milieu


	// check pour les noeuds Haut et bas milieu
	if(type == POSITION_HAUT_MILIEU || type == POSITION_BAS_MILIEU)
	{
		if(noeud->validerDeplacement(posRelative, deplacement_,VY))
		{
			noeud->assignerPositionRelative(posRelative+Vecteur3(0, deplacement_[VY], 0));
			noeud->obtenirPointSym()->assignerPositionRelative(noeud->obtenirPointSym()->getPosition()+Vecteur3(0, -deplacement_[VY], 0));
		}
	}
	else if(type == POSITION_MILIEU_GAUCHE || type == POSITION_MILIEU_DROITE)
	{
		if(noeud->validerDeplacement(posRelative, -deplacement_,VX))
		{
			noeud->assignerPositionRelative(posRelative+Vecteur3(-deplacement_[VX], 0, 0));
			noeud->obtenirPointSym()->assignerPositionRelative(noeud->obtenirPointSym()->getPosition()+Vecteur3(deplacement_[VX],0, 0));			
		}
	}

	else if(type == POSITION_HAUT_GAUCHE || type == POSITION_HAUT_DROITE || type == POSITION_BAS_DROITE || type == POSITION_BAS_GAUCHE)
	{
		if(noeud->validerDeplacement(posRelative, deplacement_,VY) && noeud->validerDeplacement(posRelative, -deplacement_,VX))
		{
			noeud->assignerPositionRelative(posRelative+Vecteur3(-deplacement_[VX], deplacement_[VY], 0));
			noeud->obtenirPointSym()->assignerPositionRelative(noeud->obtenirPointSym()->getPosition()+deplacement_.convertir<3>());
		}
	}

	// Recalcul de la longueur des buts 
	NoeudTable* table = dynamic_cast<NoeudTable*>(noeud->obtenirParent());
	if(table == 0)
		throw ExceptionJeu("Parent d'un point n'est pas une table, arbre invalide");
	table->obtenirBut(1)->updateLongueur();
	table->obtenirBut(2)->updateLongueur();
	
	
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


