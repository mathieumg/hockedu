///////////////////////////////////////////////////////////////////////////////
/// @file ZoneEdition.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "ZoneEdition.h"
#include "tinyxml.h"
#include <windows.h>
#include "glew.h"
#include "XMLUtils.h"

const float ZoneEdition::DEFAUT_LIMITE_INT_LONGUEUR = 75;
const float ZoneEdition::DEFAUT_LIMITE_INT_LARGEUR = 50;
const float ZoneEdition::DEFAUT_LIMITE_EXT_LARGEUR = 150; 
const float ZoneEdition::DEFAUT_LIMITE_EXT_LONGUEUR = 200;

////////////////////////////////////////////////////////////////////////
///
/// @fn  ZoneEdition::ZoneEdition()
///
/// Constructeur par defaut
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ZoneEdition::ZoneEdition()
{
	limiteIntLongueur_ = DEFAUT_LIMITE_INT_LONGUEUR;
	limiteIntLargeur_  = DEFAUT_LIMITE_INT_LARGEUR ;
	limiteExtLargeur_  = DEFAUT_LIMITE_EXT_LARGEUR ; 
	limiteExtLongueur_ = DEFAUT_LIMITE_EXT_LONGUEUR;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::modifierLimitesExt( const float* boiteEnglobantTable, const float& longueur,const float& largeur )
///
/// Permet de modifier les limites de la zone d'edition de facon relative à une table
///
/// @param[in] const float * boiteEnglobantTable : Tableau a 2 dimension contenant la longueur et hauteur max d'une table
///													boiteEnglobantTable[0] : hauteur
///													boiteEnglobantTable[1] : largeur
/// @param[in] const float & longueur : nouvelle longueur à assigner à la zone d'édition
/// @param[in] const float & largeur : nouvelle largeur à assigner à la zone d'édition
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ZoneEdition::modifierLimitesExt( const float* boiteEnglobantTable, const float& longueur,const float& largeur )
{
	// Si la longueur a assigner est plus petite que la longueur maximale de la table, on cap a la longueur de la table
	if(longueur< boiteEnglobantTable[1] )
	{
		limiteExtLongueur_=boiteEnglobantTable[1];

	}
	// si plus grand ou egale
	else
	{
		limiteExtLongueur_=longueur;
	}

	// Si la largeur a assigner est plus petite que la largeur maximale de la table, on cap a la largeur de la table
	if(largeur< boiteEnglobantTable[0] )
	{
		limiteExtLargeur_=boiteEnglobantTable[0];
	}
	// si plus grand ou egale
	else
	{
		limiteExtLargeur_=largeur;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn TiXmlElement* ZoneEdition::creerNoeudXML()
///
/// Creation du noeud XML de la zone d'édition
///
///
/// @return TiXmlElement* : le noeud XML avec la propriété de la zone
///
////////////////////////////////////////////////////////////////////////
TiXmlElement* ZoneEdition::creerNoeudXML()
{
	TiXmlElement* element = XMLUtils::creerNoeud("ZoneEdition");

    XMLUtils::ecrireAttribute(element,"ExtLar",limiteExtLargeur_);
    XMLUtils::ecrireAttribute(element,"ExtLon",limiteExtLongueur_);
    XMLUtils::ecrireAttribute(element,"IntLar",limiteIntLargeur_);
    XMLUtils::ecrireAttribute(element,"IntLon",limiteIntLongueur_);

	return element;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ZoneEdition::initialisationXML( TiXmlElement* element )
///
/// Initialistion de la zone d'édition à partir d'un noeud XML
///
/// @param[in] TiXmlElement * : noeud contenant les informations de la zone d'édition
///
/// @return bool vrai si l'initialisation c'est bien passé
///
////////////////////////////////////////////////////////////////////////
bool ZoneEdition::initialisationXML( const TiXmlElement* element )
{
	const TiXmlElement* zone = element->FirstChildElement("ZoneEdition");

	if(!zone)
		return false;

    if(!XMLUtils::LireAttribute(zone,"ExtLar",limiteExtLargeur_) ) return false;
    if(!XMLUtils::LireAttribute(zone,"ExtLon",limiteExtLongueur_)) return false;
    if(!XMLUtils::LireAttribute(zone,"IntLar",limiteIntLargeur_) ) return false;
    if(!XMLUtils::LireAttribute(zone,"IntLon",limiteIntLongueur_)) return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::reinitialiser()
///
/// Remet la zone d'edition a son etat par defaut
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ZoneEdition::reinitialiser()
{
	limiteIntLongueur_ = DEFAUT_LIMITE_INT_LONGUEUR;
	limiteIntLargeur_  = DEFAUT_LIMITE_INT_LARGEUR ;
	limiteExtLargeur_  = DEFAUT_LIMITE_EXT_LARGEUR ; 
	limiteExtLongueur_ = DEFAUT_LIMITE_EXT_LONGUEUR;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ZoneEdition::afficher()
///
/// Effectue l'affichage de la zone d'edition
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ZoneEdition::afficher()
{


	// Dessin de la zone d'édition
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glBegin(GL_QUADS);
	glColor4f(0,0.749f,1,1);
	glVertex3d(limiteExtLongueur_, limiteExtLargeur_, -2);
	glVertex3d(-limiteExtLongueur_, limiteExtLargeur_, -2);
	glVertex3d(-limiteExtLongueur_, -limiteExtLargeur_, -2);
	glVertex3d(limiteExtLongueur_, -limiteExtLargeur_, -2);
	glEnd();
	glPopAttrib();
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


