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
/// @brief Classe d'ajout des accelerateurs (État du gestionnaire d'événement)
///			L'arbre de noeud libre permet de centraliser la gestion de la mémoire des noeuds
///			à ajouter dans l'arbre de rend final
///
/// @author Samuel Ledoux, Michael Ferris
/// @date 2012-01-21
////////////////////////////////////////////////////////////////////////////
class SourisEtatAjoutAccelerateur :
	public SourisEtatAbstrait
{
public:
	/// Constructeur par paramètre
	SourisEtatAjoutAccelerateur();
	/// Destructeur virtuel
	virtual ~SourisEtatAjoutAccelerateur(void);

	/// Comportement lorsqu'un bouton de la souris est enfoncé
	virtual void sourisEnfoncee( EvenementSouris& evenementSouris );
	/// Comportement lorsqu'un bouton de la souris est relâché
	virtual void sourisRelachee( EvenementSouris& evenementSouris );
	/// Comportement lorsque la souris est déplacée
	virtual void sourisDeplacee( EvenementSouris& evenementSouris );

	/// Retourne l'état courant de la souris
	virtual NomEtatSouris obtenirNomEtatSouris();
private : 
	/// Permet de générer un accelerateur et rediriger vers l'arbre de rendu final si le noeud existe déjà
	void genererNoeud();

	/// Pointeur vers l'accelerateur en train d'être ajouté
	NoeudAbstrait* accelerateurConcerne_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


