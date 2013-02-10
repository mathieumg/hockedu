//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatOrbit.cpp
/// @author Mathieu Parent
/// @date 2012-04-11
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatOrbit.h"
#include "FacadeModele.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatOrbit::SourisEtatOrbit(  )
///
/// Ce constructeur appelle le constructeur de base et initialise l'�tat des boutons
/// de la souris � false (non enfonc�s).
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatOrbit::SourisEtatOrbit(  )
{
	for(int i=0; i<NB_BOUTON_SOURIS; i++)
		estEnfoncee_[i] = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatOrbit::~SourisEtatOrbit(void)
///
/// Destructeur qui ne fait rien.
///
/// @return Aucune (Destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatOrbit::~SourisEtatOrbit(void)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatOrbit::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfonc�.
///
/// @param[in] EvenementSouris& : evenementSouris.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatOrbit::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	dernierePosition_[evenementSouris.obtenirBouton()] = evenementSouris.obtenirPosition();
	estEnfoncee_[evenementSouris.obtenirBouton()] = true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatOrbit::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est rel�ch�.
///
/// @param[in] EvenementSouris& : evenementSouris.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatOrbit::sourisRelachee( EvenementSouris& evenementSouris )
{
	estEnfoncee_[evenementSouris.obtenirBouton()] = false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatOrbit::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est d�plac�e.
///
/// @param[in] EvenementSouris& : evenementSouris.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatOrbit::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Si le bouton gauche est enfonc�
	if(estEnfoncee_[BOUTON_SOURIS_GAUCHE])
	{
		// Si la souris a �t� d�plac�e
		if(evenementSouris.obtenirPosition() != dernierePosition_[BOUTON_SOURIS_GAUCHE])
		{
			// On d�place la fen�tre et on ajuste les attributs
			FacadeModele::getInstance()->orbit(evenementSouris.obtenirPosition() - dernierePosition_[BOUTON_SOURIS_GAUCHE]);
			dernierePosition_[BOUTON_SOURIS_GAUCHE] = evenementSouris.obtenirPosition();
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatOrbit::obtenirNomEtatSouris()
///
/// Retourne l'�tat actuel.
///
/// @return L'�tat actuel.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatOrbit::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_ORBIT;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


