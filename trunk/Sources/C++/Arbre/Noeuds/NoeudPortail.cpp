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
   : NoeudAbstrait(typeNoeud), mIsAttractionFieldActive(true)
{   
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);

    updatePhysicBody();

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
	mAngle = (float)((int)(mAngle+temps*1000.0f)%360);
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
        myBodyDef.type = b2_staticBody; //this will be a static body
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

        // Il s'agit ici d'un portail qui peut entré en collision avec une rondell
        myFixtureDef.filter.categoryBits = CATEGORY_PORTAL;
        myFixtureDef.filter.maskBits = CATEGORY_PUCK;

        // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
        myFixtureDef.isSensor = true;

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        //mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
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


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
