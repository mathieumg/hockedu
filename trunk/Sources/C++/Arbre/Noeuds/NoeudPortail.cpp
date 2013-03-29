///////////////////////////////////////////////////////////////////////////////
/// @file NoeudPortail.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudPortail.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "VisiteurNoeud.h"
#include "../../Physics/ForceField.h"


const float NoeudPortail::DEFAULT_RADIUS = 10;

CreateListDelegateImplementation(Portal)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudPortail::DEFAULT_RADIUS);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::NoeudPortail(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::NoeudPortail(const std::string& typeNoeud)
   : NoeudAbstrait(typeNoeud), mIsAttractionFieldActive(true),mForceField(NULL),mAttractionForce(0.5)
{   
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::~NoeudPortail()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::~NoeudPortail()
{
#if BOX2D_PLAY
    if(mForceField)
    {
        delete mForceField;
    }
#endif //BOX2D_PLAY
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::renderReal() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::renderReal() const
{
	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudAbstrait::renderReal();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::tick( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::tick( const float& temps)
{
	setAngle((float)((int)(mAngle+temps*1000.0f)%360));
	updateMatrice();
   // Appel à la version de la classe de base pour l'animation des enfants.
   NoeudAbstrait::tick(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::acceptVisitor( VisiteurNoeudComposite& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeudComposite & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudPortail(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();


        b2BodyDef myBodyDef;
        myBodyDef.type = IsInGame() ? b2_staticBody : b2_dynamicBody;; //this will be a static body
        const Vecteur3& pos = getPosition();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPhysicBody = world->CreateBody(&myBodyDef);
        b2CircleShape circleShape;
        circleShape.m_p.Set(0, 0); //position, relative to body position

        const float radius = (float32)getRadius()*utilitaire::ratioWorldToBox2D;
        /// le corps de detection de collision est plus petit que le modele 3D pour donner un peu de temps avant que la rondelle n'entre dedans
        circleShape.m_radius = radius;
        if(IsInGame())
        {
            circleShape.m_radius *= 0.2f;
        }

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
        myFixtureDef.density = 1;

        // Il s'agit ici d'un portail qui peut entré en collision avec une rondell
        if(IsInGame())
        {
            myFixtureDef.filter.categoryBits = CATEGORY_PORTAL;
            myFixtureDef.filter.maskBits = CATEGORY_PUCK;

            // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
            myFixtureDef.isSensor = true;
        }
        else
        {
            myFixtureDef.filter.categoryBits = CATEGORY_PORTAL;
            myFixtureDef.filter.maskBits = 0xFFFF;
            myFixtureDef.filter.groupIndex = 1;
        }

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::synchroniseTransformFromB2CallBack;

#if BOX2D_PLAY 
        if(mForceField)
        {
            delete mForceField;
        }
        mForceField = new ForceFieldCircle(radius*3.0f,100*utilitaire::ratioWorldToBox2D*mAttractionForce);
        mForceField->CreateBody(world,CATEGORY_PUCK);
        auto fieldBody = mForceField->getPhysicsBody();
        fieldBody->SetTransform(mPhysicBody->GetPosition(),0);
        checkf(mForceField);
#endif //BOX2D_PLAY
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::renderOpenGLES()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::renderOpenGLES() const
{
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    Super::renderOpenGLES();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudPortail::isAttractionFieldActive()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudPortail::isAttractionFieldActive() const
{
#if BOX2D_PLAY
    return mForceField && mForceField->IsActive();
#else
    return mIsAttractionFieldActive;
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::setIsAttractionFieldActive( const bool actif )
///
/// /*Description*/
///
/// @param[in] const bool actif
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::setIsAttractionFieldActive( const bool actif )
{
#if BOX2D_PLAY
    if(mForceField)
    {
        mForceField->setActive(actif);
    }
#else
    mIsAttractionFieldActive=actif;
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::setPosition( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::setPosition( const Vecteur3& positionRelative )
{
    Super::setPosition(positionRelative);
#if BOX2D_PLAY
    if( !isWorldLocked() && mForceField && mPhysicBody)
    {
        auto body = mForceField->getPhysicsBody();
        body->SetTransform(mPhysicBody->GetPosition(),0);
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* NoeudPortail::createXmlNode()
///
/// /*Description*/
///
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* NoeudPortail::createXmlNode()
{
    auto elem = Super::createXmlNode();
    if(elem)
    {
        XMLUtils::writeAttribute(elem,"forceField",mAttractionForce);
    }
    return elem;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NoeudPortail::initFromXml( const XmlElement* element )
///
/// /*Description*/
///
/// @param[in] const XmlElement * element
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NoeudPortail::initFromXml( const XmlElement* element )
{
    if(!Super::initFromXml(element))
        return false;

    // if error here keep default value
    XMLUtils::readAttribute(element,"forceField",mAttractionForce);


    return true;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
