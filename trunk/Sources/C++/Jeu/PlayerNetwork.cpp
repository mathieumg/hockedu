//////////////////////////////////////////////////////////////////////////////
/// @file JoueurNetwork.cpp
/// @author Mahtieu Parent
/// @date 2013-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PlayerNetwork.h"
#include "RazerGameUtilities.h"
#include "NoeudMaillet.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurNetwork::JoueurNetwork(std::string nom)
///
/// Constructeur qui initialisela le joueur network.
///
/// @param[in] nom : le nom du joueur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerNetwork::PlayerNetwork(std::string nom) : PlayerAbstract(nom)
{
	type_ = JOUEUR_NETWORK;
}

#if WITH_JAVA  
////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurNetwork::JoueurNetwork( JNIEnv* env, jobject& joueurHumain )
///
/// Constructeur qui initialisela le joueur network à partir d'un objet Java.
///
/// @param[in] env : l'environnement Java.
/// @param[in] joueurHumain : un joueur humain précédement déclaré en Java
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerNetwork::PlayerNetwork( JNIEnv* env, jobject& joueurHumain ) : PlayerAbstract("")
{
    // NE DEVRAIT PAS ETRE UTILISE


	// Obtention de la classe
	jclass classe = env->GetObjectClass(joueurHumain);

	// Obtention du nom
	jmethodID obtenirNom = env->GetMethodID(classe, "obtenirNom", "()Ljava/lang/String;");
	jstring nom = (jstring)env->CallObjectMethod(joueurHumain, obtenirNom);

	// Modification des attributs
	modifierNom(RazerGameUtilities::obtenirChaineISO(env, &nom));
	type_ = JOUEUR_NETWORK;

}
#endif //WITH_JAVA  

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurNetwork::~JoueurNetwork( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerNetwork::~PlayerNetwork( void )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* JoueurNetwork::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return XmlElement* le noeud XMl du joueur
///
////////////////////////////////////////////////////////////////////////
XmlElement* PlayerNetwork::creerNoeudXML() const
{
	XmlElement* elementNoeud = PlayerAbstract::creerNoeudXML();
	
	XMLUtils::writeAttribute<int>(elementNoeud,PlayerAbstract::etiquetteType.c_str(),type_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurNetwork::initialiser( const XmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool PlayerNetwork::initialiser( const XmlElement* element )
{
	if(!PlayerAbstract::initialiser(element))
		return false;
	type_ = JOUEUR_NETWORK;

	return true;
}

void PlayerNetwork::PlayTick( float time )
{
	auto maillet = getControlingMallet();
	if(maillet)
	{
		maillet->setTargetDestination(getTargetDestination(),true);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


