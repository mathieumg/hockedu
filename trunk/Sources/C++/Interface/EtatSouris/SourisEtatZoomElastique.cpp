//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatZoomElastique.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatZoomElastique.h"
#include "FacadeModele.h"
#include "Vue.h"
#include "CodesEvenements.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatZoomElastique::SourisEtatZoomElastique(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatZoomElastique::SourisEtatZoomElastique(  ):
estEnfoncee_(false)
{
	ctrlEnfoncee_ = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatZoomElastique::~SourisEtatZoomElastique(void)
///
/// Ce destructeur indique au modèle de ne plus afficher le rectangle elastique
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatZoomElastique::~SourisEtatZoomElastique(void)
{
	FacadeModele::getInstance()->modifierVariableZoomElastique(false);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomElastique::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Action a effectuer lorsqu'une touche du clavier est enfoncée.
///
/// @param[in]	evenementClavier	: evenement du clavier
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomElastique::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
	{
		ctrlEnfoncee_ = true;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomElastique::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Action a effectuer lorsqu'une touche est relachee.
///
/// @param[in]	evenementClavier	: evenement du clavier
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomElastique::toucheRelachee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
	{
		ctrlEnfoncee_ = false;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomElastique::sourisEnfoncee( EvenementSouris& evenementSouris )
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
void SourisEtatZoomElastique::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// Vérifie qu'aucun bouton n'est couramment enfoncé
	if(!estEnfoncee_ && evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concernés
		estEnfoncee_ = true;
		boutonEnfonce_ = BOUTON_SOURIS_GAUCHE;
		positionDepart_ = evenementSouris.obtenirPosition();
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomElastique::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomElastique::sourisRelachee( EvenementSouris& evenementSouris )
{
	// Vérifie que le bouton relaché est celui qui fut conservé en mémoire
	if(boutonEnfonce_ == evenementSouris.obtenirBouton())
	{
		// Indique qu'il n'y a plus de bouton enfoncé
		estEnfoncee_ = false;
		// Indique au modèle qu'il ne doit plus faire le rendu du rectangle elastique
		FacadeModele::getInstance()->modifierVariableZoomElastique(estEnfoncee_);

		// Vérification du bouton enfoncé
		if(ctrlEnfoncee_)
			// Si le bouton enfoncé est le bouton droit, on effectue un zoom out 
			FacadeModele::getInstance()->obtenirVue()->zoomerOutElastique( positionDepart_,evenementSouris.obtenirPosition()) ;
		else if(boutonEnfonce_ == BOUTON_SOURIS_GAUCHE)
			// Si le bouton enfoncé est le bouton gauche, on effectue un zoom in
			FacadeModele::getInstance()->obtenirVue()->zoomerInElastique( positionDepart_,evenementSouris.obtenirPosition() );
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomElastique::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplacée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomElastique::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Vérifie si un bouton est enfoncé
	if(estEnfoncee_)
	{
		// Mise à jours du rectangle pour son rendu
		FacadeModele::getInstance()->modifierVariableZoomElastique( estEnfoncee_,positionDepart_,evenementSouris.obtenirPosition() );
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatZoomElastique::obtenirNomEtatSouris()
///
/// Retourne l'état courant.
///
///
/// @return L'état courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatZoomElastique::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_ZOOM_ELASTIQUE;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



