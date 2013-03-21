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

#if WIN32
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include "FacadeModele.h"
#elif __APPLE__
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "RazerGameUtilities.h"
#endif

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
/// @fn XmlElement* ZoneEdition::creerNoeudXML()
///
/// Creation du noeud XML de la zone d'édition
///
///
/// @return XmlElement* : le noeud XML avec la propriété de la zone
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
/// Initialistion de la zone d'édition à partir d'un noeud XML
///
/// @param[in] XmlElement * : noeud contenant les informations de la zone d'édition
///
/// @return bool vrai si l'initialisation c'est bien passé
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
#if WIN32
    // États de la lumière 
    GLboolean lighting_state;
    // Désactiver l'éclairage
    glGetBooleanv(GL_LIGHTING, &lighting_state);
    glDisable(GL_LIGHTING);
    FacadeModele::getInstance()->DeActivateShaders();
    glEnableClientState(GL_VERTEX_ARRAY);
#endif

	// Dessin de la zone d'édition
    glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(0,0.749f,1,1);
    GLfloat vertices[12] =
    {
        limiteExtLongueur_, limiteExtLargeur_,-3,
        -limiteExtLongueur_, limiteExtLargeur_, -3,
        -limiteExtLongueur_, -limiteExtLargeur_, -3,
        limiteExtLongueur_, -limiteExtLargeur_, -3
    };
    glVertexPointer (3, GL_FLOAT , 0, vertices); 
    glDrawArrays (GL_TRIANGLE_FAN, 0, 4);

	glPopAttrib();
    glPopMatrix();
#if WIN32
    glDisableClientState(GL_VERTEX_ARRAY);
    FacadeModele::getInstance()->ActivateShaders();

    // Réactiver l'éclairage et (s'il y a lieu)
    if (lighting_state == GL_TRUE) {
        glEnable(GL_LIGHTING);
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ZoneEdition::equals( ZoneEdition * zone )
///
/// /*Description*/
///
/// @param[in] ZoneEdition * zone
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool ZoneEdition::equals( ZoneEdition * zone )
{
    if(!zone)
    {
        return false;
    }
    if(zone == this)
    {
        return true;
    }

    return
    limiteIntLongueur_ == zone->limiteIntLongueur_ &&
    limiteIntLargeur_  == zone->limiteIntLargeur_  &&
    limiteExtLargeur_  == zone->limiteExtLargeur_  &&
    limiteExtLongueur_ == zone->limiteExtLongueur_ ;
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


