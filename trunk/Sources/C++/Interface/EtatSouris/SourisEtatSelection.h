//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatSelection.h
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"
#include "Flags.h"



////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatSelection
/// @brief Classe de gestion du comportement pour l'état Sélection.
///
/// @author Mathieu Parent
/// @date 2012-02-06
////////////////////////////////////////////////////////////////////////////
class SourisEtatSelection :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatSelection();
	/// Destructeur virtuel
	virtual ~SourisEtatSelection(void);

	/// Comportement lorsqu'une touche du clavier est enfoncée.
	virtual void toucheEnfoncee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'une touche du clavier est relâchée.
	virtual void toucheRelachee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'un bouton de la souris est enfoncé.
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relaché.
	virtual void sourisRelachee( EvenementSouris& evenementSouris );

    void GetOpenGLSelectedList( ConteneurIdNoeuds &liste );

    /// Comportement lorsque la souris est déplacée.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant
	virtual NomEtatSouris obtenirNomEtatSouris();

    virtual void doubleClickEvent( EvenementSouris& evenementSouris );

private:
    enum {
        // true if the left mouse button is pressed
        SELECTION_LEFT_CLICK_PRESSED, 
        /// Indique s'il s'agit d'une selection rectangle ou non
        SELECTION_MOUSE_DRAGGING, 
        // flag telling if an undo stack has been pushed
        SELECTION_UNDO_SENT, 
        NB_SELECTION_FLAGS
    };
    Flags<char,NB_SELECTION_FLAGS> mFlags;
	/// Conserve en mémoire la position de la souris lorsquelle a ete enfoncee
	Vecteur2i mInitialMousePos;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


