#pragma once
#include "GestionnaireEtatAbstrait.h"

class GestionnaireEtatPartieTournoiTerminee :
	public GestionnaireEtatAbstrait
{
public:
	/// Constructeur
	GestionnaireEtatPartieTournoiTerminee();

	/// On vise la camera sur le tableau
	void pointerCameraSurScoreBoard();

	/// Destructeur
	virtual ~GestionnaireEtatPartieTournoiTerminee(void);

	/// Comportement pour une touche enfonc�e
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );

	/// Comportement pour une touche relach�e
	virtual void toucheRelachee( EvenementClavier& evenementClavier );

	/// Comportement pour un bouton de souris enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );

	/// Comportement pour un bouton de souris rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );

	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Comportement lorsqu'on bouge la roulette de la souris
	virtual void rouletteSouris( EvenementRouletteSouris& evenementRouletteSouris );

	/// Comportement de la methode animer 
	virtual void animer( const float& temps );
	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();
};

