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
#include "PlayerAbstract.h"
#if WITH_JAVA  
#include <jni.h>
#endif //WITH_JAVA  

///////////////////////////////////////////////////////////////////////////
/// @class JoueurNetwork
/// @brief Classe abstraite qui représente un joueur contrôlé par le reseau (agit comme un joueur humain sans etre bind a la souris)
///
/// @author Mathieu Parent
/// @date 2013-03-13
///////////////////////////////////////////////////////////////////////////
class PlayerNetwork : public PlayerAbstract
{
public:
	/// Constructeur par paramètres
	PlayerNetwork(std::string nom = "");
#if WITH_JAVA  
	/// Constructeur par objet Java
	PlayerNetwork(JNIEnv* env, jobject& joueurHumain);
#endif //WITH_JAVA  
	/// Destructeur virtuel
	virtual ~PlayerNetwork(void);

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




