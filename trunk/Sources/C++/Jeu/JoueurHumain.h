//////////////////////////////////////////////////////////////////////////////
/// @file JoueurHumain.h
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "joueurabstrait.h"

#if WITH_JAVA  
#include <jni.h>
#endif //WITH_JAVA  

///////////////////////////////////////////////////////////////////////////
/// @class JoueurHumain
/// @brief Classe abstraite qui repr�sente un joueur contr�l� par l'ordinateur.
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class JoueurHumain : public JoueurAbstrait
{
public:
	/// Constructeur par param�tres
	JoueurHumain(std::string nom = "");
#if WITH_JAVA  
	/// Constructeur par objet Java
	JoueurHumain(JNIEnv* env, jobject& joueurHumain);
#endif //WITH_JAVA  
	/// Destructeur virtuel
	virtual ~JoueurHumain(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;


	/// Permet de generer des informations aleatoire pour un joueur,
	/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
	virtual void genererAleatoirement();

	/// Permet d'obtenir une copie de ce joueur
	//virtual JoueurAbstrait* obtenirCopie();

protected:
	/// Initialisaiton du joueur � partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




