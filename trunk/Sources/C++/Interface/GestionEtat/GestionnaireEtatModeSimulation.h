//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeSimulation.h
/// @author Michael Ferris
/// @date 2013-03-03
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef GESTIONNAIREETATMODESIMULATION_H
#define GESTIONNAIREETATMODESIMULATION_H

#include "GestionnaireEtatAbstrait.h"

////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEtatModeSimulation
/// @brief Classe qui g�re les �v�nement l'affichage 
///         et la mise a jours lorsqu'en mode simulation
///
/// @author Michael Ferris
/// @date 2013-03-03
////////////////////////////////////////////////////////////////////////////
class GestionnaireEtatModeSimulation : public GestionnaireEtatAbstrait
{
public:
	/// Constructeur par param�tre.
	GestionnaireEtatModeSimulation();
	/// Destructeur virtuel
	virtual ~GestionnaireEtatModeSimulation();

	/// Comportement pour une touche enfonc�e
	virtual void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Comportement pour une touche relach�e
	virtual void toucheRelachee(EvenementClavier& evenementClavier);
	/// Comportement pour un bouton de souris enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris);
	/// Comportement pour un bouton de souris rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris);
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris);
	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);
	/// Animations pour le mode jeu
	virtual void animer( const float& temps );

	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();
	virtual void miseAJourEvenementsRepetitifs( float deltaTemps );
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////