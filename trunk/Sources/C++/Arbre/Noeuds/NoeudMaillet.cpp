///////////////////////////////////////////////////////////////////////////////
/// @file NoeudMaillet.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudMaillet.h"
#include "VisiteurCollision.h"
#include "NoeudMuret.h"
#include "NoeudTable.h"
#include "NoeudRondelle.h"
#include "NoeudGroupe.h"
#include "NoeudBut.h"
#include "Terrain.h"

#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#if PLAY_GAME
#include "PlayerComputer.h"
#endif

#if BOX2D_DEBUG
#include "DebugRenderBox2D.h"
#endif

#ifndef __APPLE__
#include "FacadeModele.h"
#endif

#include "Utilitaire.h"
#include "EditionEventManager.h"

#include "RazerGameUtilities.h"

const float NoeudMaillet::DEFAULT_RADIUS = 10;

CreateListDelegateImplementation(Mallet)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudMaillet::DEFAULT_RADIUS);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet::NoeudMaillet(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet::NoeudMaillet(const std::string& typeNoeud, unsigned int& malletCreated, unsigned int malletLimit)
   : NoeudAbstrait(RAZER_KEY_MALLET,typeNoeud), joueur_(0),mNbMalletCreated(malletCreated)
#if BOX2D_PLAY
, mMouseJoint(NULL),mMouseBody(NULL)
#endif
{
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);




    ++mNbMalletCreated;
    if(++EditionEventManager::mGlobalMallet >= EditionEventManager::mEditionLimitMallet)
    {
        EditionEventManager::TransmitEvent(DISABLE_MALLET_CREATION);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet::~NoeudMaillet()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NoeudMaillet::~NoeudMaillet()
{
	--mNbMalletCreated;
    // indique aux runnables qui lui sont associé de s'invalidé
    RunnableBreaker::signalObservers();
    if(--EditionEventManager::mGlobalMallet < EditionEventManager::mEditionLimitMallet)
    {
        EditionEventManager::TransmitEvent(ENABLE_MALLET_CREATION);
    }
#if BOX2D_PLAY
    //checkf(!mMouseJoint, "Le mouse joint a mal ete liberé");
    destroyMouseJoint();
#endif


}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::renderReal(  )
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::renderReal() const
{
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Appel à la version de la classe de base pour l'affichage des enfants.
    NoeudAbstrait::renderReal();

    // Restauration de la matrice.
    glPopAttrib();
    glPopMatrix();

#if BOX2D_DEBUG
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    if (mMouseJoint)
    {
        DebugRenderBox2D* debugRender = DebugRenderBox2D::mInstance;
        if(debugRender)
        {
            b2Vec2 p1 = mMouseJoint->GetAnchorB();
            b2Vec2 p2 = mMouseJoint->GetTarget();

            b2Color c;
            c.Set(0.0f, 1.0f, 0.0f);
            debugRender->DrawPoint(p1, 4.0f, c);
            debugRender->DrawPoint(p2, 4.0f, c);

            c.Set(0.8f, 0.8f, 0.8f);
            debugRender->DrawSegment(p1, p2, c);
        }
    }
    // Restauration de la matrice.
    glPopAttrib();
    glPopMatrix();
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::renderOpenGLES()
///
/// Fonction appeler dans afficher concret pour faire le
/// rendu OpenGL, uniquement utilisé sous APPLE.
/// utiliser les liste d'affichage pour windows
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::renderOpenGLES() const
{
    glColor4f(0.0f,0.0f,0.0f,1.0f);
    Super::renderOpenGLES();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::tick(float temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::tick( const float& temps)
{

   // Appel à la version de la classe de base pour l'animation des enfants.
   NoeudAbstrait::tick(temps);
   

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudMaillet::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::acceptVisitor( VisiteurNoeud& v )
{
	v.visiterNoeudMaillet(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::setTargetDestination( Vecteur3 pos )
///
/// Assignation de la position de la souris pour que le maillet puisse la suivre
///
/// @param[in] Vecteur3 pos
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::setTargetDestination( const Vecteur3& pos )
{
#if BOX2D_PLAY
    if(mMouseJoint)
    {
		Vecteur3 resPos = pos;
		Vecteur3 delta = resPos - getPosition();
		static float maxLength = 15;
		if(delta.norme2() >= maxLength*maxLength)
		{
			delta.normaliser();
			delta *= maxLength;
			resPos = getPosition() + delta;
		}

        b2Vec2 targetPointB2;
        utilitaire::VEC3_TO_B2VEC(resPos,targetPointB2);
        mMouseJoint->SetTarget(targetPointB2);
    }
#endif
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
        const Vecteur3& pos = getPosition();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
        myBodyDef.angle = 0; //set the starting angle
        myBodyDef.fixedRotation = true;


        mPhysicBody = world->CreateBody(&myBodyDef);
        b2CircleShape circleShape;
        circleShape.m_p.Set(0, 0); //position, relative to body position
        circleShape.m_radius = (float32)getRadius()*utilitaire::ratioWorldToBox2D; //radius

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
        myFixtureDef.density = 0.02f;
        myFixtureDef.restitution = 0.2f;
        RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_MALLET,IsInGame());

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::synchroniseTransformFromB2CallBack;
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::buildMouseJoint()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::buildMouseJoint(bool pIsNetworkControlled /*=false*/)
{
#if BOX2D_PLAY
    if(!mMouseJoint)
    {
        auto world = getWorld();
        if(world)
        {
            if(!mMouseBody)
            {
                b2BodyDef bodyDef;
                mMouseBody = world->CreateBody(&bodyDef);
            }

            b2Body* body = getPhysicBody();
            b2MouseJointDef md;
            md.bodyA = mMouseBody;
            md.bodyB = body;
            const Vecteur3& pos = getPosition();
            utilitaire::VEC3_TO_B2VEC(pos,md.target);
            if(joueur_ && joueur_->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
            {
                md.maxForce = 1200.0f * body->GetMass();
            }
            else
            {
                md.maxForce = 3000.0f * body->GetMass();
            }
            md.dampingRatio = 0;
            md.frequencyHz = 100;
            mMouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::destroyMouseJoint()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::destroyMouseJoint()
{
#if BOX2D_PLAY
    if(mMouseJoint)
    {
        auto world = getWorld();
        if(world)
        {
            world->DestroyJoint(mMouseJoint);
            world->DestroyBody(mMouseBody);
        }
        mMouseJoint = NULL;
        mMouseBody = NULL;
    }
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::playTick()
///
/// node tick received when actually playing the game (simulation running)
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::playTick(float temps)
{
    Super::playTick(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudMaillet::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
///
/// /*Description*/
///
/// @param[in] const ObjectAnimationParameters & pAnimationResult
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudMaillet::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
{

    if(pAnimationResult.CanUpdatedPosition())
    {
#ifndef __APPLE__
        if(getPlayer())
        {
            if(getPlayer()->obtenirType() == JOUEUR_VIRTUEL_RENFORCEMENT)
            {
                Vecteur3 wPos = pAnimationResult.mPosition;
                this->setTargetDestination(wPos);
            }
            else
            {
#endif
                setPosition(pAnimationResult.mPosition);
#ifndef __APPLE__
            }
        }
#endif
    }
    if(pAnimationResult.CanUpdatedAngle())
        mAngle = pAnimationResult.mAngle[VZ];
    if(pAnimationResult.CanUpdatedScale())
    {
        mScale = pAnimationResult.mScale;
        updateRadius();
    }
    updateMatrice();
}

//#if MAT_DEBUG_
//std::vector<Vecteur3> NoeudMaillet::mListePointsDebug;
//#endif


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
