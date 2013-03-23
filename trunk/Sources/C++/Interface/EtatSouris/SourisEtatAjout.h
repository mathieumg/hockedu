//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjout.h
/// @author Michael Ferris
/// @date 2013-03-23
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include "SourisEtatTransformation.h"

class NoeudAbstrait;
class HUDTexte;
class Terrain;

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatAjout
/// @brief Classe de gestion du comportement pour les ajouts sur le terrain
///
/// @author Michael Ferris
/// @date 2013-03-23
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjout :
	public SourisEtatTransformation
{
public:
	/// Constructeur par param�tre
	SourisEtatAjout(FieldModificationStrategyType modifType);
	/// Destructeur virtuel
	virtual ~SourisEtatAjout(void);

	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

    virtual void toucheRelachee( EvenementClavier& evenementClavier );

};