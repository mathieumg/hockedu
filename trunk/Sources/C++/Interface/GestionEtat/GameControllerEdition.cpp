//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeJeu.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GameControllerEdition.h"
#include "FacadeModele.h"
#include "SourisEtatAbstrait.h"
#include "GestionnaireModeles.h"
#include "SoundFMOD.h"
#include "GestionnaireHUD.h"
#include "SourisEtatSelection.h"
#include "SourisEtatAjout.h"
#include "Terrain.h"
#include "ConfigScene.h"
#include "SourisEtatPIEMode.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GameControllerEdition::GameControllerEdition(GestionnaireEvenements* contexte)
///
/// Constructeur qui initialise le contexte et mets un événement de souris par défaut.
///
/// @param[in] touche : Le nombre de coches parcouru sur la roulette.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GameControllerEdition::GameControllerEdition(Terrain* pField):
GameControllerAbstract(),mField(pField)
{
    mAutoSaveTimer.unPause();
    shiftEnfonce_ = false;
    enfonce_ = false;
    boutonEnfonce_ = NULL;
    positionSouris_ = NULL;
    modifierEtatSouris(ETAT_SOURIS_ORBIT);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés à l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerEdition::toucheEnfoncee(EvenementClavier& evenementClavier)
{
    ToucheClavier touche = evenementClavier.obtenirTouche();
    
    if(touche==VJAK_SHIFT)
        shiftEnfonce_ = true;
    else
        toucheSauvegardee_ = touche;
    
    if(etatSouris_)
    {
        etatSouris_ ->toucheEnfoncee(evenementClavier);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés au relâchement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerEdition::toucheRelachee( EvenementClavier& evenementClavier )
{
    ToucheClavier touche = evenementClavier.obtenirTouche();
    if(etatSouris_)
        etatSouris_ ->toucheRelachee(evenementClavier);

    switch (touche)
    {
    case VJAK_SHIFT:
        shiftEnfonce_ = false;
        break;
    default:
        break;
    }


    if(toucheSauvegardee_==touche)
        toucheSauvegardee_ = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés à l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerEdition::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
        enfonce_ = true;
    positionSouris_ = evenementSouris.obtenirPosition();
    boutonEnfonce_ = evenementSouris.obtenirBouton();
    if(etatSouris_)
        etatSouris_->sourisEnfoncee(evenementSouris);
}   

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au relâchement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerEdition::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
        enfonce_ = false;

    if(etatSouris_)
    {
        etatSouris_->sourisRelachee(evenementSouris);
        SoundFMOD::obtenirInstance()->playEffect(CLICK_EFFECT);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au déplacement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerEdition::sourisDeplacee( EvenementSouris& evenementSouris )
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
    if(etatSouris_)
    {
        etatSouris_->sourisDeplacee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris)
///
/// Fonction qui gère les événements liés au déplacement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerEdition::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
    // Application du zoom.
    //evenementRouletteSouris
    GameControllerAbstract::rouletteSouris(evenementRouletteSouris);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::miseAJourEvenementsRepetitifs( float deltaTemps )
///
/// Fonction qui appelle les fonctions qui doivent etre repetees tant que le bouton est enfonce
///
/// @param[in] deltaTemps : temps en seconde depuis le dernier appel
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerEdition::miseAJourEvenementsRepetitifs( float deltaTemps )
{
	GameControllerAbstract::CameraMovementFromKeyPressed(deltaTemps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::animer( const float& temps )
///
/// Animation dans le mode édition.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerEdition::animer( const float& temps )
{
    if(!etatSouris_ || !etatSouris_->OverrideAnimate(temps))
    {
        SoundFMOD::obtenirInstance()->repeatAppSong();
        // Mise à jour des objets
        if(mField)
        {
            mField->animerTerrain(temps);
        }
        if(mField && ConfigScene::obtenirInstance()->IsAutoSaveEnable())
        {
            if(mAutoSaveTimer.Elapsed_Time_sec() > ConfigScene::obtenirInstance()->getAutoSaveDelai())
            {
                mAutoSaveTimer.reset_Time();
                RazerGameUtilities::SaveFieldToFile(FacadeModele::FICHIER_TERRAIN_EN_COURS,*mField);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerEdition::afficher()
{
    if(!etatSouris_ || !etatSouris_->OverrideRender())
    {
        renderBase(mField);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerEdition::modifierEtatSouris( NomEtatSouris etatSouris )
///
/// /*Description*/
///
/// @param[in] NomEtatSouris etatSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerEdition::modifierEtatSouris( NomEtatSouris etatSouris )
{
    clearMouseState(etatSouris);
    if(!etatSouris_)
    {
        // petit hack pour pouvoir ajouter des polygones
        /*static */int i=0;
        switch(etatSouris)
        {
        case ETAT_SOURIS_TRANSFORMATION_ROTATION    : etatSouris_ = new SourisEtatTransformation ( FIELD_MODIFICATION_ROTATE     ); break;
        case ETAT_SOURIS_TRANSFORMATION_ECHELLE     : etatSouris_ = new SourisEtatTransformation ( FIELD_MODIFICATION_SCALE      ); break;
        case ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT : etatSouris_ = new SourisEtatTransformation ( FIELD_MODIFICATION_MOVE       ); break;
        case ETAT_SOURIS_SELECTION                  : etatSouris_ = new SourisEtatSelection      (                               ); break;
        case ETAT_SOURIS_AJOUTER_PORTAIL            : etatSouris_ = new SourisEtatAjout          ( FIELD_MODIFICATION_ADD_PORTAL );  break;
        case ETAT_SOURIS_AJOUTER_MURET              : etatSouris_ = new SourisEtatAjout          ( FIELD_MODIFICATION_ADD_WALL   ); break;
        case ETAT_SOURIS_AJOUTER_MAILLET            : etatSouris_ = new SourisEtatAjout          ( FIELD_MODIFICATION_ADD_MALLET );  break;
        case ETAT_SOURIS_AJOUTER_RONDELLE           : etatSouris_ = new SourisEtatAjout          ( FIELD_MODIFICATION_ADD_PUCK   ); break;
        case ETAT_SOURIS_AJOUTER_ACCELERATEUR       : etatSouris_ = new SourisEtatAjout          ( FIELD_MODIFICATION_ADD_BOOST  ); break;
        case ETAT_SOURIS_AJOUTER_BONUS              : etatSouris_ = new SourisEtatAjout          ( FIELD_MODIFICATION_ADD_BONUS  ); break;
        case ETAT_SOURIS_PIE_MODE                   : etatSouris_ = new SourisEtatPIEMode        ( positionSouris_ ); break;
        default: Super::modifierEtatSouris(etatSouris);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



