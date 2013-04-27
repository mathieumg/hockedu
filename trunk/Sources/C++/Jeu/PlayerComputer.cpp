//////////////////////////////////////////////////////////////////////////////
/// @file JoueurVirtuel.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PlayerComputer.h"
#include "RazerGameUtilities.h"
#include "AIMaillet.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuel::JoueurVirtuel(std::string nom, float vitesse, int probabiliteEchec)
///
/// Constructeur qui initialisela le joueur virtuel.
///
/// @param[in] nom : le nom du joueur.
/// @param[in] vitesse : la vitesse de déplacement du joueur.
/// @param[in] probabiliteEchec : la probabilité que le joueur manque son coup.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerComputer::PlayerComputer(std::string nom, unsigned int vitesse, unsigned int probabiliteEchec):PlayerAbstract(nom) 
{
	vitesse_ = vitesse;
	probabiliteEchec_ = probabiliteEchec;
	type_ = JOUEUR_VIRTUEL;
    aiMaillet_ = NULL;
    setAiMaillet(new AIMaillet(this));
}

#ifdef WITH_JAVA
////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuel::JoueurVirtuel( JNIEnv* env, jobject& joueurVirtuel )
///
/// Constructeur qui initialise le joueur virtuel à partir d'un objet Java.
///
/// @param[in] env : l'environnement Java.
/// @param[in] joueurVirtuel : un joueur virtuel précédement déclaré en Java
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerComputer::PlayerComputer( JNIEnv* env, jobject& joueurVirtuel ) : PlayerAbstract("") ,aiMaillet_(NULL)
{
	// Obtention de la classe
	jclass classe = env->GetObjectClass(joueurVirtuel);

	// Obtention du nom
	jmethodID obtenirNom = env->GetMethodID(classe, "obtenirNom", "()Ljava/lang/String;");
	jstring nom = (jstring)env->CallObjectMethod(joueurVirtuel, obtenirNom);
	
	// Obtention de la vitesse
	jmethodID obtenirVitesse = env->GetMethodID(classe, "obtenirVitesse", "()I");
	jint vitesse = env->CallIntMethod(joueurVirtuel, obtenirVitesse);

	// Obtention de la probabilité d'échec
	jmethodID obtenirProbabiliteEchec = env->GetMethodID(classe, "obtenirProbabiliteEchec", "()I");
	jint probabiliteEchec = env->CallIntMethod(joueurVirtuel, obtenirProbabiliteEchec);

	// Modification des attributs
	modifierNom(RazerGameUtilities::obtenirChaineISO(env, &nom));
	vitesse_ = (unsigned int)(vitesse/100.0*500.0);
	probabiliteEchec_ = probabiliteEchec;
	type_ = JOUEUR_VIRTUEL;

    setAiMaillet(new AIMaillet(this));

}


#endif // WITH_JAVA
////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuel::~JoueurVirtuel( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerComputer::~PlayerComputer( void )
{
    if(aiMaillet_)
    {
	    delete aiMaillet_;
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int JoueurVirtuel::obtenirVitesse()
///
/// Accesseur à la vitesse du joueur.
///
/// @return la vitesse du joueur
///
////////////////////////////////////////////////////////////////////////
unsigned int PlayerComputer::obtenirVitesse() const 
{ 
	return vitesse_; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurVirtuel::modifierVitesse(unsigned int vitesse)
///
/// Modificateur de la vitesse du joueur.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void PlayerComputer::modifierVitesse( unsigned int vitesse )
{ 
	vitesse_ = vitesse; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn unsigned int JoueurVirtuel::obtenirProbabiliteEchec()
///
/// Accesseur à la probabilité d'échec du joueur.
///
/// @return la probabilité d'échec du joueur
///
////////////////////////////////////////////////////////////////////////
unsigned int PlayerComputer::obtenirProbabiliteEchec() const 
{ 
	return probabiliteEchec_; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurVirtuel::modifierProbabiliteEchec(unsigned int probabiliteEchec)
///
/// Modificateur de la probabilité d'échec du joueur.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void PlayerComputer::modifierProbabiliteEchec(unsigned int probabiliteEchec) 
{ 
	probabiliteEchec_ = probabiliteEchec; 
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* JoueurVirtuel::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return XmlElement* le noeud XML du joueur
///
////////////////////////////////////////////////////////////////////////
XmlElement* PlayerComputer::creerNoeudXML() const
{
	XmlElement* elementNoeud = PlayerAbstract::creerNoeudXML();

	elementNoeud->SetAttribute(PlayerAbstract::etiquetteType.c_str(),type_);
	elementNoeud->SetAttribute("vitesse",obtenirVitesse());
	elementNoeud->SetAttribute("probEchec",obtenirProbabiliteEchec());

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurVirtuel::initialiser( const XmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
///
/// @param[in] const XmlElement * element : élément contenant les informations pour créer le joueur
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool PlayerComputer::initialiser( const XmlElement* element )
{
	if(!PlayerAbstract::initialiser(element))
		return false;
	int elem;
	if( element->QueryIntAttribute("vitesse", &elem) != TIXML_SUCCESS )
		return false;
	modifierVitesse(elem);
	if( element->QueryIntAttribute("probEchec", &elem) != TIXML_SUCCESS )
		return false;
	modifierProbabiliteEchec(elem);
	type_ = JOUEUR_VIRTUEL;
	
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurVirtuel::genererAleatoirement()
///
/// Permet de generer des informations aleatoire pour un joueur,
/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PlayerComputer::genererAleatoirement()
{
	modifierNom("Joueur Virtuel Aleatoire");
	modifierVitesse(rand()%100);
	modifierProbabiliteEchec(rand()%100);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurAbstrait* JoueurVirtuel::obtenirCopie()
///
/// Permet d'obtenir une copie de ce joueur
///
///
/// @return JoueurAbstrait*
///
////////////////////////////////////////////////////////////////////////
// JoueurAbstrait* JoueurVirtuel::obtenirCopie()
// {
// 	return new JoueurVirtuel(obtenirNom(),obtenirVitesse(),obtenirProbabiliteEchec());
// }
////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 JoueurVirtuel::obtenirDirectionAI( NoeudRondelle* rondelle, NoeudMaillet* maillet )
///
/// Permet de savoir la nouvelle direction du maillet du JV selon AIMaillet
///
/// @param[in] NoeudRondelle * rondelle : un pointeur sur la rondelle 
/// pour savoir de quel côté de la ligne elle est
///
/// @return Vecteur2 :  la nouvelle direction à prendre pour le maillet
///
////////////////////////////////////////////////////////////////////////
Vecteur2 PlayerComputer::obtenirDirectionAI( NoeudMaillet* maillet )
{
	//envoie le pointeur sur la rondelle et sur le maillet
	aiMaillet_->evaluerStrategie(maillet);
	return aiMaillet_->evaluerDirection(maillet);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void JoueurVirtuel::setAiMaillet( AIMaillet* val )
///
/// /*Description*/
///
/// @param[in] AIMaillet * val
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PlayerComputer::setAiMaillet( AIMaillet* val )
{
    if(aiMaillet_)
    {
        delete aiMaillet_;
    }
    aiMaillet_ = val;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



