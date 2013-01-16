//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeJeu.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEtatModeEdition.h"
#include "FacadeModele.h"
#include "SourisEtatAbstrait.h"
#include "GestionnaireModeles.h"
#include "ArbreRenduINF2990.h"
#include "SoundFMOD.h"
#include "GestionnaireHUD.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatModeEdition::GestionnaireEtatModeEdition(GestionnaireEvenements* contexte)
///
/// Constructeur qui initialise le contexte et mets un événement de souris par défaut.
///
/// @param[in] touche : Le nombre de coches parcouru sur la roulette.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeEdition::GestionnaireEtatModeEdition(GestionnaireEvenements* contexte):
GestionnaireEtatAbstrait(contexte)
{
	shiftEnfonce_ = false;
	enfonce_ = false;
	boutonEnfonce_ = NULL;
	positionSouris_ = NULL;
	modifierEtatSouris(ETAT_SOURIS_ORBIT);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés à l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::toucheEnfoncee(EvenementClavier& evenementClavier)
{
	ToucheClavier touche = evenementClavier.obtenirTouche();
	
	if(touche==VJAK_SHIFT)
		shiftEnfonce_ = true;
	else
		toucheSauvegardee_ = touche;
	
	etatSouris_ ->toucheEnfoncee(evenementClavier);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés au relâchement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::toucheRelachee( EvenementClavier& evenementClavier )
{
	ToucheClavier touche = evenementClavier.obtenirTouche();
	etatSouris_ ->toucheRelachee(evenementClavier);
	if(touche==VJAK_SHIFT)
		shiftEnfonce_ = false;
	if(toucheSauvegardee_==touche)
		toucheSauvegardee_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés à l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
		enfonce_ = true;
	positionSouris_ = evenementSouris.obtenirPosition();
	boutonEnfonce_ = evenementSouris.obtenirBouton();
	etatSouris_->sourisEnfoncee(evenementSouris);
}	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au relâchement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
		enfonce_ = false;

	etatSouris_->sourisRelachee(evenementSouris);
	SoundFMOD::obtenirInstance()->playEffect(CLICK_EFFECT);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au déplacement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::sourisDeplacee( EvenementSouris& evenementSouris )
{
	if(enfonce_ && boutonEnfonce_==BOUTON_SOURIS_MILIEU)
	{
		Vecteur2i delta = evenementSouris.obtenirPosition()-positionSouris_;
		if(shiftEnfonce_)
		{
			FacadeModele::getInstance()->deplacerSouris(delta);

		}
		else
		{
			FacadeModele::getInstance()->orbit(delta);
		}
		positionSouris_ = evenementSouris.obtenirPosition();
	}
	etatSouris_->sourisDeplacee(evenementSouris);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris)
///
/// Fonction qui gère les événements liés au déplacement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	// Application du zoom.
	//evenementRouletteSouris
	GestionnaireEtatAbstrait::rouletteSouris(evenementRouletteSouris);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::miseAJourEvenementsRepetitifs( double deltaTemps )
///
/// Fonction qui appelle les fonctions qui doivent etre repetees tant que le bouton est enfonce
///
/// @param[in] deltaTemps :	temps en seconde depuis le dernier appel
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::miseAJourEvenementsRepetitifs( double deltaTemps )
{
	int tempsMs = (int)(deltaTemps*1000);
	switch(toucheSauvegardee_)
	{
	case VJAK_UP:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(0, -tempsMs));
		break;
	case VJAK_DOWN:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(0, tempsMs));
		break;
	case VJAK_LEFT:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(-tempsMs, 0));
		break;
	case VJAK_RIGHT:
		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(tempsMs, 0));
		break;
	case VJAK_ADD:
	case VJAK_PLUS:
		// Utilisation temporaire de la méthode pour le zooom associé à la roulette de la souris
		// -1 indique que c'est un zoomIn
		FacadeModele::getInstance()->zoom(-tempsMs);
		break;

	case VJAK_SUBTRACT:
	case VJAK_MINUS:
		// Utilisation temporaire de la méthode pour le zooom associé à la roulette de la souris
		// 1 indique que c'est un zoomOut
		FacadeModele::getInstance()->zoom(tempsMs);
		break;
	
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::animer( const float& temps )
///
/// Animation dans le mode édition.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->repeatAppSong();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::afficher()
{
	GestionnaireHUD::obtenirInstance()->dessinerHUDEdition();
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



