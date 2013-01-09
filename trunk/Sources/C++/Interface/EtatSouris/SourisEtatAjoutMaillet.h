//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutMaillet.h
/// @author Gabriel Couvrette
/// @date 2012-02-13
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"
#include "ArbreRenduINF2990.h"

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatAjoutMaillet
/// @brief Classe de gestion du comportement dans le mode ajout de maillet
///
/// @author Gabriel Couvrette
/// @date 2012-02-13
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjoutMaillet :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par param�tre
	SourisEtatAjoutMaillet();
	/// Destructeur virtuel
	virtual ~SourisEtatAjoutMaillet(void);

	/// methode qui cree des noeuds sur le parent en parametres.
	void genererNoeud();

	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'�tat courant de la souris
	virtual NomEtatSouris obtenirNomEtatSouris();
	

private : 
	/// Pointeur vers le maillet en train d'�tre ajout�
	NoeudAbstrait* mailletConcerne_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


