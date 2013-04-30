//////////////////////////////////////////////////////////////////////////////
/// @file JoueurAbstrait.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "PlayerAbstract.h"
#include "PlayerHuman.h"
#include "PlayerComputer.h"
#include "XMLUtils.h"
#include "PlayerNetwork.h"
#include "PlayerReinforcementAI.h"
#include "NoeudMaillet.h"

const std::string PlayerAbstract::etiquetteJoueur = "Joueur";
const std::string PlayerAbstract::etiquetteType = "Type";

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurAbstrait::JoueurAbstrait(std::string nom)
///
/// Constructeur qui initialisela le joueur.
///
/// @param[in] nom : le nom du joueur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerAbstract::PlayerAbstract(const std::string& nom): nom_(nom), mControlingMallet(NULL)
{}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string JoueurAbstrait::obtenirNom()
///
/// Accesseur au nom du joueur.
///
/// @return nom_ : le nom du joueur.
///
////////////////////////////////////////////////////////////////////////
std::string PlayerAbstract::obtenirNom() const
{
	return nom_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string JoueurAbstrait::modifierNom()
///
/// Modificateur du nom du joueur.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void PlayerAbstract::modifierNom(const std::string nom)
{
	nom_ = nom;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn JoueurAbstrait::~JoueurAbstrait( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
PlayerAbstract::~PlayerAbstract( void )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* JoueurAbstrait::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return XmlElement* le noeud XML du joueur
///
////////////////////////////////////////////////////////////////////////
XmlElement* PlayerAbstract::creerNoeudXML() const
{
	XmlElement* elementNoeud = XMLUtils::createNode(etiquetteJoueur.c_str());

	XmlElement* etiquetteNom = XMLUtils::createNode("nom");
    XmlText* nom = XMLUtils::createTextNode(obtenirNom().c_str());
    XMLUtils::LinkEndChild(etiquetteNom,(XmlElement*)nom);
    XMLUtils::LinkEndChild(elementNoeud,etiquetteNom);

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurAbstrait::initialiser( const XmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
///
/// @param[in] const XmlElement * element : élément contenant les informations pour créer le joueur
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool PlayerAbstract::initialiser( const XmlElement* element )
{
	modifierNom(extraireNomXmlNode(element));
	
	return nom_ != "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string JoueurAbstrait::extraireNomXmlNode( const XmlElement* element )
///
/// Outils pour connaitre le nom d'un joueur contenu dans un noeud XML
///
/// @param[in] const XmlElement * element
///
/// @return std::string : le nom contenu dans le noeud si succes, un chaine vide si il y a une erreur
///
////////////////////////////////////////////////////////////////////////
std::string PlayerAbstract::extraireNomXmlNode( const XmlElement* element )
{
	const XmlElement* node = XMLUtils::FirstChildElement(element,"nom");
	if(!node)
		return "";
	const XmlNode* etiquetteNom = node->FirstChild();
	if(!etiquetteNom)
		return "";
	const XmlText* nom = etiquetteNom->ToText();
	if(!nom )
		return "";
	return nom->Value();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SPJoueurAbstrait JoueurAbstrait::usineJoueurXML( const XmlElement* element, ConteneurJoueur* profilsExistant )
///
/// Permet d'obtenir un pointeur sur un joueur à partir d'un élément XML
///
/// @param[in] const XmlElement * element	: élément XML contenant les informations du joueur
/// @param[in] ConteneurJoueur * profilsExistant : Conteneur ayant les profils de joueur virtuel deja existant, 
///												   pointeur null si on ne veut pas faire la vérification
///
/// @return SPJoueurAbstrait : pointeur du joueur si succes, null si echec
///
////////////////////////////////////////////////////////////////////////
SPPlayerAbstract PlayerAbstract::usineJoueurXML( const XmlElement* element, ConteneurJoueur* profilsExistant /*= 0*/ )
{
	if(!element)
		return nullptr;

	TiXmlString nom = element->ValueTStr();
	// Vérifie si l'étiquette est correcte
	if(nom == PlayerAbstract::etiquetteJoueur.c_str())
	{
		int type;
		// Crée le joueur que si l'attribut type est présent
		if( element->QueryIntAttribute(PlayerAbstract::etiquetteType.c_str(), &type) == TIXML_SUCCESS )
		{
			// Pointeur Intelligent, on ne s'occupe plus de liberer la memoire de ceux-ci
			SPPlayerAbstract joueur;
			switch(type)
			{
			case JOUEUR_HUMAIN: joueur = SPPlayerAbstract(new PlayerHuman()); break;
			case JOUEUR_VIRTUEL:
				{
					ConteneurJoueur::iterator iter;
					if(profilsExistant && ( iter = profilsExistant->find(extraireNomXmlNode(element) ) ) != profilsExistant->end())
					{
						return iter->second;
					}
					else
						joueur = SPPlayerAbstract(new PlayerComputer()); break;
				}				
            case JOUEUR_NETWORK:
                {
                    joueur = SPPlayerAbstract(new PlayerNetwork()); break;
                }
            case JOUEUR_VIRTUEL_RENFORCEMENT:
            {
                joueur = SPPlayerAbstract(new PlayerReinforcementAI()); break;
            }
			default: break;
			}
			if(joueur->initialiser(element))
			{
				// Si l'initialisation du joueur c'est bien derouler on le retourne
				return joueur;
			}
		}
	}
	return nullptr;
}

void PlayerAbstract::setControlingMallet( class NoeudMaillet* pVal )
{
    /// refuse la modification si on tente de controller un maillet
    /// deja controllé ou si on le controle deja
    checkf(!pVal || !pVal->getPlayer());
    if(mControlingMallet == pVal || (pVal &&!!pVal->getPlayer()) )
    {
        return;
    }
    auto oldMallet = mControlingMallet;
    mControlingMallet = pVal;
    // update old pointers
	if(oldMallet)
	{
		oldMallet->setPlayer(NULL);
	}
    if(mControlingMallet)
    {
        mControlingMallet->setPlayer(shared_from_this());
    }
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



