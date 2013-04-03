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

class AIMaillet;

///////////////////////////////////////////////////////////////////////////
/// @class JoueurVirtuel
/// @brief Classe qui représente un joueur contrôlé par l'ordinateur.
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class JoueurVirtuel : public JoueurAbstrait
{
public:

	/// Constructeur par paramètres
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

	/// Questionne le AIMaillet sur la direction à prendre du maillet
	virtual Vecteur2 obtenirDirectionAI(NoeudMaillet* maillet);

private:
	/// Vitesse du joueur
	unsigned int vitesse_;
	
	/// Probabilité de manquer la rondelle
	unsigned int probabiliteEchec_;

	
protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);

    /// Pointeur sur AIMaillet pour savoir dans quelle direction aller
    AIMaillet* aiMaillet_;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





