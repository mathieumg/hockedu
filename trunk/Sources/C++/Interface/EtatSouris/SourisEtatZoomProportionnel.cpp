//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatZoomProportionnel.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatZoomProportionnel.h"
#include "FacadeModele.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatZoomProportionnel::SourisEtatZoomProportionnel(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatZoomProportionnel::SourisEtatZoomProportionnel(  )
{
	estEnfoncee_ = false;
	positionPrecedente_ = NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatZoomProportionnel::~SourisEtatZoomProportionnel(void)
///
/// Ce destructeur ne fait rien
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatZoomProportionnel::~SourisEtatZoomProportionnel(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomProportionnel::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncé
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomProportionnel::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// Vérifie si le bouton enfoncé est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concernés
		estEnfoncee_ = true;
		positionPrecedente_ = evenementSouris.obtenirPosition();
	}
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomProportionnel::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée.
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomProportionnel::sourisRelachee( EvenementSouris& evenementSouris )
{
	// Vérifie si le bouton enfoncé est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concernés
		positionPrecedente_ = NULL;
		estEnfoncee_ = false;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomProportionnel::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est deplacée.
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomProportionnel::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Verifie si le bouton est enfoncé
	if(estEnfoncee_)
	{
		// Calcul le déplacement relatif entre depuis le dernier appel de la méthode
		int deplacementY = positionPrecedente_[VY]-evenementSouris.obtenirPosition()[VY];

		// Effectue le zoom
		FacadeModele::obtenirInstance()->zoomDoux(deplacementY);

		
		// Mise à jour de la position
		positionPrecedente_ = evenementSouris.obtenirPosition();
		
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatZoomProportionnel::obtenirNomEtatSouris()
///
/// Retourne l'état courant.
///
///
/// @return L'état courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatZoomProportionnel::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_ZOOM_PROPORTIONNEL;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



