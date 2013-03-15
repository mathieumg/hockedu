///////////////////////////////////////////////////////////////////////////
/// @file NodeControlPoint.cpp
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////

#include "NodeControlPoint.h"
#include "VisiteurNoeud.h"
#include <algorithm>

#include "Utilitaire.h"
#include "ControlPointMutableAbstract.h"

#if BOX2D_INTEGRATED
#include "DebugRenderBox2D.h"
#include "FacadeModele.h"
#endif
#include "VisiteurCollision.h"

#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

CreateListDelegateImplementation(ControlPoint)
{

    GLuint liste = NULL;
#if WIN32
    liste = glGenLists(1);
    glNewList(liste, GL_COMPILE);
#if BOX2D_INTEGRATED
        DebugRenderBox2D* debugRender = DebugRenderBox2D::mInstance;
        debugRender->DrawSolidCircle(b2Vec2(0,0),0.5,b2Vec2(0,0),b2Color(1,0,1));
#endif
    glEndList();
#endif
    return liste;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeControlPoint::NodeControlPoint( const std::string& typeNoeud )
///
/// /*Description*/
///
/// @param[in] const std::string & typeNoeud
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeControlPoint::NodeControlPoint( const std::string& typeNoeud ):
Super(typeNoeud),mCanBeVisited(true)
{
    setDefaultRadius(8);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeControlPoint::~NodeControlPoint()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeControlPoint::~NodeControlPoint()
{

}

void NodeControlPoint::renderReal() const
{
    glTranslatef(0, 0, 0.5);
    glColor4f(1.0f,0.84f,0.0f,1.0f);
    // Appel à la version de la classe de base pour l'affichage des enfants.
    NoeudAbstrait::renderReal();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::tick( const float& temps )
///
/// /*Description*/
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::tick( const float& temps )
{
    Super::tick(temps);
//     mAngle = (float)((int)(mAngle+temps*500.0f)%360);
//     updateMatrice();
// 
//     glPushMatrix();
//     glLoadMatrixd(matrice_); // Chargement de la matrice du noeud
//     glRotated(90, 1.0, 0.0, 0.0);
//     glGetDoublev(GL_MODELVIEW_MATRIX, matrice_); // Savegarde de la matrice courante dans le noeud
//     glPopMatrix(); // Recuperation de la matrice d'origine
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudPoint::createXmlNode()
///
/// Creation du noeud XML d'un point
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NodeControlPoint::createXmlNode()
{
    XmlElement* elementNoeud = XMLUtils::createNode(mType.c_str());

    XmlWriteNodePosition(elementNoeud);

    return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudPoint::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudPoint à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeControlPoint::initFromXml( const XmlElement* element )
{
    // faire l'initialisaiton des attribut concernant le point en premier pour que la suite puisse les utiliser
    Vecteur3 pos;
    if( !XmlReadNodePosition(pos,element) )
        throw ExceptionJeu("%s: Error reading node's position", mType.c_str());
    setPosition(pos);
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::acceptVisitor( VisiteurNoeud& v )
///
/// /*Description*/
///
/// @param[in] VisiteurNoeud & v
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::acceptVisitor( VisiteurNoeud& v )
{
    v.visiterNodeControlPoint(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::flagSelectedAssociatedPoints()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::flagSelectedAssociatedPoints() const
{
    for(auto i = mAssociatedPoints.begin(); i!= mAssociatedPoints.end(); ++i)
        {
            NodeControlPoint* point = *i;
            point->flagVisitationIfSelected();
        }
}

// permet d'empecher d'appeler assignerPositionAbsolue a partir de setPosition
// lorsque setPosition est appelée par assignerPositionAbsolue
bool PositionScopedBool = false;

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::setPosition( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::setPosition( const Vecteur3& positionRelative )
{
    Super::setPosition(positionRelative);
    signalObservers();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::setCollisionVisitorAttributes( VisiteurCollision* v )
///
/// Permet d'assigner les attribut nécessaire à la collision.
///
/// @param[in] VisiteurCollision * v : Visiteur ayant besoin d'avoir ses paramètres assigne.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::setCollisionVisitorAttributes( class VisiteurCollision* v )
{
    auto n = dynamic_cast<NoeudAbstrait*>(mLinkedObject);
    if(n)
    {
        v->setNoeudAVerifier(n);
        n->setCollisionVisitorAttributes(v);
    }
    else
    {
        Super::setCollisionVisitorAttributes(v);
    }
}


#ifdef MIKE_DEBUG
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
