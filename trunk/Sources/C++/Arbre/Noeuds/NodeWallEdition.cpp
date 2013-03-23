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

#if MIKE_DEBUG_
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
/// @fn bool NodeWallEdition::add( NoeudAbstrait* enfant )
///
/// Ajoute un noeud enfant.
///
/// @param[in] NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallEdition::add( NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = dynamic_cast<NodeControlPoint*>(enfant);
    if(controlPoint)
    {
        return addControlPoint(controlPoint);
    }
    return Super::add(enfant);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::unlinkChild( const NoeudAbstrait* enfant )
///
/// /*Description*/
///
/// @param[in] const NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::unlinkChild( const NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = const_cast<NodeControlPoint*>(dynamic_cast<const NodeControlPoint*>(enfant));
    if(controlPoint)
    {
        removeControlPoint(controlPoint);
    }
    else
    {
        Super::unlinkChild(enfant);
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
    getScale(echelle);
    echelle[VY] *= facteur;
    setScale(echelle);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::renderReal()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::renderReal() const
{
    Super::renderReal();
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
XmlElement* NodeWallEdition::createXmlNode()
{
    XmlElement* elementNoeud = Super::createXmlNode();
    return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudMuret::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudMuret à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallEdition::initFromXml( const XmlElement* element )
{
    if(!Super::initFromXml(element))
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
        point->initFromXml(child);
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
    if(Super::add(point))
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
    Super::unlinkChild(point);
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

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::updateCornerPosition()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::updateCornerPosition()
{
    mUpdatingCornerPosition= true;

    NodeControlPoint* p1 = getControlPoint(0);
    NodeControlPoint* p2 = getControlPoint(1);

    if(p1 && p2)
    {
        Vecteur3 deplacement( cos(utilitaire::DEG_TO_RAD(mAngle) ), sin(utilitaire::DEG_TO_RAD(mAngle) ) );
        deplacement *= mScale[VX];
        deplacement /= 2.0;

        p1->setPosition(mPosition+deplacement);
        p2->setPosition(mPosition-deplacement);
    }

    if(!isSyncFromB2Callback())
    {
        updatePhysicBody();
    }
    mUpdatingCornerPosition= false;
}

#if MIKE_DEBUG_
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////