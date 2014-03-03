//////////////////////////////////////////////////////////////////////////////
/// @file JoueurHumain.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PlayerHuman.h"
#include "RazerGameUtilities.h"
#include "NoeudMaillet.h"
#include "EventManager.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurHumain::JoueurHumain(std::string nom)
///
/// Constructeur qui initialisela le joueur humain.
///
/// @param[in] nom : le nom du joueur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerHuman::PlayerHuman(std::string nom) : PlayerAbstract(nom), mControllerType(CONTROLLER_TYPE_MOUSE)
{
	type_ = JOUEUR_HUMAIN;
} 

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurHumain::~JoueurHumain( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerHuman::~PlayerHuman( void )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* JoueurHumain::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return XmlElement* le noeud XMl du joueur
///
////////////////////////////////////////////////////////////////////////
XmlElement* PlayerHuman::creerNoeudXML() const
{
	XmlElement* elementNoeud = PlayerAbstract::creerNoeudXML();
	
	XMLUtils::writeAttribute<int>(elementNoeud,PlayerAbstract::etiquetteType.c_str(),type_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurHumain::initialiser( const XmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool PlayerHuman::initialiser( const XmlElement* element )
{
	if(!PlayerAbstract::initialiser(element))
		return false;
	type_ = JOUEUR_HUMAIN;

	return true;
}

void PlayerHuman::PlayTick( float time )
{
	auto maillet = getControlingMallet();
	if(maillet)
	{
		if(mControllerType & CONTROLLER_TYPE_KEYBOARD)
		{
			// if any key is pressed
			if(EventManager::mMalletDirection.mValue)
			{
				Vecteur3 newPos;
				// give direction according to key pressed
				if(EventManager::mMalletDirection.IsFlagSet(MALLET_UP))
				{
					++newPos[VY];
				}
				if(EventManager::mMalletDirection.IsFlagSet(MALLET_DOWN))
				{
					--newPos[VY];
				}
				if(EventManager::mMalletDirection.IsFlagSet(MALLET_LEFT))
				{
					--newPos[VX];
				}
				if(EventManager::mMalletDirection.IsFlagSet(MALLET_RIGHT))
				{
					++newPos[VX];
				}

				// make sure final direction is length 8
				newPos.normaliser();
				newPos *= 8;
				newPos += maillet->getPosition();
				
				maillet->setTargetDestination(newPos);
			}
		}
		else if(mControllerType & CONTROLLER_TYPE_MOUSE)
		{
			maillet->setTargetDestination(EventManager::mMouseGamePos);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


