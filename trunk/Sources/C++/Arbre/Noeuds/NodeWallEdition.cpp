///////////////////////////////////////////////////////////////////////////////
/// @file NodeWallEdition.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NodeWallEdition.h"
#include "Utilitaire.h"
#include "NodeControlPoint.h"
#include "VisiteurNoeud.h"

#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeWallEdition::NodeWallEdition( NoeudPoint* n1, NoeudPoint* n2 )
///
/// Constructeur par paramètre.
///
/// @param[in] NoeudPoint * n1 : premier point associé
/// @param[in] NoeudPoint * n2 : deuxieme point associé
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeWallEdition::NodeWallEdition( const std::string& type ):
    Super(type),ControlPointMutableAbstract(2)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeWallEdition::~NodeWallEdition()
///
/// destructor
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeWallEdition::~NodeWallEdition()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::ajouter( NoeudAbstrait* enfant )
///
/// Ajoute un noeud enfant.
///
/// @param[in] NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallEdition::ajouter( NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = dynamic_cast<NodeControlPoint*>(enfant);
    if(controlPoint)
    {
        return addControlPoint(controlPoint);
    }
    return Super::ajouter(enfant);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::detacherEnfant( const NoeudAbstrait* enfant )
///
/// /*Description*/
///
/// @param[in] const NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::detacherEnfant( const NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = const_cast<NodeControlPoint*>(dynamic_cast<const NodeControlPoint*>(enfant));
    if(controlPoint)
    {
        removeControlPoint(controlPoint);
    }
    else
    {
        Super::detacherEnfant(enfant);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::modifierEchelle( float facteur )
///
/// /*Description*/
///
/// @param[in] float facteur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::modifierEchelle( float facteur )
{
    Vecteur3 echelle;
    obtenirEchelleCourante(echelle);
    echelle[VY] *= facteur;
    modifierEchelleCourante(echelle);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::afficherConcret()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::afficherConcret() const
{
    Super::afficherConcret();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudMuret::creerNoeudXML()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NodeWallEdition::creerNoeudXML()
{
    XmlElement* elementNoeud = Super::creerNoeudXML();
    return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudMuret::initialiser( const XmlElement* element )
///
/// Initialisation du NoeudMuret à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallEdition::initialiser( const XmlElement* element )
{
    if(!Super::initialiser(element))
        return false;

    int count = 0;
    // Lecture des position a partir de point de control
    for( auto child = XMLUtils::FirstChildElement(element); child; child = XMLUtils::NextSibling(child) )
    {
        auto name = XMLUtils::GetNodeTag(child);
        if( name != RazerGameUtilities::NAME_CONTROL_POINT)
        {
            throw ExceptionJeu("Wall Node: unrecognized xml node: %s",name);
        }
        NodeControlPoint* point = new NodeControlPoint(name);
        point->initialiser(child);
        // on fait l'ajout apres pour mettre a jour limité le nombre de fois qu'on mets a jour les propriétés du noeud.
        if(!addControlPoint(point))
        {
            delete point;
        }
        else
        {
            ++count;
        }
    }

    if(count != 2)
    {
        throw ExceptionJeu("Wall Node: missing control node to initialise");
    }

    return true;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::onAddControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallEdition::onAddControlPoint( NodeControlPoint* point )
{
    if(Super::ajouter(point))
    {
        point->attach(this);
        coins_[getNBControlPoint()-1] = &point->getPosition();
        updateWallProperties();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::onRemoveControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::onRemoveControlPoint( NodeControlPoint* point )
{
    Super::detacherEnfant(point);
    point->detach(this);
    coins_[getNBControlPoint()] = NULL;
    updateWallProperties();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMuret::acceptVisitor( const VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::acceptVisitor( VisiteurNoeud& v )
{
    v.visiterNoeudMuretEdition(this);
}

#ifdef MIKE_BUILD
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////