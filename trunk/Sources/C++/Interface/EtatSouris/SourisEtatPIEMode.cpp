//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatPIEMode.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatPIEMode.h"
#include "FacadeModele.h"
#include "Vue.h"
#include "CodesEvenements.h"
#include "JoueurHumain.h"
#include "Partie.h"
#include "Terrain.h"
#include "ConfigScene.h"
#include "NoeudMaillet.h"
#include "SoundFMOD.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatPIEMode::SourisEtatPIEMode(  )
///
/// Initialisation des attributs � leur valeur de base
///
/// @param[in] context : R�f�rence du gestionnaire d'�tat de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatPIEMode::SourisEtatPIEMode(  ):shiftEnfonce_(false)
{
    shiftEnfonce_ = false;
    mMiddleMousePress = false;

    mPIEGame = new Partie(GAME_TYPE_OFFLINE,SPJoueurHumain(new JoueurHumain("Left Player")),SPJoueurHumain(new JoueurHumain("Right Player")),-1);
    mPIEGame->getField()->setResizeTableModel(false);
    auto xml = FacadeModele::getInstance()->getEditionField()->creerNoeudXML();
    mPIEGame->setFieldName("PIEGame");
    mPIEGame->setMiseAuJeuDelai(1000);

    mInitialised = false;
    mPIEGame->getField()->initialiserXml(xml,false,false);
    {
        if(mPIEGame->getField()->verifierValidite(true,true))
        {
            mInitialised = mPIEGame->getReadyToPlay(false);
            mPIEGame->miseAuJeu(true);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatPIEMode::~SourisEtatPIEMode(void)
///
/// Ce destructeur indique au mod�le de ne plus afficher le rectangle elastique
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatPIEMode::~SourisEtatPIEMode(void)
{
	delete mPIEGame;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatPIEMode::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Action a effectuer lorsqu'une touche du clavier est enfonc�e.
///
/// @param[in]	evenementClavier	: evenement du clavier
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatPIEMode::toucheEnfoncee( EvenementClavier& evenementClavier )
{
    if(!mInitialised)
        return;

	ToucheClavier touche = evenementClavier.obtenirTouche();
    Partie* wGame = mPIEGame;
    checkf(wGame);
    if(wGame)
    {
        NoeudMaillet* maillet = wGame->getField()->getRightMallet();
        checkf(maillet);
        if(maillet)
        {
            // Les 4 cas suivants d�placent le maillet du joueur 2
            if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
                maillet->modifierDirection(true,DIR_HAUT);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
                maillet->modifierDirection(true,DIR_GAUCHE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
                maillet->modifierDirection(true,DIR_DROITE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
                maillet->modifierDirection(true,DIR_BAS);
        }
    }

	// VERIF ETAT PAUSE
	if(touche==VJAK_SHIFT)
    {
		shiftEnfonce_ = true;
    }
    else if(touche == VJAK_SPACE)
    {
        if(wGame)
        {
            wGame->modifierEnPause(!wGame->estEnPause());
        }
    }
	else
    {
		toucheSauvegardee_ = touche;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatPIEMode::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Action a effectuer lorsqu'une touche est relachee.
///
/// @param[in]	evenementClavier	: evenement du clavier
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatPIEMode::toucheRelachee( EvenementClavier& evenementClavier )
{
    if(!mInitialised)
        return;

    ToucheClavier touche = evenementClavier.obtenirTouche();

    if(touche==VJAK_SHIFT)
        shiftEnfonce_ = false;
    if(toucheSauvegardee_==touche)
        toucheSauvegardee_ = 0;

    Partie* wGame = mPIEGame;
    checkf(wGame);
    if(wGame)
    {
        NoeudMaillet* maillet = wGame->getField()->getRightMallet();
        checkf(maillet);
        if(maillet)
        {
            // Les 4 cas suivants d�placent le maillet du joueur 2
            if(touche == ConfigScene::obtenirInstance()->obtenirToucheHaut())
                maillet->modifierDirection(false,DIR_HAUT);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheGauche())
                maillet->modifierDirection(false,DIR_GAUCHE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheDroite())
                maillet->modifierDirection(false,DIR_DROITE);

            if(touche == ConfigScene::obtenirInstance()->obtenirToucheBas())
                maillet->modifierDirection(false,DIR_BAS);
        }
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatPIEMode::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfonc�.
///	Un seul bouton peut �tre enfonc� � la fois.
///
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatPIEMode::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
        mMiddleMousePress = true;
    mMousePos = evenementSouris.obtenirPosition();
    boutonEnfonce_ = evenementSouris.obtenirBouton();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatPIEMode::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relach�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatPIEMode::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_MILIEU)
        mMiddleMousePress = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatPIEMode::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplac�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatPIEMode::sourisDeplacee( EvenementSouris& evenementSouris )
{
    Vecteur3 coordonneesSouris, anciennePos;
    FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX], evenementSouris.obtenirPosition()[VY], coordonneesSouris);

    // VERIF ETAT PAUSE
    if(mMiddleMousePress && boutonEnfonce_==BOUTON_SOURIS_MILIEU)
    {
        Vecteur2i delta = evenementSouris.obtenirPosition()-mMousePos;
        if(shiftEnfonce_)
        {
            FacadeModele::getInstance()->deplacerSouris(delta);

        }
        else
        {
            FacadeModele::getInstance()->orbit(delta);
        }
        mMousePos = evenementSouris.obtenirPosition();
    }
    else
    {
        if(!mInitialised)
            return;

        Partie* wGame = mPIEGame;
        checkf(wGame);
        if(wGame && wGame->getGameStatus() == GAME_STARTED)
        {
            NoeudMaillet* mailletGauche = wGame->getField()->getLeftMallet();
            NoeudMaillet* mailletDroit = wGame->getField()->getRightMallet();
            checkf(mailletGauche && mailletDroit);
            if(mailletGauche && mailletDroit)
            {
                if(!mailletGauche->estControleParNetwork())
                    mailletGauche->setTargetDestination(coordonneesSouris);
                if(!mailletDroit->estControleParNetwork())
                    mailletDroit->setTargetDestination(coordonneesSouris);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatPIEMode::obtenirNomEtatSouris()
///
/// Retourne l'�tat courant.
///
///
/// @return L'�tat courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatPIEMode::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_PIE_MODE;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool SourisEtatPIEMode::OverrideAnimate()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool SourisEtatPIEMode::OverrideAnimate(float temps)
{
    if(!mInitialised)
        return false;

    int tempsMs = (int)temps*1000;

    // VERIF ETAT SOURIS
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


    SoundFMOD::obtenirInstance()->change_song_if_end();
    Partie* wGame = mPIEGame;

    if(wGame)
    {
        wGame->animer(temps);

        if(wGame->partieTerminee())
        {
            wGame->getReadyToPlay(false);
            wGame->miseAuJeu(true);
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool SourisEtatPIEMode::OverrideRender()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool SourisEtatPIEMode::OverrideRender()
{
    if(!mInitialised)
        return false;

    GestionnaireEtatAbstrait::renderBase(mPIEGame->getField(),[&]() -> void{mPIEGame->afficher();});
    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


