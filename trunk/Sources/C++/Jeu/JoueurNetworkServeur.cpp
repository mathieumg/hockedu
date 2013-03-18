//////////////////////////////////////////////////////////////////////////////
/// @file JoueurNetworkServeur.cpp
/// @author Mahtieu Parent
/// @date 2013-03-18
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "JoueurNetworkServeur.h"
#include "RazerGameUtilities.h"

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
JoueurNetworkServeur::JoueurNetworkServeur(std::string nom) : JoueurAbstrait(nom)
{
	type_ = JOUEUR_NETWORK;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurNetworkServeur::JoueurNetworkServeur( JNIEnv* env, jobject& joueurHumain )
///
/// Constructeur qui initialisela le joueur network � partir d'un objet Java.
///
/// @param[in] env : l'environnement Java.
/// @param[in] joueurHumain : un joueur humain pr�c�dement d�clar� en Java
///
/// @return
///
////////////////////////////////////////////////////////////////////////
JoueurNetworkServeur::JoueurNetworkServeur( JNIEnv* env, jobject& joueurHumain ) : JoueurAbstrait("")
{
    // NE DEVRAIT PAS ETRE UTILISE


	// Obtention de la classe
	jclass classe = env->GetObjectClass(joueurHumain);

	// Obtention du nom
	jmethodID obtenirNom = env->GetMethodID(classe, "obtenirNom", "()Ljava/lang/String;");
	jstring nom = (jstring)env->CallObjectMethod(joueurHumain, obtenirNom);

	// Modification des attributs
	modifierNom(RazerGameUtilities::obtenirChaineISO(env, &nom));
	type_ = JOUEUR_NETWORK_SERVEUR;

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
JoueurNetworkServeur::~JoueurNetworkServeur( void )
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
XmlElement* JoueurNetworkServeur::creerNoeudXML() const
{
	XmlElement* elementNoeud = JoueurAbstrait::creerNoeudXML();
	
	XMLUtils::writeAttribute<int>(elementNoeud,JoueurAbstrait::etiquetteType.c_str(),type_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurNetworkServeur::initialiser( const XmlElement* element )
///
/// Initialisaiton du joueur � partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool Vrai si l'initialisation � bien �t� faite
///
////////////////////////////////////////////////////////////////////////
bool JoueurNetworkServeur::initialiser( const XmlElement* element )
{
	if(!JoueurAbstrait::initialiser(element))
		return false;
	type_ = JOUEUR_NETWORK_SERVEUR;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurNetworkServeur::genererAleatoirement()
///
/// Permet de generer des informations aleatoire pour un joueur,
/// utile si on a besoin d'un joueur, mais probleme rencontr� a son chargement
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void JoueurNetworkServeur::genererAleatoirement()
{
	modifierNom("Joueur Network Serveur Aleatoire");
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


