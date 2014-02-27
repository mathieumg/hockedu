///////////////////////////////////////////////////////////////////////////////
/// @file NoeudGroupe.cpp
/// @author Michael Ferris
/// @date 2012-03-02
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NoeudGroupe.h"
#include "XMLUtils.h"
#include "ArbreRendu.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudGroupe::NoeudGroupe( std::string type, std::string typeEnfants ) : NoeudComposite(type),typeEnfants_(typeEnfants)
///
/// Constructeur du groupe de noeuds.
///
/// @param[in] std::string type
/// @param[in] std::string typeEnfants
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NoeudGroupe::NoeudGroupe( const RazerKey typeEnfants ) :
NoeudComposite(RAZER_KEY_GROUP),typeEnfants_(typeEnfants)
{
    /// les noeuds points ne peuvent etre supprimer
    mFlags.SetFlag(false,NODEFLAGS_CAN_BE_DELETED);
	setCanBeSelected(false);
    setRecordable(false);
    setVisible(false);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudGroupe::~NoeudGroupe( void )
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NoeudGroupe::~NoeudGroupe(void)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudGroupe::createXmlNode()
///
/// Retourne un élément XML pour le groupe de noeuds.
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudGroupe::createXmlNode()
{
	XmlElement* element = XMLUtils::createNode(mType.c_str());
    XMLUtils::writeAttribute( element, "id", (int)mNodeKey );
    XMLUtils::writeAttribute( element,"TypeEnfants",(int)typeEnfants_);
	
	return element;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudGroupe::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudGroupe à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudGroupe::initFromXml( const XmlElement* element )
{
    int typeId;
    if( !XMLUtils::readAttribute( element, "TypeEnfants", typeId ) )
        return false;
    typeEnfants_ = (RazerKey)typeId;

    const ArbreRendu* treeRoot = GetTreeRoot();
    if(treeRoot)
    {
        for( auto child = XMLUtils::FirstChildElement(element); child; child = XMLUtils::NextSibling(child) )
        {
            if( !XMLUtils::readAttribute( child, "id", typeId ) || typeId != typeEnfants_ )
            {
                throw ExceptionJeu("Attempting to create a node in a group that it doesn't belong",typeId);
            }
            CreateAndInitNodesFromXml(child);
        }
    }
    else
    {
        throw ExceptionJeu("%s : Missing tree root",mType.c_str());
    }

	return true;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



