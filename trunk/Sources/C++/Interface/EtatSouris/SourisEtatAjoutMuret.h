//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutMuret.h
/// @author Mathieu Parent
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include "SourisEtatAjout.h"

enum AjoutMuret{PLACERPOINT1, PLACERPOINT2, PLACERNBETATS};
////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatAjoutMuret
/// @brief Classe de gestion du comportement dans le mode ajout de murets.
///
/// @author Mathieu Parent
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjoutMuret :
	public SourisEtatAjout
{
public:
	/// Constructeur par param�tre
	SourisEtatAjoutMuret(Terrain* pField);
	/// Destructeur virtuel
	virtual ~SourisEtatAjoutMuret(void);

	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );
	
private : 
	/// Sert � v�rifier l'�tat de cr�ation du muret
	AjoutMuret etat_;
	/// Positions des extr�mit�s du muret
	Vecteur3 positionClic1_;
	Vecteur3 positionClic2_;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


