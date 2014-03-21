//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerQuickPlayEnd.cpp
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "GameControllerQuickPlayEnd.h"
#include "GestionnaireHUD.h"

#include "FacadeModele.h"
#include "Partie.h"
#include "SoundFMOD.h"
#include "NoeudMaillet.h"
#include "GestionnaireReseau.h"
#include "PlayerAbstract.h"
#include "ManagerAnimations.h"
#include "AnimationCamera.h"
#include "Camera.h"
#include "Vue.h"
#include "AnimationNodePosition.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  GameControllerQuickPlayEnd::GameControllerQuickPlayEnd( void )
///
/// Constructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameControllerQuickPlayEnd::GameControllerQuickPlayEnd(): GameControllerAbstract(), mIdAnimCam(-1), mIdAnimMallet(-1)
{
    Partie* partie = FacadeModele::getInstance()->obtenirPartieCourante();
    if(partie)
    {
        partie->obtenirGameTime()->pause();
        
        // Animation qui lance le maillet du perdant dans le feu
        NoeudMaillet* mailletPerdant = NULL;
        SPPlayerAbstract looser;
        if(partie->obtenirPositionGagant() != GAGNANT_GAUCHE)
            looser = partie->obtenirJoueurGauche();
        else
            looser = partie->obtenirJoueurDroit();

        if(looser)
        {
            mailletPerdant = looser->getControlingMallet();

            if(mailletPerdant)
            {
                Vecteur3 positionDepart = mailletPerdant->getPosition();
                Vecteur3 positionFinale = Vecteur3(-1170, -15, -55);

                vue::Camera* cameraCourante = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();

                {
                    // Animation pour le maillet
                    auto anim = ManagerAnimations::obtenirInstance()->CreateAnimation<AnimationNodePosition>();
                    mIdAnimMallet = anim.first;
                    AnimationNodePosition* malletAnim = anim.second;
                    malletAnim->AddFrame(2,positionDepart);
                    malletAnim->AddFrame(4,positionDepart+Vecteur3(0, 0, 120));
                    malletAnim->AddFrame(5,positionFinale);
                    malletAnim->SetAlgo(ANIMATION_LINEAR);
                    malletAnim->SetNode(mailletPerdant);
                }

                {
                    Vecteur3 positionDepartCamera = cameraCourante->obtenirPosition();
                    // Animation pour la camera
                    auto anim = ManagerAnimations::obtenirInstance()->CreateAnimation<AnimationCamera>();
                    mIdAnimCam = anim.first;
                    AnimationCamera* animation = anim.second;
                    animation->AddFrame(2, positionDepartCamera, cameraCourante->obtenirPointVise(), cameraCourante->obtenirDirectionHaut());
                    animation->AddFrame(4, positionDepartCamera+Vecteur3(0, 100, 0), positionDepart+Vecteur3(0, 0, 120), Vecteur3(0, 0, 1));
                    animation->AddFrame(5, positionDepartCamera+Vecteur3(0, 300, 0),  positionFinale, Vecteur3(0, 0, 1));

                    animation->SetCamera(cameraCourante);
                    animation->SetAlgo(ANIMATION_LINEAR);
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GameControllerQuickPlayEnd::~GameControllerQuickPlayEnd( void )
///
/// Destructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GameControllerQuickPlayEnd::~GameControllerQuickPlayEnd(void)
{
    ManagerAnimations::obtenirInstance()->DeleteAnimation(mIdAnimCam);
    ManagerAnimations::obtenirInstance()->DeleteAnimation(mIdAnimMallet);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche enfoncée
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::toucheEnfoncee( EvenementClavier& evenementClavier )
{
    if(evenementClavier.obtenirTouche() == VJAK_R || evenementClavier.obtenirTouche() == VJAK_CONTROL /*|| GestionnaireAnimations::obtenirInstance()->estJouerReplay()*/)
        return;
// 	GestionnaireAnimations::obtenirInstance()->terminerReplay();
// 	GestionnaireAnimations::obtenirInstance()->viderBufferReplay();
    FacadeModele::getInstance()->reinitialiserPartie();
    FacadeModele::getInstance()->obtenirPartieCourante()->modifierEnPause(false);
    if(FacadeModele::getInstance()->obtenirPartieCourante()->isNetworkClientGame())
    {
        GestionnaireReseau::obtenirInstance()->transmitEvent(EXIT_NETWORK_GAME);
    }
    EventManager::modifierEtat(ETAT_MODE_JEU);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche relachée
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::toucheRelachee( EvenementClavier& evenementClavier )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris enfoncé
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::sourisEnfoncee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris relâché
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::sourisRelachee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est déplacée
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::sourisDeplacee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Comportement lorsqu'on bouge la roulette de la souris
///
/// @param[in] EvenementRouletteSouris & evenementRouletteSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::animer( const float& temps )
///
/// Comportement de la methode animer 
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::animer( const float& temps )
{
    SoundFMOD::obtenirInstance()->repeatAppSong();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GameControllerQuickPlayEnd::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GameControllerQuickPlayEnd::afficher()
{
    auto game = FacadeModele::getInstance()->obtenirPartieCourante();
    if(game)
    {
        renderBase(game->getField(),[&]()->void{game->afficher();});
    }
    GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
}
