//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeSimulation.cpp
/// @author Michael Ferris
/// @date 2013-03-03
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEtatModeSimulation.h"
#include "FacadeModele.h"
#include "NoeudMaillet.h"
#include "ConfigScene.h"
#include "GestionnaireAnimations.h"
#include "Partie.h"
#include "Terrain.h"
#include "GestionnaireHUD.h"
#include "GestionnaireEtatAbstrait.h"
#include "SourisEtatAbstrait.h"
#include "SoundFMOD.h"
#include "JoueurVirtuel.h"

#include "Vue.h"
#include "Projection.h"
#include "ConfigScene.h"
#include "BoiteEnvironnement.h"
#if BOX2D_DEBUG
#include "Box2D\Dynamics\b2World.h"
#endif
#include "VuePerspectiveSplit.h"

typedef int IDPartie;
typedef std::hash_map<IDPartie,Partie*> GamesContainer;
GamesContainer games;

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireEtatModeSimulation::GestionnaireEtatModeSimulation( GestionnaireEvenements* contexte )
///
/// Constructeur qui initialise les valeurs requises pour faire fonctionner le gestionnaire. 
/// Place l'état par défaut à Sélection.
///
/// @param[in] GestionnaireEvenements * contexte
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeSimulation::GestionnaireEtatModeSimulation( ):
GestionnaireEtatAbstrait()
{
	modifierEtatSouris(ETAT_SOURIS_AUCUN);
    
    // vue split
    int xMinCourant, yMinCourant, xMaxCourant, yMaxCourant;
    FacadeModele::getInstance()->obtenirVue()->obtenirProjection().obtenirCoordonneesCloture(xMinCourant, xMaxCourant, yMinCourant, yMaxCourant);

    vue::Vue* nouvelleVue = new vue::VuePerspectiveSplit(
        vue::Camera(Vecteur3(-200.0, 0.0, 75.0), Vecteur3(0, 0, 0),
        Vecteur3(0, 0, 1)  , Vecteur3(0, 1, 0)),
        0, 200, 0, 400,
        180, 50000, /*ZoomInMax*/10, /*ZoomOutMax*/15000, 1.25,
        -150, 150, -150, 150);
    nouvelleVue->redimensionnerFenetre(Vecteur2i(xMinCourant, yMinCourant), Vecteur2i(xMaxCourant, yMaxCourant));

    FacadeModele::getInstance()->modifierVue(nouvelleVue);

    for(int i=0; i<2; ++i)
    {
        auto partieCourante = new Partie(SPJoueurAbstrait(new JoueurVirtuel("Joueur Gauche",225,50)),SPJoueurAbstrait(new JoueurVirtuel("Joueur Droit",225,50)));
        if(i==0)partieCourante->setFieldName(FacadeModele::FICHIER_TERRAIN_EN_COURS);
        partieCourante->getReadyToPlay();
        partieCourante->miseAuJeu(true);

        FacadeModele::getInstance()->obtenirVue()->centrerCamera(partieCourante->getField()->GetTableWidth(),i+1);
        games[i] = partieCourante;
    }


}

////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireEtatModeSimulation::~GestionnaireEtatModeSimulation()
///
/// Destructeur virtuel
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
GestionnaireEtatModeSimulation::~GestionnaireEtatModeSimulation()
{
    // remettre la vue par defaut
    FacadeModele::getInstance()->initialiserVue();

    for(auto it = games.begin();it != games.end(); ++it)
    {
        delete (it->second);
    }
    games.clear();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeSimulation::toucheEnfoncee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés à l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::toucheEnfoncee(EvenementClavier& evenementClavier)
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeSimulation::toucheRelachee(EvenementClavier& evenementClavier)
///
/// Fonction qui gère les événements liés au relâchement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::toucheRelachee( EvenementClavier& evenementClavier )
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeSimulation::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés à l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisEnfoncee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeSimulation::sourisRelachee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au relâchement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::sourisRelachee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisRelachee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeSimulation::sourisDeplacee(EvenementSouris& evenementSouris)
///
/// Fonction qui gère les événements liés au déplacement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::sourisDeplacee( EvenementSouris& evenementSouris )
{
    if (etatSouris_)
    {
        etatSouris_->sourisDeplacee(evenementSouris);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeSimulation::rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris)
///
/// Fonction qui gère les événements liés au déplacement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant à l'événement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	GestionnaireEtatAbstrait::rouletteSouris(evenementRouletteSouris);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatModeSimulation::animer( const float& temps )
///
/// Animation du mode jeu.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->change_song_if_end();

    for(auto it = games.begin();it != games.end(); ++it)
    {
        Partie* partieCourante = it->second;
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
/// @fn void GestionnaireEtatModeSimulation::afficher()
///
/// Permet d'effectuer l'affichage specifique a l'etat
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatModeSimulation::afficher()
{
    auto vue = FacadeModele::getInstance()->obtenirVue();
    if(vue)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        const int nbViewPort = vue->obtenirNbViewports();
        auto it = games.begin();
        for(int currentCam=1; currentCam <= nbViewPort && it != games.end(); ++currentCam, ++it)
        {
            Partie* partieCourante = it->second;
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

void GestionnaireEtatModeSimulation::miseAJourEvenementsRepetitifs( float deltaTemps )
{
// 	int tempsMs = (int)deltaTemps*1000;
// 
// 	// VERIF ETAT SOURIS
// 	switch(toucheSauvegardee_)
// 	{
// 	case VJAK_UP:
// 		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(0, -tempsMs));
// 		break;
// 	case VJAK_DOWN:
// 		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(0, tempsMs));
// 		break;
// 	case VJAK_LEFT:
// 		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(-tempsMs, 0));
// 		break;
// 	case VJAK_RIGHT:
// 		FacadeModele::getInstance()->deplacerFleches(Vecteur2i(tempsMs, 0));
// 		break;
// 	case VJAK_ADD:
// 	case VJAK_PLUS:
// 		// Utilisation temporaire de la méthode pour le zooom associé à la roulette de la souris
// 		// -1 indique que c'est un zoomIn
// 		FacadeModele::getInstance()->zoom(-tempsMs);
// 		break;
// 
// 	case VJAK_SUBTRACT:
// 	case VJAK_MINUS:
// 		// Utilisation temporaire de la méthode pour le zooom associé à la roulette de la souris
// 		// 1 indique que c'est un zoomOut
// 		FacadeModele::getInstance()->zoom(tempsMs);
// 		break;
// 
// 	default:
// 		break;
// 	}
}









///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




