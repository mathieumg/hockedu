//////////////////////////////////////////////////////////////////////////////
/// @file JoueurNetwork.cpp
/// @author Mahtieu Parent
/// @date 2013-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "JoueurNetwork.h"
#include "RazerGameUtilities.h"

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
JoueurNetwork::JoueurNetwork(std::string nom) : JoueurAbstrait(nom)
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
JoueurNetwork::JoueurNetwork( JNIEnv* env, jobject& joueurHumain ) : JoueurAbstrait("")
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
JoueurNetwork::~JoueurNetwork( void )
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
XmlElement* JoueurNetwork::creerNoeudXML() const
{
	XmlElement* elementNoeud = JoueurAbstrait::creerNoeudXML();
	
	XMLUtils::writeAttribute<int>(elementNoeud,JoueurAbstrait::etiquetteType.c_str(),type_);
	
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
bool JoueurNetwork::initialiser( const XmlElement* element )
{
	if(!JoueurAbstrait::initialiser(element))
		return false;
	type_ = JOUEUR_NETWORK;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurNetwork::genererAleatoirement()
///
/// Permet de generer des informations aleatoire pour un joueur,
/// utile si on a besoin d'un joueur, mais probleme rencontré a son chargement
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void JoueurNetwork::genererAleatoirement()
{
	modifierNom("Joueur Network Aleatoire");
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


