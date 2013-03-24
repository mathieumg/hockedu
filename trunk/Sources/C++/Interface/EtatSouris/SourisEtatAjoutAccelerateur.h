//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutAccelerateur.h
/// @author Samuel Ledoux
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAbstrait.h"

class ArbreNoeudLibre;
class ArbreRenduINF2990;
class NoeudAbstrait;

////////////////////////////////////////////////////////////////////////////
/// @class SourisEtatAjoutAccelerateur
/// @brief Classe d'ajout des accelerateurs (�tat du gestionnaire d'�v�nement)
///			L'arbre de noeud libre permet de centraliser la gestion de la m�moire des noeuds
///			� ajouter dans l'arbre de rend final
///
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjoutAccelerateur :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par param�tre
	SourisEtatAjoutAccelerateur();
	/// Destructeur virtuel
	virtual ~SourisEtatAjoutAccelerateur(void);

	/// Comportement lorsqu'un bouton de la souris est enfonc�
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est rel�ch�
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est d�plac�e
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'�tat courant de la souris
	virtual NomEtatSouris obtenirNomEtatSouris();
private : 
	/// Permet de g�n�rer un accelerateur et rediriger vers l'arbre de rendu final si le noeud existe d�j�
	void genererNoeud();

	/// Pointeur vers l'accelerateur en train d'�tre ajout�
	NoeudAbstrait* accelerateurConcerne_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


