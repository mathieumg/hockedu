//////////////////////////////////////////////////////////////////////////////
/// @file JoueurAbstrait.cpp
/// @author Vincent Lemire
/// @date 2012-02-17
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "JoueurAbstrait.h"
#include "JoueurHumain.h"
#include "JoueurVirtuel.h"
#include "tinyxml.h"
#include "XMLUtils.h"

const std::string JoueurAbstrait::etiquetteJoueur = "Joueur";
const std::string JoueurAbstrait::etiquetteType = "Type";

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
JoueurAbstrait::JoueurAbstrait(const std::string& nom): nom_(nom), mControlingMallet(NULL)
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
std::string JoueurAbstrait::obtenirNom() const
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
void JoueurAbstrait::modifierNom(const std::string nom)
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
JoueurAbstrait::~JoueurAbstrait( void )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* JoueurAbstrait::creerNoeudXML()
///
/// Creation du noeud XML du joueur
///
///
/// @return TiXmlElement* le noeud XML du joueur
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* JoueurAbstrait::creerNoeudXML() const
{
	TiXmlElement* elementNoeud = XMLUtils::creerNoeud(etiquetteJoueur.c_str());

	TiXmlElement* etiquetteNom = XMLUtils::creerNoeud("nom");
    TiXmlText* nom = XMLUtils::creerNoeudText(obtenirNom().c_str());
    etiquetteNom->LinkEndChild(nom);
    XMLUtils::LinkEndChild(elementNoeud,etiquetteNom);

	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool JoueurAbstrait::initialiser( const TiXmlElement* element )
///
/// Initialisaiton du joueur à partir d'un element XML
///
/// @param[in] const TiXmlElement * element : élément contenant les informations pour créer le joueur
///
/// @return bool Vrai si l'initialisation à bien été faite
///
////////////////////////////////////////////////////////////////////////
bool JoueurAbstrait::initialiser( const TiXmlElement* element )
{
	nom_ = extraireNomXmlNode(element);
	
	return nom_ != "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string JoueurAbstrait::extraireNomXmlNode( const TiXmlElement* element )
///
/// Outils pour connaitre le nom d'un joueur contenu dans un noeud XML
///
/// @param[in] const TiXmlElement * element
///
/// @return std::string : le nom contenu dans le noeud si succes, un chaine vide si il y a une erreur
///
////////////////////////////////////////////////////////////////////////
std::string JoueurAbstrait::extraireNomXmlNode( const TiXmlElement* element )
{
	const TiXmlElement* node = element->FirstChildElement("nom");
	if(node == 0)
		return "";
	const TiXmlNode* etiquetteNom = node->FirstChild();
	if(etiquetteNom == 0)
		return "";
	const TiXmlText* nom = etiquetteNom->ToText();
	if(nom == 0)
		return "";
	return nom->Value();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SPJoueurAbstrait JoueurAbstrait::usineJoueurXML( const TiXmlElement* element, ConteneurJoueur* profilsExistant )
///
/// Permet d'obtenir un pointeur sur un joueur à partir d'un élément XML
///
/// @param[in] const TiXmlElement * element	: élément XML contenant les informations du joueur
/// @param[in] ConteneurJoueur * profilsExistant : Conteneur ayant les profils de joueur virtuel deja existant, 
///												   pointeur null si on ne veut pas faire la vérification
///
/// @return SPJoueurAbstrait : pointeur du joueur si succes, null si echec
///
////////////////////////////////////////////////////////////////////////
SPJoueurAbstrait JoueurAbstrait::usineJoueurXML( const TiXmlElement* element, ConteneurJoueur* profilsExistant /*= 0*/ )
{
	if(!element)
		return nullptr;

	TiXmlString nom = element->ValueTStr();
	// Vérifie si l'étiquette est correcte
	if(nom == JoueurAbstrait::etiquetteJoueur.c_str())
	{
		int type;
		// Crée le joueur que si l'attribut type est présent
		if( element->QueryIntAttribute(JoueurAbstrait::etiquetteType.c_str(), &type) == TIXML_SUCCESS )
		{
			// Pointeur Intelligent, on ne s'occupe plus de liberer la memoire de ceux-ci
			SPJoueurAbstrait joueur;
			switch(type)
			{
			case JOUEUR_HUMAIN: joueur = SPJoueurAbstrait(new JoueurHumain()); break;
			case JOUEUR_VIRTUEL:
				{
					ConteneurJoueur::iterator iter;
					if(profilsExistant && ( iter = profilsExistant->find(extraireNomXmlNode(element) ) ) != profilsExistant->end())
					{
						return iter->second;
					}
					else
						joueur = SPJoueurAbstrait(new JoueurVirtuel()); break;
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

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



