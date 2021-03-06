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
#include "NoeudMaillet.h"
#include "AIMaillet.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurVirtuel::JoueurVirtuel(std::string nom, float vitesse, int probabiliteEchec)
///
/// Constructeur qui initialisela le joueur virtuel.
///
/// @param[in] nom : le nom du joueur.
/// @param[in] vitesse : la vitesse de d�placement du joueur.
/// @param[in] probabiliteEchec : la probabilit� que le joueur manque son coup.
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
/// Accesseur � la vitesse du joueur.
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
/// Accesseur � la probabilit� d'�chec du joueur.
///
/// @return la probabilit� d'�chec du joueur
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
/// Modificateur de la probabilit� d'�chec du joueur.
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
/// Initialisaiton du joueur � partir d'un element XML
///
/// @param[in] const XmlElement * element : �l�ment contenant les informations pour cr�er le joueur
///
/// @return bool Vrai si l'initialisation � bien �t� faite
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
/// @fn Vecteur2 JoueurVirtuel::obtenirDirectionAI( NoeudRondelle* rondelle, NoeudMaillet* maillet )
///
/// Permet de savoir la nouvelle direction du maillet du JV selon AIMaillet
///
/// @param[in] NoeudRondelle * rondelle : un pointeur sur la rondelle 
/// pour savoir de quel c�t� de la ligne elle est
///
/// @return Vecteur2 :  la nouvelle direction � prendre pour le maillet
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

void PlayerComputer::PlayTick( float time )
{
	auto maillet = getControlingMallet();
	if(maillet)
	{
		auto newPos = obtenirDirectionAI(maillet);
		newPos += maillet->getPosition();
		maillet->setTargetDestination(newPos);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



