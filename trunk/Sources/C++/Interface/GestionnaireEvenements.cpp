//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEvenement.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#if BOX2D_DEBUG
#include "DebugRenderBox2D.h"
#endif

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
#include "GestionnaireEtatModeSimulation.h"



GestionnaireEtatAbstrait* GestionnaireEvenements::etatCourant_ = new GestionnaireEtatMenuPrincipal(); 

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
        case ETAT_MODE_SIMULATION: etatCourant_ = new GestionnaireEtatModeSimulation(); break;
	}
	checkf(etatCourant_);
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
    auto touche = evenementClavier.obtenirTouche() ;
    switch(touche)
    {
    case VJAK_P:
        SoundFMOD::obtenirInstance()->togglePlaying();
        break;
    case VJAK_0:
        FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->getTableWidth());
#if BOX2D_DEBUG
    case VJAK_NUMPAD0:
        {
            if(DebugRenderBox2D::mInstance)
            {
                auto flag = DebugRenderBox2D::mInstance->GetFlags();
                if(flag & b2Draw::e_shapeBit)
                {
                    DebugRenderBox2D::mInstance->ClearFlags(b2Draw::e_shapeBit);
                }
                else
                {
                    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_shapeBit);
                }
            }
        }
        break;
    case VJAK_NUMPAD1:
        {
            if(DebugRenderBox2D::mInstance)
            {
                auto flag = DebugRenderBox2D::mInstance->GetFlags();
                if(flag & b2Draw::e_jointBit)
                {
                    DebugRenderBox2D::mInstance->ClearFlags(b2Draw::e_jointBit);
                }
                else
                {
                    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_jointBit);
                }
            }
        }
        break;
    case VJAK_NUMPAD2:
        {
            if(DebugRenderBox2D::mInstance)
            {
                auto flag = DebugRenderBox2D::mInstance->GetFlags();
                if(flag & b2Draw::e_aabbBit)
                {
                    DebugRenderBox2D::mInstance->ClearFlags(b2Draw::e_aabbBit);
                }
                else
                {
                    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_aabbBit);
                }
            }
        }
        break;
    case VJAK_NUMPAD3:
        {
            if(DebugRenderBox2D::mInstance)
            {
                auto flag = DebugRenderBox2D::mInstance->GetFlags();
                if(flag & b2Draw::e_pairBit)
                {
                    DebugRenderBox2D::mInstance->ClearFlags(b2Draw::e_pairBit);
                }
                else
                {
                    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_pairBit);
                }
            }
        }
        break;
    case VJAK_NUMPAD4:
        {
            if(DebugRenderBox2D::mInstance)
            {
                auto flag = DebugRenderBox2D::mInstance->GetFlags();
                if(flag & b2Draw::e_centerOfMassBit)
                {
                    DebugRenderBox2D::mInstance->ClearFlags(b2Draw::e_centerOfMassBit);
                }
                else
                {
                    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_centerOfMassBit);
                }
            }
        }
        break;
    case VJAK_NUMPAD5:
        {
            if(DebugRenderBox2D::mInstance)
            {
                auto flag = DebugRenderBox2D::mInstance->GetFlags();
                if(flag & b2Draw::e_drawInactiveBit)
                {
                    DebugRenderBox2D::mInstance->ClearFlags(b2Draw::e_drawInactiveBit);
                }
                else
                {
                    DebugRenderBox2D::mInstance->AppendFlags(b2Draw::e_drawInactiveBit);
                }
            }
        }
        break;
#endif //BOX2D_INTEGRATED
    default:
        break;
    };


    if(etatCourant_)etatCourant_->toucheEnfoncee(evenementClavier);
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
	if(etatCourant_)etatCourant_->toucheRelachee(evenementClavier);
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
	if(etatCourant_)etatCourant_->sourisEnfoncee(evenementSouris);
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
	if(etatCourant_)etatCourant_->sourisRelachee(evenementSouris);
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
	if(etatCourant_)etatCourant_->sourisDeplacee(evenementSouris);
    auto partie = FacadeModele::getInstance()->obtenirPartieCourante();
    if(partie)
    {
        partie->setMousePosScreen(evenementSouris.obtenirPosition());
    }
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
	if(etatCourant_)etatCourant_->rouletteSouris(evenementRouletteSouris);
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
	if(etatCourant_)etatCourant_->miseAJourEvenementsRepetitifs(detlaTemps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::modifierEtatSouris( NomEtatSouris etatSouris )
///
/// Modifier l'état de la souris
///
/// @param[in] NomEtatSouris etatSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEvenements::modifierEtatSouris( NomEtatSouris etatSouris )
{
    if(etatCourant_)etatCourant_->modifierEtatSouris(etatSouris);
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
    if(etatCourant_)etatCourant_->afficher();
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
    if(etatCourant_)
    {
        etatCourant_->animer(temps);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



