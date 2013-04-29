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
#include "Vecteur.h"

enum TypeJoueur{JOUEUR_HUMAIN,JOUEUR_VIRTUEL,JOUEUR_VIRTUEL_RENFORCEMENT,JOUEUR_NETWORK,JOUEUR_NETWORK_SERVEUR};
enum PlayerSide{PLAYER_SIDE_LEFT,PLAYER_SIDE_RIGHT};
///////////////////////////////////////////////////////////////////////////
/// @class JoueurAbstrait
/// @brief Classe abstraite qui représente un joueur dans le mode jeu.
///
/// @author Vincent Lemire, Michael Ferris
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class PlayerAbstract
{
public:
	/// Constructeur
    PlayerAbstract(const std::string& nom);	
	/// Destructeur virtuel pure
	virtual ~PlayerAbstract(void) = 0;
	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;

	/// Permet d'obtenir un pointeur sur un joueur à partir d'un élément XML, pointeur null si echec
	static SPPlayerAbstract usineJoueurXML(const XmlElement* element, ConteneurJoueur* profilsExistant = 0);

	virtual void PlayTick(float time) = 0;

    virtual void gameInit() {}
protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);

	/// Le type du joueur
	TypeJoueur type_;

	static const std::string etiquetteJoueur;
	static const std::string etiquetteType;
private:
	/// Le cote du joueur
	PlayerSide mPlayerSide_;

	/// Le nom du joueur
	std::string nom_;
	/// Outils pour connaitre le nom d'un joueur contenu dans un noeud XML
	static std::string extraireNomXmlNode(const XmlElement* element);

    class NoeudMaillet* mControlingMallet;

    
public:
	/// Accesseur pour le nom
	std::string obtenirNom() const;
	/// Modificateur pour le nom
	virtual void modifierNom(const std::string nom);
	TypeJoueur obtenirType() const { return type_; }
	void modifierType(const TypeJoueur val) { type_ = val; }

    /// Accessors of mControlingMallet
    inline class NoeudMaillet* getControlingMallet() const { return mControlingMallet; }
    virtual void setControlingMallet(class NoeudMaillet* pVal) { mControlingMallet = pVal; }



    /// Accesseur de mPlayerSide_
    PlayerSide getPlayerSide() const { return mPlayerSide_; }
    virtual void setPlayerSide(PlayerSide val) { mPlayerSide_ = val; }

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





