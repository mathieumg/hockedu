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
#include "GestionnaireAnimations.h"
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
	: NoeudAbstrait(typeNoeud),bonusAccel_(1.50), activer_(true)
{
    // Assigner le rayon par d�faut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);
#if WIN32
	AnimationFrame* frame[5];
	frame[0] = new AnimationFrame(0, Vecteur3(50, 0, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[1] = new AnimationFrame(200, Vecteur3(0, 50, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[2] = new AnimationFrame(400, Vecteur3(-50, 0, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[3] = new AnimationFrame(600, Vecteur3(0, -50, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));
	frame[4] = new AnimationFrame(800, Vecteur3(50, 0, 0), Vecteur3(0, 0, 0), Vecteur3(1, 1, 1));

	Animation* animation = new Animation(BEZIER, true, true, true, true);
	for(int i=0; i<5; i++)
		animation->ajouterFrame(frame[i]);


	animation->ajouterObjet(this);
#endif
    updatePhysicBody();

	//GestionnaireAnimations::getInstance()->ajouterAnimation(animation);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NoeudAccelerateur::~NoeudAccelerateur(  )
///
/// Ce destructeur d�sallouee la liste d'affichage de l'acc�l�rateur.
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
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::renderReal() const
{
	// Appel � la version de la classe de base pour l'affichage des enfants.
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
/// Permet d'acceuillir les diff�rents visiteur pour les transformations.
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
/// Initialisation du NoeudAccelerateur � partir d'un element XML
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
        circleShape.m_radius = (float32)getRadius()*mScale[VX]*utilitaire::ratioWorldToBox2D; //radius

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
        myFixtureDef.density = 1;

        // Il s'agit ici d'un boost qui peut entr� en collision avec une rondell
        if(IsInGame())
        {
            myFixtureDef.filter.categoryBits = CATEGORY_BOOST;
            myFixtureDef.filter.maskBits = CATEGORY_PUCK;

            // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
            myFixtureDef.isSensor = true;
        }

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::synchroniseTransformFromB2CallBack;
    }
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAccelerateur::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAccelerateur::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
{
    if(pAnimationResult.CanUpdatedPosition())
        setPosition(pAnimationResult.mPosition);
    if(pAnimationResult.CanUpdatedAngle())
        mAngle = pAnimationResult.mAngle[VZ];
    if(pAnimationResult.CanUpdatedScale())
    {
        mScale = pAnimationResult.mScale;
        updateRadius();
    }
    updateMatrice();
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



