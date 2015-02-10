///////////////////////////////////////////////////////////////////////////
/// @file NodeControlPoint.cpp
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////

#if BOX2D_INTEGRATED
#include <Box2D/Box2D.h>
#endif

#include "NodeControlPoint.h"
#include "VisiteurNoeud.h"
#include <algorithm>

#include "Utilitaire.h"
#include "ControlPointMutableAbstract.h"



#if BOX2D_DEBUG
#include "DebugRenderBox2D.h"
#endif
#include "VisiteurCollision.h"


const float DEFAULT_RADIUS = 5;

CreateListDelegateImplementation(ControlPoint)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,DEFAULT_RADIUS);


    /// ancien affichage avec seulement opengl
    GLuint liste = NULL;
#if WIN32
    liste = glGenLists(1);
    glNewList(liste, GL_COMPILE);
#if BOX2D_DEBUG
    DebugRenderBox2D* debugRender = DebugRenderBox2D::mInstance;
    debugRender->DrawSolidCircle(b2Vec2(0,0),0.5,b2Vec2(0,0),b2Color(1,0,1));
#endif
    glEndList();
#endif
    return liste;
}

static const float NODE_Z_BASE_POSITION = 20.f;

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
NodeControlPoint::NodeControlPoint():
Super(RAZER_KEY_CONTROL_POINT),mCanBeVisited(true),mHeightAngle(0)
{
    setDefaultRadius(DEFAULT_RADIUS);
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
    if(isVisible())
    {
        mHeightAngle += temps * 3;
        const float zTranslated = NODE_Z_BASE_POSITION + sin( mHeightAngle )*3.f;
        mPosition.Z() = zTranslated;
    }
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
    XmlElement* elementNoeud = XMLUtils::createNode(mNodeName.c_str());
    XMLUtils::writeAttribute( elementNoeud, ETIQUETTE_KEY, (int)mNodeKey );
    XmlWriteNodePosition(elementNoeud);
    XMLUtils::writeAttribute(elementNoeud,"selection",IsSelected());

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
        throw ExceptionJeu("%s: Error reading node's position", mNodeName.c_str());
    setPosition(pos);

    bool selected;
    if(XMLUtils::readAttribute(element,"selection",selected))
    {
        setSelection(selected);
    }


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
    mPosition[VZ] = NODE_Z_BASE_POSITION;
    PositionSubject::signalObservers();
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

void NodeControlPoint::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        if(!IsInGame())
        {
            b2BodyDef myBodyDef;
            myBodyDef.type = b2_dynamicBody;; //this will be a static body
            const Vecteur3& pos = getPosition();
            b2Vec2 posB2;
            utilitaire::VEC3_TO_B2VEC(pos,posB2);
            myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
            myBodyDef.angle = 0; //set the starting angle

            mPhysicBody = world->CreateBody(&myBodyDef);
            b2CircleShape circleShape;
            circleShape.m_p.Set(0, 0); //position, relative to body position
            circleShape.m_radius = (float32)getRadius()*utilitaire::ratioWorldToBox2D; //radius



            b2FixtureDef myFixtureDef;
            myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
            myFixtureDef.density = 1;

            RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_CONTROL_POINT,IsInGame());

            mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
            mPhysicBody->SetUserData(this);
        }
    }
#endif
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
