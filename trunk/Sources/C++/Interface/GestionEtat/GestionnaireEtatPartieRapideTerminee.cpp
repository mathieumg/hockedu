//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatPartieRapideTerminee.cpp
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "GestionnaireEtatPartieRapideTerminee.h"
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
/// @fn  GestionnaireEtatPartieRapideTerminee::GestionnaireEtatPartieRapideTerminee( void )
///
/// Constructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatPartieRapideTerminee::GestionnaireEtatPartieRapideTerminee(): GestionnaireEtatAbstrait(), mIdAnimCam(-1), mIdAnimMallet(-1)
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
/// @fn  GestionnaireEtatPartieRapideTerminee::~GestionnaireEtatPartieRapideTerminee( void )
///
/// Destructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatPartieRapideTerminee::~GestionnaireEtatPartieRapideTerminee(void)
{
    ManagerAnimations::obtenirInstance()->DeleteAnimation(mIdAnimCam);
    ManagerAnimations::obtenirInstance()->DeleteAnimation(mIdAnimMallet);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche enfonc�e
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::toucheEnfoncee( EvenementClavier& evenementClavier )
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
/// @fn void GestionnaireEtatPartieRapideTerminee::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche relach�e
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::toucheRelachee( EvenementClavier& evenementClavier )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris enfonc�
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::sourisEnfoncee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement pour un bouton de souris rel�ch�
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::sourisRelachee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est d�plac�e
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::sourisDeplacee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Comportement lorsqu'on bouge la roulette de la souris
///
/// @param[in] EvenementRouletteSouris & evenementRouletteSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::animer( const float& temps )
///
/// Comportement de la methode animer 
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::animer( const float& temps )
{
    SoundFMOD::obtenirInstance()->repeatAppSong();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::afficher()
{
    auto game = FacadeModele::getInstance()->obtenirPartieCourante();
    if(game)
    {
        renderBase(game->getField(),[&]()->void{game->afficher();});
    }
    GestionnaireHUD::obtenirInstance()->dessinerHUDJeu();
}
