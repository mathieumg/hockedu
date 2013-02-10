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
#include "SourisEtatTransformationRotation.h"
#include "SourisEtatTransformationEchelle.h"
#include "SourisEtatTransformationDeplacement.h"
#include "SourisEtatSelection.h"
#include "SourisEtatAjoutMuret.h"
#include "SourisEtatAjout.h"
#include "GestionnaireAnimations.h"
#include "Partie.h"
#include "NoeudRondelle.h"
#include "NoeudMaillet.h"

HANDLE mutexMouseState;

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
GestionnaireEtatAbstrait::GestionnaireEtatAbstrait(GestionnaireEvenements* contexte) :etatSouris_(0),contexte_(contexte)
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
	if(etatSouris < 0 || etatSouris >= NB_ETATS_SOURIS)
		return;

	// S'il existe déjà un état de souris on vérifie si on le remplace
	if(etatSouris_ != NULL)
		if(etatSouris_->obtenirNomEtatSouris() == etatSouris)
			return;
		else
			delete etatSouris_;
    etatSouris_ = NULL;
	switch(etatSouris)
	{
		case ETAT_SOURIS_DEPLACER_FENETRE			: etatSouris_ = new SourisEtatDeplacerFenetre			();	break;
		case ETAT_SOURIS_ZOOM_ELASTIQUE				: etatSouris_ = new SourisEtatZoomElastique				();	break;
		case ETAT_SOURIS_ZOOM_PROPORTIONNEL			: etatSouris_ = new SourisEtatZoomProportionnel			();	break;
		case ETAT_SOURIS_TRANSFORMATION_ROTATION	: etatSouris_ = new SourisEtatTransformationRotation	();	break;
		case ETAT_SOURIS_TRANSFORMATION_ECHELLE		: etatSouris_ = new SourisEtatTransformationEchelle		();	break;
		case ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT	: etatSouris_ = new SourisEtatTransformationDeplacement	();	break;
		case ETAT_SOURIS_SELECTION					: etatSouris_ = new SourisEtatSelection					();	break;
		case ETAT_SOURIS_AJOUTER_PORTAIL			: etatSouris_ = new SourisEtatAjout						(RazerGameUtilities::NOM_PORTAIL);	break;
		case ETAT_SOURIS_AJOUTER_MURET				: etatSouris_ = new SourisEtatAjoutMuret				();	break;
		case ETAT_SOURIS_AJOUTER_MAILLET			: etatSouris_ = new SourisEtatAjout						(RazerGameUtilities::NOM_MAILLET);	break;
		case ETAT_SOURIS_AJOUTER_RONDELLE			: etatSouris_ = new SourisEtatAjout						(RazerGameUtilities::NOM_RONDELLE);	break;
		case ETAT_SOURIS_AJOUTER_ACCELERATEUR		: etatSouris_ = new SourisEtatAjout						(RazerGameUtilities::NOM_ACCELERATEUR);	break;
		case ETAT_SOURIS_ORBIT						: etatSouris_ = new SourisEtatOrbit						();	break;
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
		NoeudMaillet* maillet1 = FacadeModele::getInstance()->obtenirMailletJoueurGauche();
		NoeudMaillet* maillet2 = FacadeModele::getInstance()->obtenirMailletJoueurDroit();
		NoeudRondelle* rondelle = FacadeModele::getInstance()->obtenirRondelle();

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
				pointVise = rondelle->obtenirPositionAbsolue();
				//directionHaut = 
			}
			AnimationFrame* animationFrameCamera = new AnimationFrame(temps*1000, camera->obtenirPosition(), pointVise, directionHaut);
			AnimationRepriseFrame* animationRepriseFrameCamera = new AnimationRepriseFrame(animationFrameCamera, camera);

			listeAnimationFrame.push_back(animationRepriseFrameCamera);
		}
		if(maillet1)
		{
			AnimationFrame* animationFrameMaillet1 = new AnimationFrame(temps*1000, maillet1->obtenirPositionRelative(), Vecteur3(0, 0, maillet1->obtenirAngle()));
			AnimationRepriseFrame* animationRepriseFrameMaillet1 = new AnimationRepriseFrame(animationFrameMaillet1, maillet1);

			listeAnimationFrame.push_back(animationRepriseFrameMaillet1);
		}
		if(maillet2)
		{
			AnimationFrame* animationFrameMaillet2 = new AnimationFrame(temps*1000, maillet2->obtenirPositionRelative(), Vecteur3(0, 0, maillet2->obtenirAngle()));
			AnimationRepriseFrame* animationRepriseFrameMaillet2 = new AnimationRepriseFrame(animationFrameMaillet2, maillet2);

			listeAnimationFrame.push_back(animationRepriseFrameMaillet2);
		}
		if(rondelle)
		{
			AnimationFrame* animationFrameRondelle = new AnimationFrame(temps*1000, rondelle->obtenirPositionRelative(), Vecteur3(0, 0, rondelle->obtenirAngle()));
			AnimationRepriseFrame* animationRepriseFrameRondelle = new AnimationRepriseFrame(animationFrameRondelle, rondelle);

			listeAnimationFrame.push_back(animationRepriseFrameRondelle);
		}
		IterationReplay* iterationReplay = new IterationReplay(temps*1000, listeAnimationFrame);
		GestionnaireAnimations::obtenirInstance()->saveReplayFrame(iterationReplay);
	}
}






///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



