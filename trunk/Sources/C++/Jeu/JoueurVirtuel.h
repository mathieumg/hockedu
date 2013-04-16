//////////////////////////////////////////////////////////////////////////////
/// @file JoueurVirtuel.h
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
#include "NoeudRondelle.h"
#include <memory>

class AIMaillet;

///////////////////////////////////////////////////////////////////////////
/// @class JoueurVirtuel
/// @brief Classe qui repr�sente un joueur contr�l� par l'ordinateur.
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class JoueurVirtuel : public JoueurAbstrait, public std::enable_shared_from_this<JoueurVirtuel>
{
public:

	/// Constructeur par param�tres
	JoueurVirtuel(std::string nom = "", unsigned int vitesse = 0, unsigned int probabiliteEchec = 0);

#if WITH_JAVA  
	/// Constructeur par objet Java
	JoueurVirtuel(JNIEnv* env, jobject& joueurVirtuel);
#endif //WITH_JAVA  

	/// Destructeur virtuel
	virtual ~JoueurVirtuel(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;


	/// Permet de generer des informations aleatoire pour un joueur,
	/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
	virtual void genererAleatoirement();

	/// Permet d'obtenir une copie de ce joueur
	//virtual JoueurAbstrait* obtenirCopie();

	/// Accesseurs et modificateurs des attributs
	unsigned int obtenirVitesse() const;
	void modifierVitesse(unsigned int vitesse);
	unsigned int obtenirProbabiliteEchec() const;
	void modifierProbabiliteEchec(unsigned int probabiliteEchec);

	/// Questionne le AIMaillet sur la direction � prendre du maillet
	virtual Vecteur2 obtenirDirectionAI(NoeudMaillet* maillet);
	/// Accesseur de aiMaillet_
	AIMaillet* getAiMaillet() const { return aiMaillet_; }
	virtual void setAiMaillet(AIMaillet* val);


private:
	/// Vitesse du joueur
	unsigned int vitesse_;
	
	/// Probabilit� de manquer la rondelle
	unsigned int probabiliteEchec_;
    /// Pointeur sur AIMaillet pour savoir dans quelle direction aller
    AIMaillet* aiMaillet_;
	
protected:
	/// Initialisaiton du joueur � partir d'un element XML
	virtual bool initialiser(const XmlElement* element);


	
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





