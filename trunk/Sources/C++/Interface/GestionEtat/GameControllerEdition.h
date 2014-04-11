//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerEdition.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#ifndef GESTIONNAIREETATMODEEDITION_H
#define GESTIONNAIREETATMODEEDITION_H

#include "GameControllerAbstract.h"
#include "GameTime.h"

class GestionnaireEvenementsTest;
class Terrain;
////////////////////////////////////////////////////////////////////////////
/// @class GameControllerAbstract
/// @brief Classe qui g�re les �v�nement lors du mode �dition
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GameControllerEdition : public GameControllerAbstract
{
public:
    typedef GameControllerAbstract Super;
	/// utile pour les tests.
	friend GestionnaireEvenementsTest;

	/// Constructeur par param�tre.
	GameControllerEdition(Terrain* pField);

    virtual void modifierEtatSouris(NomEtatSouris etatSouris);

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

	virtual void miseAJourEvenementsRepetitifs( float deltaTemps );
	/// Animation en mode �dition
	virtual void animer( const float& temps );
	/// Permet d'effectuer l'affichage specifique a l'etat
	virtual void afficher();
private:
	bool shiftEnfonce_;
	bool enfonce_;
	int boutonEnfonce_;
	Vecteur2i positionSouris_;
    /// pointer on the field currently being modified
    Terrain* mField;
    GameTime mAutoSaveTimer;
};

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


