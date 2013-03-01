//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenement.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
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


GestionnaireEtatAbstrait* GestionnaireEvenements::etatCourant_ = new GestionnaireEtatMenuPrincipal(); 
MouseMoveSubject GestionnaireEvenements::mMouseMoveSubject;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::modifierEtat(GestionnaireEtatAbstrait* nouvelEtat)
///
/// Fonction qui g�re la modification de l'�tat du programme.
///
/// @param[in] etatSouris : Le nouvel �tat.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::modifierEtat(const EtatGestion &nouvelEtat)
{
	if(etatCourant_)
		delete etatCourant_;
    etatCourant_ = NULL;
	switch(nouvelEtat)
	{
		case ETAT_MENU_PRINCIPAL: etatCourant_ = new GestionnaireEtatMenuPrincipal(); break;
		case ETAT_MODE_EDITION: etatCourant_ = new GestionnaireEtatModeEdition(FacadeModele::getInstance()->getEditionField()); break;
        case ETAT_MODE_JEU: etatCourant_ = new GestionnaireEtatModeJeu(FacadeModele::getInstance()->obtenirPartieCourante()); break;
		case ETAT_MODE_TOURNOI: etatCourant_ = new GestionnaireEtatModeTournoi(); break;
		case ETAT_PARTIE_RAPIDE_TERMINEE: etatCourant_ = new GestionnaireEtatPartieRapideTerminee(); break;
		case ETAT_PARTIE_TOURNOI_TERMINEE: etatCourant_ = new GestionnaireEtatPartieTournoiTerminee(); break;
	}
	checkf(etatCourant_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui appelle la fonction du m�me nom de l'�tat courant.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
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
		FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->getTableWidth());
	}
    checkf(etatCourant_);etatCourant_->toucheEnfoncee(evenementClavier);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui appelle la fonction du m�me nom de l'�tat courant.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::toucheRelachee( EvenementClavier& evenementClavier )
{
	checkf(etatCourant_);etatCourant_->toucheRelachee(evenementClavier);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Fonction qui appelle la fonction du m�me nom de l'�tat courant.
///
/// @param[in] evenementSouris : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	checkf(etatCourant_);etatCourant_->sourisEnfoncee(evenementSouris);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Fonction qui appelle la fonction du m�me nom de l'�tat courant.
///
/// @param[in] evenementSouris : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::sourisRelachee( EvenementSouris& evenementSouris )
{
	checkf(etatCourant_);etatCourant_->sourisRelachee(evenementSouris);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Fonction qui appelle la fonction du m�me nom de l'�tat courant.
///
/// @param[in] evenementSouris : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::sourisDeplacee( EvenementSouris& evenementSouris )
{
	checkf(etatCourant_);etatCourant_->sourisDeplacee(evenementSouris);
    mMouseMoveSubject.mEvent = evenementSouris;
    mMouseMoveSubject.signalObservers();
    FacadeModele::getInstance()->MouseMove(evenementSouris);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Fonction qui appelle la fonction du m�me nom de l'�tat courant.
///
/// @param[in] evenementSouris : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	checkf(etatCourant_);etatCourant_->rouletteSouris(evenementRouletteSouris);
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
	checkf(etatCourant_);etatCourant_->miseAJourEvenementsRepetitifs(detlaTemps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::modifierEtatSouris( NomEtatSouris etatSouris )
///
/// Modifier l'�tat de la souris
///
/// @param[in] NomEtatSouris etatSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::modifierEtatSouris( NomEtatSouris etatSouris )
{
    checkf(etatCourant_);etatCourant_->modifierEtatSouris(etatSouris);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::afficher()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::afficher()
{
    checkf(etatCourant_);etatCourant_->afficher();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::animer( float temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::animer( float temps )
{
    checkf(etatCourant_);etatCourant_->animer(temps);
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



