//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatPIEMode.h
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SourisEtatAbstrait.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatPIEMode
/// @brief Classe de gestion du comportement du PIE Mode
///
/// @author Michael Ferris
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatPIEMode:
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatPIEMode(Vecteur2i& pos);
	/// Destructeur virtuel
	virtual ~SourisEtatPIEMode(void);
	/// Comportement lorsqu'une touche du clavier est enfonc�e.
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'une touche du clavier est rel�ch�e.
	virtual void toucheRelachee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'un bouton de la souris est enfonc�.
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relach�.
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

    virtual bool OverrideAnimate(float temps);
    virtual bool OverrideRender();

	/// Retourne l'�tat courant
	virtual NomEtatSouris obtenirNomEtatSouris();

private:
    class Partie* mPIEGame;
    bool mInitialised;
    bool shiftEnfonce_;
    ToucheClavier toucheSauvegardee_;
    Vecteur2i mMousePos;
    BoutonSouris boutonEnfonce_;
    bool mMiddleMousePress;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


