///////////////////////////////////////////////////////////////////////////////
/// @file ZoneEdition.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "ZoneEdition.h"
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include "XMLUtils.h"
#include "FacadeModele.h"

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
/// Permet de modifier les limites de la zone d'edition de facon relative � une table
///
/// @param[in] const float * boiteEnglobantTable : Tableau a 2 dimension contenant la longueur et hauteur max d'une table
///													boiteEnglobantTable[0] : hauteur
///													boiteEnglobantTable[1] : largeur
/// @param[in] const float & longueur : nouvelle longueur � assigner � la zone d'�dition
/// @param[in] const float & largeur : nouvelle largeur � assigner � la zone d'�dition
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
/// @fn XmlElement* ZoneEdition::creerNoeudXML()
///
/// Creation du noeud XML de la zone d'�dition
///
///
/// @return XmlElement* : le noeud XML avec la propri�t� de la zone
///
////////////////////////////////////////////////////////////////////////
XmlElement* ZoneEdition::creerNoeudXML()
{
	XmlElement* element = XMLUtils::createNode("ZoneEdition");

    XMLUtils::writeAttribute(element,"ExtLar",limiteExtLargeur_);
    XMLUtils::writeAttribute(element,"ExtLon",limiteExtLongueur_);
    XMLUtils::writeAttribute(element,"IntLar",limiteIntLargeur_);
    XMLUtils::writeAttribute(element,"IntLon",limiteIntLongueur_);

	return element;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ZoneEdition::initialisationXML( XmlElement* element )
///
/// Initialistion de la zone d'�dition � partir d'un noeud XML
///
/// @param[in] XmlElement * : noeud contenant les informations de la zone d'�dition
///
/// @return bool vrai si l'initialisation c'est bien pass�
///
////////////////////////////////////////////////////////////////////////
bool ZoneEdition::initialisationXML( const XmlElement* element )
{
	const XmlElement* zone = XMLUtils::FirstChildElement(element,"ZoneEdition");

	if(!zone)
		return false;

    if(!XMLUtils::readAttribute(zone,"ExtLar",limiteExtLargeur_) ) return false;
    if(!XMLUtils::readAttribute(zone,"ExtLon",limiteExtLongueur_)) return false;
    if(!XMLUtils::readAttribute(zone,"IntLar",limiteIntLargeur_) ) return false;
    if(!XMLUtils::readAttribute(zone,"IntLon",limiteIntLongueur_)) return false;

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
    // �tats de la lumi�re 
    GLboolean lighting_state;
    // D�sactiver l'�clairage
    glGetBooleanv(GL_LIGHTING, &lighting_state);
    glDisable(GL_LIGHTING);
    FacadeModele::getInstance()->DeActivateShaders();

	// Dessin de la zone d'�dition
    glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(0,0.749f,1,1);
	glBegin(GL_QUADS);
	glVertex3d(limiteExtLongueur_, limiteExtLargeur_, -2);
	glVertex3d(-limiteExtLongueur_, limiteExtLargeur_, -2);
	glVertex3d(-limiteExtLongueur_, -limiteExtLargeur_, -2);
	glVertex3d(limiteExtLongueur_, -limiteExtLargeur_, -2);
	glEnd();
	glPopAttrib();
    glPopMatrix();
    FacadeModele::getInstance()->ActivateShaders();

    // R�activer l'�clairage et (s'il y a lieu)
    if (lighting_state == GL_TRUE) {
        glEnable(GL_LIGHTING);
    }
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


