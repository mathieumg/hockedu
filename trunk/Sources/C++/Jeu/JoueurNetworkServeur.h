//////////////////////////////////////////////////////////////////////////////
/// @file JoueurNetworkServeur.h
/// @author Mathieu Parent
/// @date 2013-03-18
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "joueurabstrait.h"


///////////////////////////////////////////////////////////////////////////
/// @class JoueurNetworkServeur
/// @brief Classe abstraite qui représente un joueur contrôlé par le reseau dans le serveur jeu (agit comme un joueur humain sans etre bind a la souris) et doit envoyer sa position
///
/// @author Mathieu Parent
/// @date 2013-03-18
///////////////////////////////////////////////////////////////////////////
class JoueurNetworkServeur : public JoueurAbstrait
{
public:
	/// Constructeur par paramètres
	JoueurNetworkServeur(std::string nom = "");
	/// Destructeur virtuel
	virtual ~JoueurNetworkServeur(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;


	/// Permet de generer des informations aleatoire pour un joueur,
	/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
	virtual void genererAleatoirement();

	/// Permet d'obtenir une copie de ce joueur
	//virtual JoueurAbstrait* obtenirCopie();

protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




