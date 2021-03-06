///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMuret.cpp
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudMuret.h"
#include "Utilitaire.h"
#include "XMLUtils.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudMuret::NoeudMuret( const std::string& typeNoeud, bool estSurTable )
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par d�faut aux variables membres.
///
/// @param[in] const std::string & typeNoeud : Le type du noeud.
/// @param[in] bool estSurTable : si le muret est sur la table ou si c'est une bordure
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuret::NoeudMuret( )
	: Super(RAZER_KEY_WALL)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::~NoeudMuret()
///
/// Ce destructeur d�sallouee la liste d'affichage du muret.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMuret::~NoeudMuret()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirCoin1()
///
/// Retourne la position du coin1.
///
/// @param[in] void
///
/// @return Vecteur3 positionCoin1 : vecteur contenant la position.
///
////////////////////////////////////////////////////////////////////////
const Vecteur3& NoeudMuret::obtenirCoin1() const
{
	return positionCoin1_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::obtenirCoin2()
///
/// Retourne la position du coin2.
///
/// @param[in] void
///
/// @return Vecteur3 positionCoin2 : vecteur contenant la position.
///
////////////////////////////////////////////////////////////////////////
const Vecteur3& NoeudMuret::obtenirCoin2() const
{
	return positionCoin2_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMuret::updateCornerPosition(  )
///
/// Permet de recalculer la position des coin des muret suite a une rotation ou une mise a l'�chelle
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::updateCornerPosition()
{
	Vecteur3 deplacement( cos(utilitaire::DEG_TO_RAD(mAngle) ), sin(utilitaire::DEG_TO_RAD(mAngle) ) );
	deplacement*= mScale[VX];
	deplacement /= 2.0;
	positionCoin1_ = mPosition+deplacement;
	positionCoin2_ = mPosition-deplacement;
    if(!isWorldLocked())
    {
        updatePhysicBody();
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::assignerPositionCoin( int lequel, Vecteur3 position )
///
/// Modifie la position des coins
///
/// @param[in] int			lequel		: lequel des deux coins ( 1 ou 2)
///				Vecteur3	position	: position a assigner
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMuret::assignerPositionCoin( int lequel, Vecteur3 position )
{
	if(lequel==1)
	{
		positionCoin1_ = position;
	}
	else if(lequel==2)
	{
		positionCoin2_ = position;
	}

	updateWallProperties();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudMuret::createXmlNode()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudMuret::createXmlNode()
{
	XmlElement* elementNoeud = Super::createXmlNode();
	// Ajouter la position des coins des murets
    XMLUtils::writeArray(positionCoin1_.c_arr(),3,elementNoeud,"coinA");
    XMLUtils::writeArray(positionCoin2_.c_arr(),3,elementNoeud,"coinB");
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudMuret::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudMuret � partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudMuret::initFromXml( const XmlElement* element )
{
	if(!Super::initFromXml(element))
		return false;

    // Lecture des position a partir de point de control
    int cornerNumber=0;
    Vecteur3* cornerPos[2] = {&positionCoin1_,&positionCoin2_};
    for( auto child = XMLUtils::FirstChildElement(element); child; child = XMLUtils::NextSibling(child) )
    {
        int key;
        if( cornerNumber>1 || XMLUtils::readAttribute( child, ETIQUETTE_KEY, key ) && key != RAZER_KEY_CONTROL_POINT )
        {
            throw ExceptionJeu( "Wall Node: unrecognized xml node: %s", RazerGameUtilities::KeyToString( (RazerKey)key ) );
        }
        Vecteur3 pos;
        if(!XmlReadNodePosition(pos,child))
            throw ExceptionJeu( "Error reading wall's position", RazerGameUtilities::KeyToString( (RazerKey)key ) );
        *cornerPos[cornerNumber++] = pos;
    }

    // Lecture normal des positions si les points de control ne sont pas present
    if(cornerNumber != 2)
    {
        if( !XMLUtils::readArray(positionCoin1_.c_arr(),3,element,"coinA") )
            return false;
        if( !XMLUtils::readArray(positionCoin2_.c_arr(),3,element,"coinB") )
            return false;
    }
    updateWallProperties();
	return true;
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////