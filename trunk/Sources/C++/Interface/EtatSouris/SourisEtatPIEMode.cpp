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
#include "PlayerHuman.h"
#include "Partie.h"
#include "Terrain.h"
#include "ConfigScene.h"
#include "NoeudMaillet.h"
#include "SoundFMOD.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatPIEMode::SourisEtatPIEMode(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatPIEMode::SourisEtatPIEMode( Vecteur2i& pos ):shiftEnfonce_(false)
{
    shiftEnfonce_ = false;
    mMiddleMousePress = false;

    mPIEGame = new Partie(GAME_TYPE_OFFLINE, Partie::POINTAGE_GAGNANT,SPPlayerHuman(new PlayerHuman("Left Player")),SPPlayerHuman(new PlayerHuman("Right Player")),-1);
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
/// Ce destructeur indique au modèle de ne plus afficher le rectangle elastique
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
/// Action a effectuer lorsqu'une touche du clavier est enfoncée.
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
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatPIEMode::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncé.
///	Un seul bouton peut être enfoncé à la fois.
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
/// Comportement lorsqu'un bouton de la souris est relachée
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
/// Comportement lorsqu'un bouton de la souris est deplacée
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
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatPIEMode::obtenirNomEtatSouris()
///
/// Retourne l'état courant.
///
///
/// @return L'état courant.
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

    SoundFMOD::obtenirInstance()->change_song_if_end();
    Partie* wGame = mPIEGame;

    if(wGame)
    {
        wGame->animer(temps);

        if(wGame->partieTerminee())
        {
            wGame->reinitialiserPartie();
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

    GestionnaireEtatAbstrait::renderBase(mPIEGame->getField(),[this]() -> void{mPIEGame->afficher();});
    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



