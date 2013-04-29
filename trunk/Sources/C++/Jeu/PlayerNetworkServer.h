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
#include "PlayerAbstract.h"


///////////////////////////////////////////////////////////////////////////
/// @class JoueurNetworkServeur
/// @brief Classe abstraite qui représente un joueur contrôlé par le reseau dans le serveur jeu (agit comme un joueur humain sans etre bind a la souris) et doit envoyer sa position
///
/// @author Mathieu Parent
/// @date 2013-03-18
///////////////////////////////////////////////////////////////////////////
class PlayerNetworkServer : public PlayerAbstract
{
public:
	/// Constructeur par paramètres
	PlayerNetworkServer(std::string nom = "");
	/// Destructeur virtuel
	virtual ~PlayerNetworkServer(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;
	virtual void PlayTick( float time );
protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
	Vecteur3 mTargetDestination;
	bool mIsReady;

public:
	inline const Vecteur3& getTargetDestination() const { return mTargetDestination; }
	inline void setTargetDestination(const Vecteur3& val) { mTargetDestination = val; }

	inline bool isReady() { return mIsReady; }
	inline void setReady(const bool pReady) { mIsReady = pReady; }
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




