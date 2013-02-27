//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeJeu.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef GESTIONNAIREETATMODEJEU_H
#define GESTIONNAIREETATMODEJEU_H

#include "GestionnaireEtatAbstrait.h"

////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEtatModeJeu
/// @brief Classe qui gère les événement lors du mode jeu
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEtatModeJeu : public GestionnaireEtatAbstrait
{
public:
	/// Constructeur par paramètre.
	GestionnaireEtatModeJeu(Partie* pGame);
	/// Destructeur virtuel
	virtual ~GestionnaireEtatModeJeu();

	/// Comportement pour une touche enfoncée
	virtual void toucheEnfoncee(EvenementClavier& evenementClavier);
	/// Comportement pour une touche relachée
	virtual void toucheRelachee(EvenementClavier& evenementClavier);
	/// Comportement pour un bouton de souris enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris);
	/// Comportement pour un bouton de souris relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris);
	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris);
	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris(EvenementRouletteSouris& evenementRouletteSouris);
	/// Animations pour le mode jeu
	virtual void animer( const float& temps );

	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();
	virtual void miseAJourEvenementsRepetitifs( float deltaTemps );

private:
	bool shiftEnfonce_;
	bool enfonce_;
	int boutonEnfonce_;
	Vecteur2i positionSouris_;
    /// pointer to the current game
    Partie* mGame;
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////