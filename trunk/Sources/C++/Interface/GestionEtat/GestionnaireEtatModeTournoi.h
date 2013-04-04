//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatModeTournoi.h
/// @author Vincent Lemire
/// @date 2012-03-02
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GestionnaireEtatAbstrait.h"
#include "ReplayObserver.h"


class Tournoi;

////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEtatModeTournoi
/// @brief Gère le logiciel lorsque l'utilisateur est en tournoi 
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEtatModeTournoi : public GestionnaireEtatAbstrait, public ReplayObserver
{
public:
	/// Constructeur par paramètre.
	GestionnaireEtatModeTournoi(Tournoi* tournoi);

	/// Comportement pour une touche enfoncée
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );

	/// Comportement pour une touche relachée
	virtual void toucheRelachee( EvenementClavier& evenementClavier );

	/// Comportement pour un bouton de souris enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );

	/// Comportement pour un bouton de souris relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris );

	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris );

	/// Comportement de la methode animer 
	virtual void animer( const float& temps );

	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();


    virtual void updateObserver( const ReplaySubject* pSubject );
  
private:
    Tournoi* mTournoi;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



