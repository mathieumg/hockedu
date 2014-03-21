//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerSimulation.cpp
/// @author Michael Ferris
/// @date 2013-03-03
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "GameControllerSimulation.h"
#include "FacadeModele.h"
#include "ConfigScene.h"

#include "Partie.h"
#include "Terrain.h"
#include "GestionnaireHUD.h"
#include "GameControllerAbstract.h"
#include "SourisEtatAbstrait.h"
#include "SoundFMOD.h"
#include "PlayerComputer.h"

#include "Vue.h"
#include "Projection.h"
#include "ConfigScene.h"
#include "BoiteEnvironnement.h"
#if BOX2D_DEBUG
#include "Box2D\Dynamics\b2World.h"
#endif
#include "VuePerspectiveSplit.h"
#include "GameManager.h"




////////////////////////////////////////////////////////////////////////
///
/// @fn  GameControllerSimulation::GameControllerSimulation( GestionnaireEvenements* contexte )
///
/// Constructeur qui initialise les valeurs requises pour faire fonctionner le gestionnaire. 
/// Place l'état par défaut à Sélection.
///
/// @param[in] GestionnaireEvenements * contexte
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameControllerSimulation::GameControllerSimulation( ):
GameControllerAbstract()
{
	modifierEtatSouris(ETAT_SOURIS_AUCUN);
    
    // vue split
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    FacadeModele::getInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

    vue::Vue* nouvelleVue = new vue::VuePerspectiveSplit(
        vue::Camera(Vecteur3(-200.0, 0.0, 75.0), Vecteur3(0, 0, 0),
        Vecteur3(0, 0, 1)),
        0, 200, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
        -150, 150, -150, 150);
    nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));

    FacadeModele::getInstance()->modifierVue(nouvelleVue);

    for(int i=0; i<2; ++i)
    {
        int partieCourante = GameManager::obtenirInstance()->addNewGame(GAME_TYPE_OFFLINE, Partie::POINTAGE_GAGNANT,SPPlayerAbstract(new PlayerComputer("Left Player",225,50)),SPPlayerAbstract(new PlayerComputer("Joueur Droit",225,50)));
        auto pointeurPartie = GameManager::obtenirInstance()->getGame(partieCourante);
        if(i==0)
        {
            pointeurPartie->setFieldName(FacadeModele::FICHIER_TERRAIN_EN_COURS);
        }
        pointeurPartie->getReadyToPlay();
        pointeurPartie->miseAuJeu(true);

        FacadeModele::getInstance()->obtenirVue()->centrerCamera(pointeurPartie->getField()->GetTableWidth(),i+1);
        games[i] = partieCourante;
    }


}

////////////////////////////////////////////////////////////////////////
///
/// @fn  GameControllerSimulation::~GameControllerSimulation()
///
/// Destructeur virtuel
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
GameControllerSimulation::~GameControllerSimulation()
{
    // remettre la vue par defaut
    FacadeModele::getInstance()->initialiserVue();
    
    for(auto it = games.begin();it != games.end(); ++it)
    {
        GameManager::obtenirInstance()->removeGame(*it);
    }
    games.clear();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés à l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::toucheEnfoncee(EvenementClavier& evenementClavier)
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés au relâchement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::toucheRelachee( EvenementClavier& evenementClavier )
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés à l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisEnfoncee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au relâchement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::sourisRelachee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisRelachee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::sourisDeplacee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au déplacement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::sourisDeplacee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisDeplacee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris)
///
/// Fonction qui gère les événements liés au déplacement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	GameControllerAbstract::rouletteSouris(evenementRouletteSouris);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::animer( const float& temps )
///
/// Animation du mode jeu.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->change_song_if_end();

    for(auto it = games.begin();it != games.end(); ++it)
    {
        Partie* partieCourante = GameManager::obtenirInstance()->getGame(*it);
        if(partieCourante)
        {
            partieCourante->animer(temps);
            partieCourante->updateMinuterie((int)(temps*1000));
            gestionAnimationEnJeu(partieCourante, temps);
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerSimulation::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerSimulation::afficher()
{
    auto vue = FacadeModele::getInstance()->obtenirVue();
    if(vue)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  );
        const int nbViewPort = vue->obtenirNbViewports();
        auto it = games.begin();
        for(int currentCam=1; currentCam <= nbViewPort && it != games.end(); ++currentCam, ++it)
        {
            Partie* partieCourante = GameManager::obtenirInstance()->getGame(*it);
            if(partieCourante)
            {
                glEnable(GL_LIGHTING);
                vue->appliquerVue(currentCam);

                // ICI APPLIQUER LES PATENTE DE SHADER
                FacadeModele::getInstance()->ActivateShaders();
                FacadeModele::getInstance()->getBoiteEnvironnement()->afficher(vue->obtenirCamera(currentCam).obtenirPosition(),5000.0);

                // On rafraichi la lumière
                ConfigScene::obtenirInstance()->rafraichirLumiere();

                partieCourante->afficher();

                FacadeModele::getInstance()->DeActivateShaders();
                glDisable(GL_LIGHTING);

    #if BOX2D_DEBUG
                glPushMatrix();
                glPushAttrib(GL_ALL_ATTRIB_BITS);

                auto world = partieCourante->getField()->GetWorld();
                if(world)
                {
                    world->DrawDebugData();
                }
                // Restauration de la matrice.
                glPopAttrib();
                glPopMatrix();
    #endif
            }
        }
        if(nbViewPort>1)
        {
            vue->obtenirProjection().mettreAJourCloture();
        }
    }
}

void GameControllerSimulation::miseAJourEvenementsRepetitifs( float deltaTemps )
{

}









///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




