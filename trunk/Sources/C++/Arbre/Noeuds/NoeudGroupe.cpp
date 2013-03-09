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
NoeudGroupe::NoeudGroupe(std::string type, std::string typeEnfants):
NoeudComposite(type),typeEnfants_(typeEnfants)
{
	assignerEstSelectionnable(false);
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
/// @fn XmlElement* NoeudGroupe::creerNoeudXML()
///
/// Retourne un élément XML pour le groupe de noeuds.
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudGroupe::creerNoeudXML()
{
	XmlElement* element = XMLUtils::createNode(type_.c_str());

    XMLUtils::writeAttribute(element,"TypeEnfants",typeEnfants_);
	
	return element;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudGroupe::initialiser( const XmlElement* element )
///
/// Initialisation du NoeudGroupe à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudGroupe::initialiser( const XmlElement* element )
{
    if(!XMLUtils::readAttribute(element,"TypeEnfants",typeEnfants_))
        return false;

    const ArbreRendu* treeRoot = GetTreeRoot();
    if(treeRoot)
    {
        for( auto child = XMLUtils::FirstChildElement(element); child; child = XMLUtils::NextSibling(child) )
        {
            auto name = XMLUtils::GetNodeTag(child);
            if(name != typeEnfants_)
            {
                throw ExceptionJeu("Attempting to create a node in a group that it doesn't belong",name);
            }
            auto node = treeRoot->creerNoeud(name);
            ajouter(node);
            if(!node)
            {
                throw ExceptionJeu("Error creating node : %s",name);
            }
            node->initialiser(child);
        }
    }
    else
    {
        throw ExceptionJeu("%s : Missing tree root",type_.c_str());
    }

	return true;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



