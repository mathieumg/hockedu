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
/// Initialisation des attributs � leur valeur de base
///
/// @param[in] context : R�f�rence du gestionnaire d'�tat de la souris
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
/// Comportement lorsqu'un bouton de la souris est enfonc�
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomProportionnel::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// V�rifie si le bouton enfonc� est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concern�s
		estEnfoncee_ = true;
		positionPrecedente_ = evenementSouris.obtenirPosition();
	}
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomProportionnel::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relach�e.
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomProportionnel::sourisRelachee( EvenementSouris& evenementSouris )
{
	// V�rifie si le bouton enfonc� est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concern�s
		positionPrecedente_ = NULL;
		estEnfoncee_ = false;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatZoomProportionnel::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est deplac�e.
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatZoomProportionnel::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Verifie si le bouton est enfonc�
	if(estEnfoncee_)
	{
		// Calcul le d�placement relatif entre depuis le dernier appel de la m�thode
		int deplacementY = positionPrecedente_[VY]-evenementSouris.obtenirPosition()[VY];

		// Effectue le zoom
		FacadeModele::obtenirInstance()->zoomDoux(deplacementY);

		
		// Mise � jour de la position
		positionPrecedente_ = evenementSouris.obtenirPosition();
		
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatZoomProportionnel::obtenirNomEtatSouris()
///
/// Retourne l'�tat courant.
///
///
/// @return L'�tat courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatZoomProportionnel::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_ZOOM_PROPORTIONNEL;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



