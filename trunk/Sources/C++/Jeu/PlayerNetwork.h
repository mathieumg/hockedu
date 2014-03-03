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
	/// Destructeur virtuel
	virtual ~PlayerNetwork(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;

	virtual void PlayTick( float time );

protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
	Vecteur3 mTargetDestination;

public:
	inline const Vecteur3& getTargetDestination() const { return mTargetDestination; }
	inline void setTargetDestination(const Vecteur3& val) { mTargetDestination = val; }
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




