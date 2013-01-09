//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatPartieRapideTerminee.h
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////


#pragma once
#include "GestionnaireEtatAbstrait.h"


////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEtatPartieRapideTerminee
/// @brief Classe abstraite dont les h�riti�res gereront les �tats selon le
/// patron state.
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-04-13
////////////////////////////////////////////////////////////////////////////
class GestionnaireEtatPartieRapideTerminee :
	public GestionnaireEtatAbstrait
{
public:
	/// Constructeur
	GestionnaireEtatPartieRapideTerminee(GestionnaireEvenements* contexte);
	/// Destructeur
	virtual ~GestionnaireEtatPartieRapideTerminee(void);

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

