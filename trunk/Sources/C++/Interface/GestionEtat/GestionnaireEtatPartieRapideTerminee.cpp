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
#include "GestionnaireAnimations.h"
#include "FacadeModele.h"
#include "Partie.h"
#include "SoundFMOD.h"
#include "NoeudMaillet.h"


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
GestionnaireEtatPartieRapideTerminee::GestionnaireEtatPartieRapideTerminee(): GestionnaireEtatAbstrait()
{
	Partie* partie = FacadeModele::getInstance()->obtenirPartieCourante();
	if(partie)
    {
        partie->obtenirGameTime()->pause();

        // Animation qui lance le maillet du perdant dans le feu
        NoeudMaillet* mailletPerdant;
        if(partie->obtenirPositionGagant() == GAGNANT_GAUCHE)
            mailletPerdant = FacadeModele::getInstance()->obtenirMailletJoueurGauche();
        else
            mailletPerdant = FacadeModele::getInstance()->obtenirMailletJoueurDroit();

        if(mailletPerdant)
        {
            Vecteur3 positionDepart = mailletPerdant->getPosition();
            Vecteur3 positionFinale = Vecteur3(-1170, -15, -55);

            vue::Camera* cameraCourante = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();

            // Animation pour le maillet
            AnimationFrame* frame[3];
            frame[0] = new AnimationFrame(2000, positionDepart);
            frame[1] = new AnimationFrame(4000, positionDepart+Vecteur3(0, 0, 120));
            frame[2] = new AnimationFrame(5000, positionFinale);
            Animation* animation = new Animation(LINEAIRE, true, false, false, false);
            for(int i=0; i<3; i++)
                animation->ajouterFrame(frame[i]);
            animation->ajouterObjet(mailletPerdant);
            GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
            Vecteur3 positionDepartCamera = cameraCourante->obtenirPosition();
            // Animation pour la camera
            AnimationFrame* frame2[3];
            frame2[0] = new AnimationFrame(2000, positionDepartCamera, cameraCourante->obtenirPointVise(), cameraCourante->obtenirDirectionHaut());
            frame2[1] = new AnimationFrame(4000, positionDepartCamera+Vecteur3(0, 100, 0), positionDepart+Vecteur3(0, 0, 120), Vecteur3(0, 0, 1));
            frame2[2] = new AnimationFrame(5000, positionDepartCamera+Vecteur3(0, 300, 0),  positionFinale, Vecteur3(0, 0, 1));
            Animation* animation2 = new Animation(LINEAIRE, true, true, true, false);
            for(int i=0; i<3; i++)
                animation2->ajouterFrame(frame2[i]);
            animation2->ajouterObjet(cameraCourante);
            GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation2);
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

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche enfoncée
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatPartieRapideTerminee::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_R || GestionnaireAnimations::obtenirInstance()->estJouerReplay())
		return;
	GestionnaireAnimations::obtenirInstance()->terminerReplay();
	GestionnaireAnimations::obtenirInstance()->viderBufferReplay();
	FacadeModele::getInstance()->reinitialiserPartie();
	FacadeModele::getInstance()->obtenirPartieCourante()->modifierEnPause(false);
	GestionnaireEvenements::modifierEtat(ETAT_MODE_JEU);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatPartieRapideTerminee::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement pour une touche relachée
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
/// Comportement pour un bouton de souris enfoncé
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
/// Comportement pour un bouton de souris relâché
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
/// Comportement lorsque la souris est déplacée
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
