//////////////////////////////////////////////////////////////////////////////
/// @file GameControllerQuickPlay.h
/// @author Vincent Lemire
/// @date 2012-01-20
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GameControllerAbstract.h"
#include <map>
#include "GameTime.h"

enum GolTools
{
    GOL_TOOL_CREATE,
    GOL_TOOL_ERASE
};

class HUDElement;
////////////////////////////////////////////////////////////////////////////
/// @class GameControllerGoL
/// @brief Classe qui gère les événement lors du mode jeu Game Of Life
///
/// @author Vincent Lemire
/// @date 2012-01-20
////////////////////////////////////////////////////////////////////////////
class GameControllerGoL : public GameControllerAbstract
{
public:
	/// Constructeur par paramètre.
    GameControllerGoL();
	/// Destructeur virtuel
    virtual ~GameControllerGoL( );

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
    typedef std::map<Vecteur2i, int> Cells;
    Cells mAlives, mMaybes;
    GameTime mTimer;
    HUDElement* mHUDRoot;
    GolTools mTool;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////