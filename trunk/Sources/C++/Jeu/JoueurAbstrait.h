//////////////////////////////////////////////////////////////////////////////
/// @file JoueurAbstrait.h
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "RazerGameTypeDef.h"
#include "XMLUtils.h"

enum TypeJoueur{JOUEUR_HUMAIN,JOUEUR_VIRTUEL};

///////////////////////////////////////////////////////////////////////////
/// @class JoueurAbstrait
/// @brief Classe abstraite qui représente un joueur dans le mode jeu.
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class JoueurAbstrait
{
public:
	/// Constructeur
    JoueurAbstrait(const std::string& nom);	
	/// Destructeur virtuel pure
	virtual ~JoueurAbstrait(void) = 0;
	/// Accesseur pour le nom
	std::string obtenirNom() const;
	/// Modificateur pour le nom
	void modifierNom(const std::string nom);
	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;
	/// Permet de generer des informations aleatoire pour un joueur,
	/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
	virtual void genererAleatoirement() = 0;

	TypeJoueur obtenirType() const { return type_; }
	void modifierType(const TypeJoueur val) { type_ = val; }

	static const std::string etiquetteJoueur;
	static const std::string etiquetteType;

	/// Permet d'obtenir un pointeur sur un joueur à partir d'un élément XML, pointeur null si echec
	static SPJoueurAbstrait usineJoueurXML(const XmlElement* element, ConteneurJoueur* profilsExistant = 0);

    /// Accessors of mControlingMallet
    inline class NoeudMaillet* getControlingMallet() const { return mControlingMallet; }
    inline void setControlingMallet(class NoeudMaillet* pVal) { mControlingMallet = pVal; }

protected:
	/// Le type du joueur (humain ou virtuel)
	TypeJoueur type_;
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
private:
	/// Le nom du joueur
	std::string nom_;
	/// Outils pour connaitre le nom d'un joueur contenu dans un noeud XML
	static std::string extraireNomXmlNode(const XmlElement* element);

    class NoeudMaillet* mControlingMallet;

    

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





