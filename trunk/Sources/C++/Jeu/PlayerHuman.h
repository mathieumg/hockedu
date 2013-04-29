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
#include "PlayerAbstract.h"

#if WITH_JAVA  
#include <jni.h>
#endif //WITH_JAVA  

enum ControllerType
{
	CONTROLLER_TYPE_NONE	 = 0x00,  // Cannot move mallet
	CONTROLLER_TYPE_MOUSE	 = 0x01,
	CONTROLLER_TYPE_KEYBOARD = 0x02,
	CONTROLLER_TYPE_GAMEPAD  = 0x04,  // Reserved but not used
};

///////////////////////////////////////////////////////////////////////////
/// @class JoueurHumain
/// @brief Classe abstraite qui représente un joueur contrôlé par l'ordinateur.
///
/// @author Vincent Lemire
/// @date 2012-02-17
///////////////////////////////////////////////////////////////////////////
class PlayerHuman : public PlayerAbstract
{
public:
	/// Constructeur par paramètres
	PlayerHuman(std::string nom = "");
#if WITH_JAVA  
	/// Constructeur par objet Java
	PlayerHuman(JNIEnv* env, jobject& joueurHumain);
#endif //WITH_JAVA  
	/// Destructeur virtuel
	virtual ~PlayerHuman(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;

	virtual void PlayTick( float time );

protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
	ControllerType mControllerType;

public:
	inline ControllerType getControllerType() const { return mControllerType; }
	inline void setControllerType(ControllerType val) { mControllerType = val; }
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




