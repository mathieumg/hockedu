//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatAbstrait.cpp
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEtatAbstrait.h"
#include "FacadeModele.h"
#include "SourisEtatDeplacerFenetre.h"
#include "SourisEtatOrbit.h"
#include "SourisEtatZoomElastique.h"
#include "SourisEtatZoomProportionnel.h"
#include "GestionnaireAnimations.h"
#include "Partie.h"
#include "NoeudRondelle.h"
#include "NoeudMaillet.h"
#include "Terrain.h"

#include "Vue.h"
#include "Projection.h"
#include "ConfigScene.h"
#include "BoiteEnvironnement.h"
#if BOX2D_DEBUG
#include "Box2D\Dynamics\b2World.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatAbstrait::GestionnaireEtatAbstrait(GestionnaireEvenements* contexte)
///
/// Constructeur. Initialise le pointeur sur l'état de la souris et le contexte.
///
/// @param contexte : Le contexte auquel ce gestionnaire d'état appartient.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatAbstrait::GestionnaireEtatAbstrait() :etatSouris_(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatAbstrait::~GestionnaireEtatAbstrait()
///
/// Destructeur de la classe; Désalloue la mémoire allouée pour l'état de souris.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatAbstrait::~GestionnaireEtatAbstrait()
{
	if(etatSouris_ != NULL)
		delete etatSouris_;
	etatSouris_ = NULL;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatAbstrait::clearMouseState( NomEtatSouris newState )
///
/// clear mouse state memory if the newState is different from current
///
/// @param[in] NomEtatSouris newState
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatAbstrait::clearMouseState( NomEtatSouris newState )
{
    if((unsigned int)newState < NB_ETATS_SOURIS)
    {
        // S'il existe déjà un état de souris on vérifie si on le remplace
        if(etatSouris_ != NULL)
            if(etatSouris_->obtenirNomEtatSouris() == newState)
                return;
            else
                delete etatSouris_;
        etatSouris_ = NULL;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatAbstrait::modifierEtatSouris( NomEtatSouris etatSouris )
///
/// /*Description*/
///
/// @param[in] NomEtatSouris etatSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatAbstrait::modifierEtatSouris( NomEtatSouris etatSouris )
{
	clearMouseState(etatSouris);
    if(!etatSouris_)
    {
        switch(etatSouris)
        {
        case ETAT_SOURIS_DEPLACER_FENETRE			: etatSouris_ = new SourisEtatDeplacerFenetre			();	break;
        case ETAT_SOURIS_ZOOM_ELASTIQUE				: etatSouris_ = new SourisEtatZoomElastique				();	break;
        case ETAT_SOURIS_ZOOM_PROPORTIONNEL			: etatSouris_ = new SourisEtatZoomProportionnel			();	break;
        case ETAT_SOURIS_ORBIT						: etatSouris_ = new SourisEtatOrbit                     ();	break;
        default: break;
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool GestionnaireEtatAbstrait::GetMouseStateName( NomEtatSouris& outName )
///
// retourne vrai si on arrive a retrouver un nom valid
///
/// @param[in] NomEtatSouris & outName
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool GestionnaireEtatAbstrait::GetMouseStateName( NomEtatSouris& outName )
{
    bool nameFound = true;
    if(etatSouris_)
    {
        outName = etatSouris_->obtenirNomEtatSouris();
    }
    else
    {
        nameFound = false;
    }
    return nameFound;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatAbstrait::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// /*Description*/
///
/// @param[in] EvenementRouletteSouris & evenementRouletteSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatAbstrait::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
	FacadeModele::getInstance()->zoom(evenementRouletteSouris.obtenirNbCoches());
}



void GestionnaireEtatAbstrait::miseAJourEvenementsRepetitifs( float deltaTemps )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatAbstrait::gestionAnimationEnJeu( Partie* partieCourante, const float& temps )
///
/// /*Description*/
///
/// @param[in] Partie * partieCourante
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatAbstrait::gestionAnimationEnJeu( Partie* partieCourante, const float& temps )
{
	if(!GestionnaireAnimations::obtenirInstance()->estJouerReplay())
	{
		vue::Camera* camera = &FacadeModele::getInstance()->obtenirVue()->obtenirCamera();
		NoeudMaillet* maillet1 = partieCourante->getField()->getLeftMallet();
		NoeudMaillet* maillet2 = partieCourante->getField()->getRightMallet();
		NoeudRondelle* rondelle = partieCourante->getField()->getPuck();

		GestionnaireAnimations* gestionnaire = GestionnaireAnimations::obtenirInstance();
		if(GestionnaireAnimations::obtenirInstance()->replayEstTermine())
		{
			vue::Camera ancienneCamera = GestionnaireAnimations::obtenirInstance()->obtenirAncienneCamera();

			AnimationFrame* frame[2];
			frame[0] = new AnimationFrame(0, camera->obtenirPosition(), camera->obtenirPointVise(), camera->obtenirDirectionHaut());
			frame[1] = new AnimationFrame(500, ancienneCamera.obtenirPosition(), ancienneCamera.obtenirPointVise(), ancienneCamera.obtenirDirectionHaut());

			Animation* animation = new Animation(BEZIER, true, true, true);
			for(int i=0; i<2; i++)
				animation->ajouterFrame(frame[i]);
			animation->ajouterObjet(camera);
			GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);

			if(partieCourante)
				partieCourante->delais(4100);
		}

		std::vector<AnimationRepriseFrame*> listeAnimationFrame;
		if(camera)
		{
			Vecteur3 pointVise =  camera->obtenirPointVise();
			Vecteur3 directionHaut = camera->obtenirDirectionHaut();
			if(rondelle!=0)
			{
				pointVise = rondelle->getPosition();
				//directionHaut = 
			}
			AnimationFrame* animationFrameCamera = new AnimationFrame(temps*1000, camera->obtenirPosition(), pointVise, directionHaut);
			AnimationRepriseFrame* animationRepriseFrameCamera = new AnimationRepriseFrame(animationFrameCamera, camera);

			listeAnimationFrame.push_back(animationRepriseFrameCamera);
		}
		if(maillet1)
		{
			AnimationFrame* animationFrameMaillet1 = new AnimationFrame(temps*1000, maillet1->getPosition(), Vecteur3(0, 0, maillet1->obtenirAngle()));
			AnimationRepriseFrame* animationRepriseFrameMaillet1 = new AnimationRepriseFrame(animationFrameMaillet1, maillet1);

			listeAnimationFrame.push_back(animationRepriseFrameMaillet1);
		}
		if(maillet2)
		{
			AnimationFrame* animationFrameMaillet2 = new AnimationFrame(temps*1000, maillet2->getPosition(), Vecteur3(0, 0, maillet2->obtenirAngle()));
			AnimationRepriseFrame* animationRepriseFrameMaillet2 = new AnimationRepriseFrame(animationFrameMaillet2, maillet2);

			listeAnimationFrame.push_back(animationRepriseFrameMaillet2);
		}
		if(rondelle)
		{
			AnimationFrame* animationFrameRondelle = new AnimationFrame(temps*1000, rondelle->getPosition(), Vecteur3(0, 0, rondelle->obtenirAngle()));
			AnimationRepriseFrame* animationRepriseFrameRondelle = new AnimationRepriseFrame(animationFrameRondelle, rondelle);

			listeAnimationFrame.push_back(animationRepriseFrameRondelle);
		}
		IterationReplay* iterationReplay = new IterationReplay(temps*1000, listeAnimationFrame);
		GestionnaireAnimations::obtenirInstance()->saveReplayFrame(iterationReplay);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatAbstrait::renderBase( class Terrain* pField, RenderSpecific /*= NULL*/ )
///
/// /*Description*/
///
/// @param[in] class Terrain * pField
/// @param[in] RenderSpecific
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatAbstrait::renderBase( Terrain* pField, RenderSpecific pSpecificRender/*= NULL*/ )
{
    auto vue = FacadeModele::getInstance()->obtenirVue();
    if(vue)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        const int nbViewPort = vue->obtenirNbViewports();
        for(int currentCam=1; currentCam <= nbViewPort; currentCam++)
        {
            glEnable(GL_LIGHTING);
            vue->appliquerVue(currentCam);

            // ICI APPLIQUER LES PATENTE DE SHADER
            FacadeModele::getInstance()->ActivateShaders();
            FacadeModele::getInstance()->getBoiteEnvironnement()->afficher(vue->obtenirCamera(currentCam).obtenirPosition(),5000.0);

            // On rafraichi la lumière
            ConfigScene::obtenirInstance()->rafraichirLumiere();

            if(pSpecificRender)
            {
                pSpecificRender();
            }
            else
            {
                pField->renderField();
            }

            FacadeModele::getInstance()->DeActivateShaders();
            glDisable(GL_LIGHTING);

#if BOX2D_DEBUG
            glPushMatrix();
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            auto world = pField->GetWorld();
            if(world)
            {
                world->DrawDebugData();
            }
            // Restauration de la matrice.
            glPopAttrib();
            glPopMatrix();
#endif
        }
        if(nbViewPort>1)
        {
            vue->obtenirProjection().mettreAJourCloture();
        }
    }
}








///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



