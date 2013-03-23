//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformation.h
/// @author Michael Ferris
/// @date 2013-03-23
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"
#include "RazerGameTypeDef.h"
#include "..\..\Arbre\FieldModificationStrategy\FieldModificationStrategyAbstract.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatTransformation
/// @brief Classe de gestion du comportement pour les modifications sur le terrain
///
/// @author Michael Ferris
/// @date 2013-03-23
////////////////////////////////////////////////////////////////////////////
class SourisEtatTransformation :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par parametres
	SourisEtatTransformation(FieldModificationStrategyType modifType);
	/// Destructeur virtuel
	virtual ~SourisEtatTransformation(void);

	/// Comportement lorsqu'une touche du clavier est enfoncé.
	virtual void toucheRelachee( EvenementClavier& evenementClavier );
	/// Comportement lorsqu'un bouton de la souris est enfoncé.
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relaché.
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est déplacée.
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant
	virtual NomEtatSouris obtenirNomEtatSouris();

protected:
    FieldModificationStrategyType mModifType;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


