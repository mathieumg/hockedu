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
#include "SourisEtatTransformationRotation.h"
#include "SourisEtatTransformationEchelle.h"
#include "SourisEtatTransformationDeplacement.h"
#include "SourisEtatSelection.h"
#include "SourisEtatAjout.h"
#include "SourisEtatAjoutControlPointMutable.h"
#include "Terrain.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatModeEdition::GestionnaireEtatModeEdition(GestionnaireEvenements* contexte)
///
/// Constructeur qui initialise le contexte et mets un �v�nement de souris par d�faut.
///
/// @param[in] touche : Le nombre de coches parcouru sur la roulette.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeEdition::GestionnaireEtatModeEdition(Terrain* pField):
GestionnaireEtatAbstrait(),mField(pField)
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
/// Fonction qui g�re les �v�nements li�s � l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
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
/// Fonction qui g�re les �v�nements li�s au rel�chement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::toucheRelachee( EvenementClavier& evenementClavier )
{
    ToucheClavier touche = evenementClavier.obtenirTouche();
    if(etatSouris_)
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
/// Fonction qui g�re les �v�nements li�s � l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
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
/// Fonction qui g�re les �v�nements li�s au rel�chement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
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
/// Fonction qui g�re les �v�nements li�s au d�placement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant � l'�v�nement.
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
/// Fonction qui g�re les �v�nements li�s au d�placement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant � l'�v�nement.
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
        // Utilisation temporaire de la m�thode pour le zooom associ� � la roulette de la souris
        // -1 indique que c'est un zoomIn
        FacadeModele::getInstance()->zoom(-tempsMs);
        break;

    case VJAK_SUBTRACT:
    case VJAK_MINUS:
        // Utilisation temporaire de la m�thode pour le zooom associ� � la roulette de la souris
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
/// Animation dans le mode �dition.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeEdition::animer( const float& temps )
{
    SoundFMOD::obtenirInstance()->repeatAppSong();
    // Mise � jour des objets
    if(FacadeModele::getInstance()->getEditionField())
    {
        FacadeModele::getInstance()->getEditionField()->animerTerrain(temps);
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
    GestionnaireHUD::obtenirInstance()->dessinerHUDEdition();
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
        case ETAT_SOURIS_TRANSFORMATION_ROTATION    : etatSouris_ = new SourisEtatTransformationRotation    (); break;
        case ETAT_SOURIS_TRANSFORMATION_ECHELLE     : etatSouris_ = new SourisEtatTransformationEchelle     (); break;
        case ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT : etatSouris_ = new SourisEtatTransformationDeplacement (); break;
        case ETAT_SOURIS_SELECTION                  : etatSouris_ = new SourisEtatSelection                 (); break;
        case ETAT_SOURIS_AJOUTER_PORTAIL            : etatSouris_ = new SourisEtatAjout                     (mField,RazerGameUtilities::NOM_PORTAIL);  break;
        case ETAT_SOURIS_AJOUTER_MURET              : etatSouris_ = new SourisEtatAjoutControlPointMutable  (mField, ((i++)&1) == 0 ? RazerGameUtilities::NAME_RELATIVE_WALL : RazerGameUtilities::NAME_POLYGONE); break;
        case ETAT_SOURIS_AJOUTER_MAILLET            : etatSouris_ = new SourisEtatAjout                     (mField,RazerGameUtilities::NOM_MAILLET);  break;
        case ETAT_SOURIS_AJOUTER_RONDELLE           : etatSouris_ = new SourisEtatAjout                     (mField,RazerGameUtilities::NOM_RONDELLE); break;
        case ETAT_SOURIS_AJOUTER_ACCELERATEUR       : etatSouris_ = new SourisEtatAjout                     (mField,RazerGameUtilities::NOM_ACCELERATEUR); break;
        default: Super::modifierEtatSouris(etatSouris);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



