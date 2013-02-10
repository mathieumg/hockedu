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
/// Ce constructeur appelle le constructeur de base et initialise l'état des boutons
/// de la souris à false (non enfoncés).
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
/// Comportement lorsqu'un bouton de la souris est enfoncé.
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
/// Comportement lorsqu'un bouton de la souris est relâché.
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
/// Comportement lorsque la souris est déplacée.
///
/// @param[in] EvenementSouris& : evenementSouris.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatOrbit::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Si le bouton gauche est enfoncé
	if(estEnfoncee_[BOUTON_SOURIS_GAUCHE])
	{
		// Si la souris a été déplacée
		if(evenementSouris.obtenirPosition() != dernierePosition_[BOUTON_SOURIS_GAUCHE])
		{
			// On déplace la fenêtre et on ajuste les attributs
			FacadeModele::getInstance()->orbit(evenementSouris.obtenirPosition() - dernierePosition_[BOUTON_SOURIS_GAUCHE]);
			dernierePosition_[BOUTON_SOURIS_GAUCHE] = evenementSouris.obtenirPosition();
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatOrbit::obtenirNomEtatSouris()
///
/// Retourne l'état actuel.
///
/// @return L'état actuel.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatOrbit::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_ORBIT;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


