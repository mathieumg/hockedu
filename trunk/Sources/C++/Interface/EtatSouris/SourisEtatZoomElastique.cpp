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
/// Initialisation des attributs � leur valeur de base
///
/// @param[in] context : R�f�rence du gestionnaire d'�tat de la souris
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
/// Ce destructeur indique au mod�le de ne plus afficher le rectangle elastique
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
/// Action a effectuer lorsqu'une touche du clavier est enfonc�e.
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
/// Comportement lorsqu'un bouton de la souris est enfonc�.
///	Un seul bouton peut �tre enfonc� � la fois.
///
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomElastique::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// V�rifie qu'aucun bouton n'est couramment enfonc�
	if(!estEnfoncee_ && evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concern�s
		estEnfoncee_ = true;
		boutonEnfonce_ = BOUTON_SOURIS_GAUCHE;
		positionDepart_ = evenementSouris.obtenirPosition();
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomElastique::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relach�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomElastique::sourisRelachee( EvenementSouris& evenementSouris )
{
	// V�rifie que le bouton relach� est celui qui fut conserv� en m�moire
	if(boutonEnfonce_ == evenementSouris.obtenirBouton())
	{
		// Indique qu'il n'y a plus de bouton enfonc�
		estEnfoncee_ = false;
		// Indique au mod�le qu'il ne doit plus faire le rendu du rectangle elastique
		FacadeModele::getInstance()->modifierVariableZoomElastique(estEnfoncee_);

		// V�rification du bouton enfonc�
		if(ctrlEnfoncee_)
			// Si le bouton enfonc� est le bouton droit, on effectue un zoom out 
			FacadeModele::getInstance()->obtenirVue()->zoomerOutElastique( positionDepart_,evenementSouris.obtenirPosition()) ;
		else if(boutonEnfonce_ == BOUTON_SOURIS_GAUCHE)
			// Si le bouton enfonc� est le bouton gauche, on effectue un zoom in
			FacadeModele::getInstance()->obtenirVue()->zoomerInElastique( positionDepart_,evenementSouris.obtenirPosition() );
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomElastique::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplac�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomElastique::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// V�rifie si un bouton est enfonc�
	if(estEnfoncee_)
	{
		// Mise � jours du rectangle pour son rendu
		FacadeModele::getInstance()->modifierVariableZoomElastique( estEnfoncee_,positionDepart_,evenementSouris.obtenirPosition() );
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatZoomElastique::obtenirNomEtatSouris()
///
/// Retourne l'�tat courant.
///
///
/// @return L'�tat courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatZoomElastique::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_ZOOM_ELASTIQUE;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



