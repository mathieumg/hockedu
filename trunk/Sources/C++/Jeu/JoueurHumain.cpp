//////////////////////////////////////////////////////////////////////////////
/// @file JoueurHumain.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "JoueurHumain.h"
#include "RazerGameUtilities.h"

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
	modifierNom(RazerGameUtilities::obtenirChaineISO(env, &nom));
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
/// @fn XmlElement* JoueurHumain::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return XmlElement* le noeud XMl du joueur
///
////////////////////////////////////////////////////////////////////////
XmlElement* JoueurHumain::creerNoeudXML() const
{
	XmlElement* elementNoeud = JoueurAbstrait::creerNoeudXML();
	
	XMLUtils::writeAttribute<int>(elementNoeud,JoueurAbstrait::etiquetteType.c_str(),type_);
	
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
bool JoueurHumain::initialiser( const XmlElement* element )
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


