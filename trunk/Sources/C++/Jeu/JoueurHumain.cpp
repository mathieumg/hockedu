//////////////////////////////////////////////////////////////////////////////
/// @file JoueurHumain.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "JoueurHumain.h"
#include "UtilitaireINF2990.h"
#include "tinyxml.h"

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
JoueurHumain::JoueurHumain(std::string nom) : JoueurAbstrait(nom)
{
	type_ = JOUEUR_HUMAIN;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurHumain::JoueurHumain( JNIEnv* env, jobject& joueurHumain )
///
/// Constructeur qui initialisela le joueur virtuel à partir d'un objet Java.
///
/// @param[in] env : l'environnement Java.
/// @param[in] joueurHumain : un joueur humain précédement déclaré en Java
///
/// @return
///
////////////////////////////////////////////////////////////////////////
JoueurHumain::JoueurHumain( JNIEnv* env, jobject& joueurHumain ) : JoueurAbstrait("")
{
	// Obtention de la classe
	jclass classe = env->GetObjectClass(joueurHumain);

	// Obtention du nom
	jmethodID obtenirNom = env->GetMethodID(classe, "obtenirNom", "()Ljava/lang/String;");
	jstring nom = (jstring)env->CallObjectMethod(joueurHumain, obtenirNom);

	// Modification des attributs
	modifierNom(utilitaire::obtenirChaineISO(env, &nom));
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
JoueurHumain::~JoueurHumain( void )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* JoueurHumain::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return TiXmlElement* le noeud XMl du joueur
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* JoueurHumain::creerNoeudXML() const
{
	TiXmlElement* elementNoeud = JoueurAbstrait::creerNoeudXML();
	
	elementNoeud->SetAttribute(JoueurAbstrait::etiquetteType.c_str(),type_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurHumain::initialiser( const TiXmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
///
/// @param[in] const TiXmlElement * element
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool JoueurHumain::initialiser( const TiXmlElement* element )
{
	if(!JoueurAbstrait::initialiser(element))
		return false;
	type_ = JOUEUR_HUMAIN;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurHumain::genererAleatoirement()
///
/// Permet de generer des informations aleatoire pour un joueur,
/// utile si on a besoin d'un joueur, mais probleme rencontré a son chargement
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void JoueurHumain::genererAleatoirement()
{
	modifierNom("Joueur Humain Aleatoire");
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


