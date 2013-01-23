//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenement.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEvenements.h"
#include "GestionnaireEtatModeEdition.h"
#include "GestionnaireEtatModeJeu.h"
#include "SoundFMOD.h"
#include "CodesEvenements.h"
#include "FacadeModele.h"
#include "Vue.h"
#include "GestionnaireEtatMenuPrincipal.h"
#include "GestionnaireEtatModeTournoi.h"
#include "GestionnaireEtatPartieRapideTerminee.h"
#include "GestionnaireEtatPartieTournoiTerminee.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireEvenements);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn GestionnaireEvenements::GestionnaireEvenements()
///
/// Constructeur qui initialise l'état
///
/// @return Aucune.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEvenements::GestionnaireEvenements():etatCourant_(0)
{
	modifierEtat(ETAT_MODE_EDITION);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEvenements::~GestionnaireEvenements()
///
/// Destructeur de la classe; désalloue la mémoire allouée pour l'état courant.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEvenements::~GestionnaireEvenements()
{
	if(etatCourant_ != 0)
		delete etatCourant_;
	etatCourant_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::modifierEtat(GestionnaireEtatAbstrait* nouvelEtat)
///
/// Fonction qui gère la modification de l'état du programme.
///
/// @param[in] etatSouris : Le nouvel état.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::modifierEtat(const EtatGestion &nouvelEtat)
{
	if(etatCourant_ != 0)
		delete etatCourant_;
	switch(nouvelEtat)
	{
		case ETAT_MENU_PRINCIPAL: etatCourant_ = new GestionnaireEtatMenuPrincipal(this); break;
		case ETAT_MODE_EDITION: etatCourant_ = new GestionnaireEtatModeEdition(this); break;
		case ETAT_MODE_JEU: etatCourant_ = new GestionnaireEtatModeJeu(this); break;
		case ETAT_MODE_TOURNOI: etatCourant_ = new GestionnaireEtatModeTournoi(this); break;
		case ETAT_PARTIE_RAPIDE_TERMINEE: etatCourant_ = new GestionnaireEtatPartieRapideTerminee(this); break;
		case ETAT_PARTIE_TOURNOI_TERMINEE: etatCourant_ = new GestionnaireEtatPartieTournoiTerminee(this); break;
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatAbstrait* GestionnaireEvenements::obtenirEtat()
///
/// Fonction qui retourne l'état courant.
///
/// @return etatCourant_ : L'état courant.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatAbstrait* GestionnaireEvenements::obtenirEtat()
{
	return etatCourant_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui appelle la fonction du même nom de l'état courant.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::toucheEnfoncee(EvenementClavier& evenementClavier)
{
	if(evenementClavier.obtenirTouche() == VJAK_P)
	{
		SoundFMOD::obtenirInstance()->togglePlaying();
	}
	if(evenementClavier.obtenirTouche() == VJAK_0)
	{
		FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->obtenirLargeurZoneEdition());
	}
	etatCourant_->toucheEnfoncee(evenementClavier);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui appelle la fonction du même nom de l'état courant.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::toucheRelachee( EvenementClavier& evenementClavier )
{
	etatCourant_->toucheRelachee(evenementClavier);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Fonction qui appelle la fonction du même nom de l'état courant.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	etatCourant_->sourisEnfoncee(evenementSouris);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Fonction qui appelle la fonction du même nom de l'état courant.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::sourisRelachee( EvenementSouris& evenementSouris )
{
	etatCourant_->sourisRelachee(evenementSouris);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Fonction qui appelle la fonction du même nom de l'état courant.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::sourisDeplacee( EvenementSouris& evenementSouris )
{
	etatCourant_->sourisDeplacee(evenementSouris);
    FacadeModele::getInstance()->MouseMove(evenementSouris);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Fonction qui appelle la fonction du même nom de l'état courant.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	etatCourant_->rouletteSouris(evenementRouletteSouris);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::miseAJourEvenementsRepetitifs( float detlaTemps )
///
/// Fonction qui appelle la mise a jour des touches pour l'etat courant.
///
/// @param[in] deltaTemps :	temps depuis le dernier appel
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::miseAJourEvenementsRepetitifs( float detlaTemps )
{
	etatCourant_->miseAJourEvenementsRepetitifs(detlaTemps);
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



