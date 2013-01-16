//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutMuret.cpp
/// @author Mathieu Parent
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjoutMuret.h"
#include "VisiteurEchelle.h"
#include "VisiteurRotation.h"
#include "FacadeModele.h"
#include "VisiteurCollision.h"
#include "NoeudMuret.h"
#include "ArbreRenduINF2990.h"
#include "Terrain.h"
#include "HUDTexte.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutMuret::SourisEtatAjoutMuret()
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage et initialise les attributs
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutMuret::SourisEtatAjoutMuret():SourisEtatAjout(ArbreRenduINF2990::NOM_MURET)
{
	etat_ = PLACERPOINT1;
	positionClic1_ = 0;
	positionClic2_ = 0;
}

	
////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutMuret::~SourisEtatAjoutMuret(void)
///
/// Ce destructeur supprime le noeud en cours d'ajout
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutMuret::~SourisEtatAjoutMuret(void)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuret::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuret::sourisEnfoncee( EvenementSouris& evenementSouris )
{ 

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuret::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuret::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(noeud_ == NULL)
		return;

	if(evenementSouris.obtenirBouton()!=BOUTON_SOURIS_GAUCHE)
		return;
	hudTextPosInvalide->modifierVisibilite(false);
	if(!FacadeModele::getInstance()->validerPositionNoeud(noeud_))
	{
		hudTextPosInvalide->modifierVisibilite(true);
		return;
	}
		

	switch(etat_)
	{
	case PLACERPOINT1:
		{
			// On fixe le premier point
			FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], positionClic1_);
			VisiteurCollision v(positionClic1_.convertir<2>());
			FacadeModele::getInstance()->visiterArbre(&v);
			noeud_->modifierSurligner(false);

			if(v.collisionPresente() && v.obtenirNbrNoeudEncollision() > 1)
				return;
			((NoeudMuret*)noeud_)->assignerPositionCoin(1, positionClic1_);
			((NoeudMuret*)noeud_)->assignerPositionCoin(2, positionClic1_);
			etat_ = PLACERPOINT2;
			break;
		}
	case PLACERPOINT2:
		// On fixe le deuxieme point
		FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], positionClic2_);
		FacadeModele::getInstance()->getTerrain()->transfererNoeud(noeud_);

		genererNoeud();

		etat_ = PLACERPOINT1;

		break;
	default:
		// Gestion des erreurs
		etat_ = PLACERPOINT1;
		break;
	}
		
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuret::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est déplacée
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuret::sourisDeplacee( EvenementSouris& evenementSouris )
{
	if(noeud_ == NULL)
		return;

	FacadeModele::getInstance()->getTerrain()->ajouterNoeudTemp(noeud_);

	Vecteur2i positionSouris;
	Vecteur3 positionVirtuelle;
	Vecteur3 positionVirtuelle2;
	double	deltaX, deltaY, angle, rayon;
	switch(etat_)
	{
	case PLACERPOINT1:
		// On affiche la forme de base
		positionSouris=evenementSouris.obtenirPosition();
		FacadeModele::getInstance()->convertirClotureAVirtuelle(positionSouris[VX],positionSouris[VY],positionVirtuelle);
		noeud_->assignerPositionRelative(positionVirtuelle);
		break;
	case PLACERPOINT2:
		// On affiche la forme modifiee
		positionSouris = evenementSouris.obtenirPosition();
		FacadeModele::getInstance()->convertirClotureAVirtuelle(positionSouris[VX], positionSouris[VY], positionVirtuelle);
		deltaX = positionVirtuelle[VX]-positionClic1_[VX];
		deltaY = positionVirtuelle[VY]-positionClic1_[VY];
		angle = (atan2(deltaY, deltaX)*180.0/(double)M_PI);
		rayon = sqrt(deltaX*deltaX+deltaY*deltaY);
		if(rayon>1)
		{
			((NoeudMuret*)noeud_)->assignerPositionCoin(2, positionVirtuelle);
			noeud_->modifierEchelleCourante(Vecteur3(rayon, 1, 1));
			//muretConcerne_->assignerAngle((int)angle);
			//muretConcerne_->updateMatrice();
		}

		// Appel pour faire le surlignement
		FacadeModele::getInstance()->validerPositionNoeud(noeud_, true);

		break;
	default:
		break;
	}

	
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


