//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatDeplacerFenetre.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatDeplacerFenetre.h"
#include "FacadeModele.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatDeplacerFenetre::SourisEtatDeplacerFenetre(  )
///
/// Ce constructeur appelle le constructeur de base et initialise l'�tat des boutons
/// de la souris � false (non enfonc�s).
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatDeplacerFenetre::SourisEtatDeplacerFenetre(  )
{
	for(int i=0; i<NB_BOUTON_SOURIS; i++)
		estEnfoncee_[i] = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatDeplacerFenetre::~SourisEtatDeplacerFenetre(void)
///
/// Destructeur qui ne fait rien.
///
/// @return Aucune (Destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatDeplacerFenetre::~SourisEtatDeplacerFenetre(void)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatDeplacerFenetre::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfonc�.
///
/// @param[in] EvenementSouris& : evenementSouris.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatDeplacerFenetre::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	dernierePosition_[evenementSouris.obtenirBouton()] = evenementSouris.obtenirPosition();
	estEnfoncee_[evenementSouris.obtenirBouton()] = true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatDeplacerFenetre::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est rel�ch�.
///
/// @param[in] EvenementSouris& : evenementSouris.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatDeplacerFenetre::sourisRelachee( EvenementSouris& evenementSouris )
{
	estEnfoncee_[evenementSouris.obtenirBouton()] = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatDeplacerFenetre::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est d�plac�e.
///
/// @param[in] EvenementSouris& : evenementSouris.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatDeplacerFenetre::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Si le bouton gauche est enfonc�
	if(estEnfoncee_[BOUTON_SOURIS_GAUCHE])
	{
		// Si la souris a �t� d�plac�e
		if(evenementSouris.obtenirPosition() != dernierePosition_[BOUTON_SOURIS_GAUCHE])
		{
			// On d�place la fen�tre et on ajuste les attributs
			FacadeModele::getInstance()->deplacerSouris(evenementSouris.obtenirPosition() - dernierePosition_[BOUTON_SOURIS_GAUCHE]);
			dernierePosition_[BOUTON_SOURIS_GAUCHE] = evenementSouris.obtenirPosition();
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatDeplacerFenetre::obtenirNomEtatSouris()
///
/// Retourne l'�tat actuel.
///
/// @return L'�tat actuel.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatDeplacerFenetre::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_DEPLACER_FENETRE;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


