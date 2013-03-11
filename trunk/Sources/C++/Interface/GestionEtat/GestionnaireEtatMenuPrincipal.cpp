//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatMenuPrincipal.cpp
/// @author Vincent Lemire
/// @date 2012-03-02
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireEtatMenuPrincipal.h"
#include "SoundFMOD.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireEtatMenuPrincipal::GestionnaireEtatMenuPrincipal(GestionnaireEvenements* contexte) : GestionnaireEtatAbstrait(contexte)
///
/// Constructeur qui initialise le contexte et mets un �v�nement de souris par d�faut.
///
/// @param[in] touche : Le nombre de coches parcouru sur la roulette.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
GestionnaireEtatMenuPrincipal::GestionnaireEtatMenuPrincipal() : GestionnaireEtatAbstrait()
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatMenuPrincipal::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Fonction qui g�re les �v�nements li�s � l'enfoncement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatMenuPrincipal::toucheEnfoncee( EvenementClavier& evenementClavier )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatMenuPrincipal::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Fonction qui g�re les �v�nements li�s au rel�chement d'une touche.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatMenuPrincipal::toucheRelachee( EvenementClavier& evenementClavier )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatMenuPrincipal::sourisEnfoncee(EvenementSouris& evenementSouris)
///
/// Fonction qui g�re les �v�nements li�s � l'enfoncement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatMenuPrincipal::sourisEnfoncee( EvenementSouris& evenementSouris )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatMenuPrincipal::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Fonction qui g�re les �v�nements li�s au rel�chement d'un bouton de la souris.
///
/// @param[in] evenementClavier : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatMenuPrincipal::sourisRelachee( EvenementSouris& evenementSouris )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatMenuPrincipal::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Fonction qui g�re les �v�nements li�s au d�placement d'un bouton de la souris.
///
/// @param[in] evenementSouris : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatMenuPrincipal::sourisDeplacee( EvenementSouris& evenementSouris )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatMenuPrincipal::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
///
/// Fonction qui g�re les �v�nements li�s au d�placement de la roulette de la souris
///
/// @param[in] evenementRouletteSouris : L'objet correspondant � l'�v�nement.
///
/// @return Aucune.
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GestionnaireEtatMenuPrincipal::rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireEtatMenuPrincipal::animer( const float& temps )
///
/// Animations du menu principal.
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireEtatMenuPrincipal::animer( const float& temps )
{
	SoundFMOD::obtenirInstance()->repeatAppSong();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
