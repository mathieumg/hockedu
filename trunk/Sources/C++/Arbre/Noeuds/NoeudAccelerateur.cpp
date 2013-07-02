///////////////////////////////////////////////////////////////////////////////
/// @file NoeudAccelerateur.cpp
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudAccelerateur.h"
#include "NoeudRondelle.h"
#include "Utilitaire.h"
#include "VisiteurNoeud.h"
#include "XMLUtils.h"

#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif

#if MANUAL_PHYSICS_DETECTION
#include "Terrain.h"
#endif

#if WIN32

#endif


const float NoeudAccelerateur::DEFAULT_RADIUS = 7;

CreateListDelegateImplementation(Boost)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudAccelerateur::DEFAULT_RADIUS);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAccelerateur::NoeudAccelerateur( const std::string& typeNoeud )
///
/// Constructeur de la classe qui charge le modele dans la banque de modele.
///
/// @param[in] const std::string & typeNoeud : Le type de noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudAccelerateur::NoeudAccelerateur(const std::string& typeNoeud)
	: NoeudAbstrait(RAZER_KEY_BOOST,typeNoeud),bonusAccel_(1.50), activer_(true)
{
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAccelerateur::~NoeudAccelerateur(  )
///
/// Ce destructeur désallouee la liste d'affichage de l'accélérateur.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudAccelerateur::~NoeudAccelerateur()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::renderReal(  )
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::renderReal() const
{
	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudAbstrait::renderReal();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::tick( const float& temps )
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] float temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::tick( const float& temps)
{
	setAngle((float)((int)(mAngle+temps*500.0f)%360));
	updateMatrice();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'acceuillir les différents visiteur pour les transformations.
///
/// @param[in] VisiteurNoeud & v :  le visiteur qui fait la transformation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudAccelerateur(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudAccelerateur::createXmlNode()
///
/// Creation du noeud XML du joueur
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudAccelerateur::createXmlNode()
{
	XmlElement* elementNoeud = NoeudAbstrait::createXmlNode();

    XMLUtils::writeAttribute(elementNoeud,"bonusAccel",bonusAccel_);
	
	return elementNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudAccelerateur::initFromXml( const XmlElement* element )
///
/// Initialisation du NoeudAccelerateur à partir d'un element XML
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudAccelerateur::initFromXml( const XmlElement* element )
{
	if(!NoeudAbstrait::initFromXml(element))
		return false;
	auto doubleElem = bonusAccel_;
	if( !XMLUtils::readAttribute(element,"bonusAccel",doubleElem) )
        throw ExceptionJeu("Node Boost: Error reading attribute bonusAccel");
	bonusAccel_ = doubleElem;

	return true;
}

#if MANUAL_PHYSICS_DETECTION
////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::collisionDetection( const float& temps )
///
/// /*Description*/
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::collisionDetection( const float& temps )
{
    auto field = getField();
    if(field)
    {
        NoeudRondelle* rondelle = field->getPuck();
        if(rondelle)
        {
            Vecteur3 distance = getPosition()- rondelle->getPosition();
            float rayon = getRadius()+rondelle->getRadius();
            if(distance.norme2() > rayon*rayon+25)
            {
                ActivateBoost(true);
            }
        }
    }
}
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::ActivateBoost( bool val )
///
/// /*Description*/
///
/// @param[in] bool val
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::ActivateBoost( bool val )
{
	activer_ = val;
	setHighlight(!val);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        b2BodyDef myBodyDef;
        myBodyDef.type = IsInGame() ? b2_staticBody : b2_dynamicBody; //this will be a dynamic body
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
        RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_BOOST,IsInGame());

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::synchroniseTransformFromB2CallBack;
    }
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::renderOpenGLES()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::renderOpenGLES() const
{
    glColor4f(0.0f,1.0f,0.0f,1.0f);
    Super::renderOpenGLES();
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



