//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeJeu.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEtatModeEdition.h"
#include "FacadeModele.h"
#include "SourisEtatAbstrait.h"
#include "GestionnaireModeles.h"
#include "SoundFMOD.h"
#include "GestionnaireHUD.h"
#include "SourisEtatSelection.h"
#include "SourisEtatAjout.h"
#include "Terrain.h"
#include "ConfigScene.h"


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
GestionnaireEtatModeEdition::GestionnaireEtatModeEdition(Terrain* pField):
GestionnaireEtatAbstrait(),mField(pField)
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
    
    if(etatSouris_)
    {
        etatSouris_ ->toucheEnfoncee(evenementClavier);
    }
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
    if(etatSouris_)
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

    if(etatSouris_)
    {
        etatSouris_->sourisRelachee(evenementSouris);
        SoundFMOD::obtenirInstance()->playEffect(CLICK_EFFECT);
    }
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
    if(etatSouris_)
    {
        etatSouris_->sourisDeplacee(evenementSouris);
    }
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
/// @fn void GestionnaireEtatModeEdition::miseAJourEvenementsRepetitifs( float deltaTemps )
///
/// Fonction qui appelle les fonctions qui doivent etre repetees tant que le bouton est enfonce
///
/// @param[in] deltaTemps : temps en seconde depuis le dernier appel
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::miseAJourEvenementsRepetitifs( float deltaTemps )
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
    renderBase(mField);
    //GestionnaireHUD::obtenirInstance()->dessinerHUDEdition();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeEdition::modifierEtatSouris( NomEtatSouris etatSouris )
///
/// /*Description*/
///
/// @param[in] NomEtatSouris etatSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::modifierEtatSouris( NomEtatSouris etatSouris )
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
        default: Super::modifierEtatSouris(etatSouris);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



