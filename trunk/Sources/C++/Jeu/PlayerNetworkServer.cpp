//////////////////////////////////////////////////////////////////////////////
/// @file JoueurNetworkServeur.cpp
/// @author Mahtieu Parent
/// @date 2013-03-18
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PlayerNetworkServer.h"
#include "RazerGameUtilities.h"
#include "NoeudMaillet.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurNetworkServeur::JoueurNetworkServeur(std::string nom)
///
/// Constructeur qui initialisela le joueur network du serveur jeu.
///
/// @param[in] nom : le nom du joueur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerNetworkServer::PlayerNetworkServer(std::string nom) : PlayerAbstract(nom)
{
	type_ = JOUEUR_NETWORK_SERVEUR;
    setReady(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurNetworkServeur::~JoueurNetworkServeur( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerNetworkServer::~PlayerNetworkServer( void )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* JoueurNetworkServeur::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return XmlElement* le noeud XMl du joueur
///
////////////////////////////////////////////////////////////////////////
XmlElement* PlayerNetworkServer::creerNoeudXML() const
{
	XmlElement* elementNoeud = PlayerAbstract::creerNoeudXML();
	
	XMLUtils::writeAttribute<int>(elementNoeud,PlayerAbstract::etiquetteType.c_str(),type_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurNetworkServeur::initialiser( const XmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool PlayerNetworkServer::initialiser( const XmlElement* element )
{
	if(!PlayerAbstract::initialiser(element))
		return false;
	type_ = JOUEUR_NETWORK_SERVEUR;

	return true;
}

void PlayerNetworkServer::PlayTick( float time )
{
	auto maillet = getControlingMallet();
	if(maillet)
	{
		maillet->setTargetDestination(getTargetDestination());
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


