//////////////////////////////////////////////////////////////////////////////
/// @file JoueurNetwork.h
/// @author Mathieu Parent
/// @date 2013-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "joueurabstrait.h"
#include <jni.h>

///////////////////////////////////////////////////////////////////////////
/// @class JoueurNetwork
/// @brief Classe abstraite qui représente un joueur contrôlé par le reseau (agit comme un joueur humain sans etre bind a la souris)
///
/// @author Mathieu Parent
/// @date 2013-03-13
///////////////////////////////////////////////////////////////////////////
class JoueurNetwork : public JoueurAbstrait
{
public:
	/// Constructeur par paramètres
	JoueurNetwork(std::string nom = "");
	/// Constructeur par objet Java
	JoueurNetwork(JNIEnv* env, jobject& joueurHumain);
	/// Destructeur virtuel
	virtual ~JoueurNetwork(void);

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




