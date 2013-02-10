//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireEtatMenuPrincipal.h
/// @author Vincent Lemire
/// @date 2012-03-02
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GestionnaireEtatAbstrait.h"

////////////////////////////////////////////////////////////////////////////
/// @class GestionnaireEtatMenuPrincipal
/// @brief G�re le logiciel lorsque l'utilisateur est sur le menu principal 
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GestionnaireEtatMenuPrincipal : public GestionnaireEtatAbstrait
{
public:

	/// Constructeur par param�tre.
	GestionnaireEtatMenuPrincipal(GestionnaireEvenements* contexte);

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
	/// Comportement de la methode animer pour le menu principal
	virtual void animer( const float& temps );

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



