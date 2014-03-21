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

#include "EventManager.h"
#include "GameControllerEdition.h"
#include "GameControllerQuickPlay.h"
#include "SoundFMOD.h"
#include "CodesEvenements.h"
#include "FacadeModele.h"
#include "Vue.h"
#include "GameControllerMenu.h"
#include "GameControllerTournament.h"
#include "GameControllerQuickPlayEnd.h"
#include "GameControllerTournamentEnd.h"
#include "GameControllerSimulation.h"
#include "ConfigScene.h"
#include "GameTime.h"

std::set<ToucheClavier> pressedKeys;

GameControllerAbstract* EventManager::etatCourant_ = new GameControllerMenu(); 
Flags<char,NB_CAMERA_PRESSED_DIRECTIONS> EventManager::mCameraDirection;
Flags<char,NB_MALLET_PRESSED_DIRECTIONS> EventManager::mMalletDirection;
Vecteur2i EventManager::mMouseScreenPos;
Vecteur3  EventManager::mMouseGamePos;
GameTime* EventManager::mDoubleClickTimer = new GameTime(false);
const float DoubleClickDelay = 500; // in ms
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEvenements::modifierEtat(GameControllerAbstract* nouvelEtat)
///
/// Fonction qui gère la modification de l'état du programme.
///
/// @param[in] etatSouris : Le nouvel état.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventManager::modifierEtat(const EtatGestion &nouvelEtat)
{
	if(etatCourant_)
		delete etatCourant_;
    etatCourant_ = NULL;
	switch(nouvelEtat)
	{
		case ETAT_MENU_PRINCIPAL: etatCourant_ = new GameControllerMenu(); break;
		case ETAT_MODE_EDITION: etatCourant_ = new GameControllerEdition(FacadeModele::getInstance()->getEditionField()); break;
        case ETAT_MODE_JEU: etatCourant_ = new GameControllerQuickPlay(FacadeModele::getInstance()->obtenirPartieCouranteId()); break;
        case ETAT_MODE_TOURNOI: etatCourant_ = new GameControllerTournament(FacadeModele::getInstance()->obtenirTournoi()); break;
		case ETAT_PARTIE_RAPIDE_TERMINEE: etatCourant_ = new GameControllerQuickPlayEnd(); break;
        case ETAT_PARTIE_TOURNOI_TERMINEE: etatCourant_ = new GameControllerTournamentEnd(); break;
        case ETAT_MODE_SIMULATION: etatCourant_ = new GameControllerSimulation(); break;
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
void EventManager::toucheEnfoncee(EvenementClavier& evenementClavier)
{
    auto touche = evenementClavier.obtenirTouche();
    switch(touche)
    {
    case VJAK_P:
        SoundFMOD::obtenirInstance()->togglePlaying();
        break;
    case VJAK_0:
        FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->getTableWidth());
		break;
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

	case VJAK_UP:
		mCameraDirection.SetFlag(true,CAMERA_UP);
		break;
	case VJAK_DOWN:
		mCameraDirection.SetFlag(true,CAMERA_DOWN);
		break;
	case VJAK_LEFT:
		mCameraDirection.SetFlag(true,CAMERA_LEFT);
		break;
	case VJAK_RIGHT:
		mCameraDirection.SetFlag(true,CAMERA_RIGHT);
		break;
    default:
        break;
    };
	if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
		mMalletDirection.SetFlag(true,MALLET_UP);
	else if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
		mMalletDirection.SetFlag(true,MALLET_LEFT);
	else if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
		mMalletDirection.SetFlag(true,MALLET_RIGHT);
	else if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
		mMalletDirection.SetFlag(true,MALLET_DOWN);

	pressedKeys.insert(touche);
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
void EventManager::toucheRelachee( EvenementClavier& evenementClavier )
{
	auto touche = evenementClavier.obtenirTouche();
	switch(touche)
	{
	case VJAK_UP:
		mCameraDirection.SetFlag(false,CAMERA_UP);
		break;
	case VJAK_DOWN:
		mCameraDirection.SetFlag(false,CAMERA_DOWN);
		break;
	case VJAK_LEFT:
		mCameraDirection.SetFlag(false,CAMERA_LEFT);
		break;
	case VJAK_RIGHT:
		mCameraDirection.SetFlag(false,CAMERA_RIGHT);
		break;
	default:
		break;
	};
	if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
		mMalletDirection.SetFlag(false,MALLET_UP);
	else if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
		mMalletDirection.SetFlag(false,MALLET_LEFT);
	else if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
		mMalletDirection.SetFlag(false,MALLET_RIGHT);
	else if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
		mMalletDirection.SetFlag(false,MALLET_DOWN);

	pressedKeys.erase(evenementClavier.obtenirTouche());
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
void EventManager::sourisEnfoncee( EvenementSouris& evenementSouris )
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
void EventManager::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(mDoubleClickTimer->Elapsed_Time_ms() < DoubleClickDelay)
    {
        if(etatCourant_)etatCourant_->doubleClickEvent(evenementSouris);
    }
    else
    {
        mDoubleClickTimer->reset_Time();
        if(etatCourant_)etatCourant_->sourisRelachee(evenementSouris);
    }
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
void EventManager::sourisDeplacee( EvenementSouris& evenementSouris )
{
	mMouseScreenPos = evenementSouris.obtenirPosition();
	FacadeModele::getInstance()->convertirClotureAVirtuelle(mMouseScreenPos.X(),mMouseScreenPos.Y(),mMouseGamePos);

	if(etatCourant_)etatCourant_->sourisDeplacee(evenementSouris);
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
void EventManager::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
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
void EventManager::miseAJourEvenementsRepetitifs( float detlaTemps )
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
void EventManager::modifierEtatSouris( NomEtatSouris etatSouris )
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
void EventManager::afficher()
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
void EventManager::animer( float temps )
{
    if(etatCourant_)
    {
        etatCourant_->animer(temps);
    }
}

bool EventManager::IsKeyPressed( ToucheClavier key )
{
	return pressedKeys.find(key) != pressedKeys.end();
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



