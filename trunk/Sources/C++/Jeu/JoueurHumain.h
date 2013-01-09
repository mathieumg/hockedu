//////////////////////////////////////////////////////////////////////////////
/// @file JoueurHumain.h
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "joueurabstrait.h"
#include <jni.h>

///////////////////////////////////////////////////////////////////////////
/// @class JoueurHumain
/// @brief Classe abstraite qui représente un joueur contrôlé par l'ordinateur.
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class JoueurHumain : public JoueurAbstrait
{
public:
	/// Constructeur par paramètres
	JoueurHumain(std::string nom = "");
	/// Constructeur par objet Java
	JoueurHumain(JNIEnv* env, jobject& joueurHumain);
	/// Destructeur virtuel
	virtual ~JoueurHumain(void);

	/// Creation du noeud XML du joueur
	virtual TiXmlElement* creerNoeudXML() const;


	/// Permet de generer des informations aleatoire pour un joueur,
	/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
	virtual void genererAleatoirement();

	/// Permet d'obtenir une copie de ce joueur
	//virtual JoueurAbstrait* obtenirCopie();

protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const TiXmlElement* element);
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




