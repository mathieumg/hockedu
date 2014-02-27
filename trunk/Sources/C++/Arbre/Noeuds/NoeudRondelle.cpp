///////////////////////////////////////////////////////////////////////////////
/// @file NoeudRondelle.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __APPLE__
#include "FacadeModele.h"
#endif

#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif

#if MANUAL_PHYSICS_DETECTION
#include "SoundFMOD.h"
#endif

#include "NoeudRondelle.h"
#include "VisiteurCollision.h"
#include "NoeudMuret.h"
#include "AideCollision.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include <iostream>
#include "NoeudGroupe.h"
#include "Terrain.h"
#include "NoeudPortail.h"
#include "Utilitaire.h"
#include "NoeudBut.h"
#include "UsineNoeud.h"
#include "ExceptionJeu.h"
#include "EditionEventManager.h"


const float NoeudRondelle::DEFAULT_RADIUS = 8;

CreateListDelegateImplementation(Puck)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudRondelle::DEFAULT_RADIUS);
}
CreateListDelegateImplementation(PuckTroll)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,NoeudRondelle::DEFAULT_RADIUS*1.35f);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle::NoeudRondelle(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle::NoeudRondelle( unsigned int& puckCreated, unsigned int puckLimit)
    : NoeudAbstrait(RAZER_KEY_PUCK),mNbPuckCreated(puckCreated),mLastHittingMallet(NULL)
{
    // Assigner le rayon par défaut le plus tot possible car la suite peut en avoir besoin
    setDefaultRadius(DEFAULT_RADIUS);

#if MANUAL_PHYSICS_DETECTION
    puissanceVent_ = 0.05f;
    mCoefFriction = 2.5f;
    mVelocite = Vecteur3(0.0f,0.0f,0.0f);
    mAngle = 0.0f;
    mVitesseRotation = 0.0f;
#endif
    
    ++mNbPuckCreated;
    if(++EditionEventManager::mGlobalPucks >= EditionEventManager::mEditionLimitPucks)
    {
        EditionEventManager::TransmitEvent(DISABLE_PUCK_CREATION);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle::~NoeudRondelle()
///
/// Ce destructeur désallouee la liste d'affichage de la rondelle et diminuer le nombre de rondelle.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudRondelle::~NoeudRondelle()
{
    --mNbPuckCreated;
    RunnableBreaker::signalObservers();

    if(--EditionEventManager::mGlobalPucks < EditionEventManager::mEditionLimitPucks)
    {
        EditionEventManager::TransmitEvent(ENABLE_PUCK_CREATION);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::renderReal() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::renderReal() const
{
    // Appel à la version de la classe de base pour l'affichage des enfants.
    NoeudAbstrait::renderReal();
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
void NoeudRondelle::renderOpenGLES() const
{
    glColor4f(1.0f,0.0f,0.0f,1.0f);
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
void NoeudRondelle::tick( const float& temps)
{
    //A mettre dans positionUpdate();
    //mPosition += mVelocite*temps;
    // Appel à la version de la classe de base pour l'animation des enfants.


    NoeudAbstrait::tick(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudRondelle::acceptVisitor( VisiteurNoeud& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeud & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::acceptVisitor( VisiteurNoeud& v )
{
    v.visiterNoeudRondelle(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudRondelle::updatePhysicBody()
///
/// Recreates the physics body according to current attributes
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudRondelle::updatePhysicBody()
{
#if BOX2D_INTEGRATED

    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody; //this will be a dynamic body

        float puckRadius = getRadius();

        const Vecteur3& pos = getPosition();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        float friction = 0.5f;
        auto field = getField();
        if(field)
        {
            auto table = getField()->getTable();
           if(table)
           {
               friction = table->obtenirCoefFriction();
           }
        }

        myBodyDef.linearDamping = friction;
        myBodyDef.angularDamping = 0.3f;
        mPhysicBody = world->CreateBody(&myBodyDef);
        b2CircleShape circleShape;
        circleShape.m_p.Set(0, 0); //position, relative to body position
        circleShape.m_radius = puckRadius*utilitaire::ratioWorldToBox2D; //radius

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
        myFixtureDef.density = 0.01f;
        myFixtureDef.friction = 0.8f;
        myFixtureDef.restitution = 0;
        RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_PUCK,IsInGame());

        /// calcul plus precis, mais plus couteux de la physique sur la rondelle
        mPhysicBody->SetBullet(true);
        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::synchroniseTransformFromB2CallBack;

        /// Update Goals physics body accordingly with puck radius
        Terrain* terrain = getField();
        if(terrain)
        {
            NoeudBut* goals[2];
            terrain->getGoals(goals);

            for(int i=0; i<2; ++i)
            {
                NoeudBut* goal = goals[i];
                if(goal)
                {
                    goal->updatePuckCatcher(puckRadius);
                }
            }
        }
    }

#endif

}

void NoeudRondelle::modifierVelocite( const Vecteur3& val )
{
#if BOX2D_PLAY
    auto body = getPhysicBody();
    if(body)
    {
        b2Vec2 velocity;
        utilitaire::VEC3_TO_B2VEC(val,velocity);
        body->SetLinearVelocity(velocity);
    }
#elif MANUAL_PHYSICS_DETECTION
    mVelocite = val;
#endif
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 NoeudRondelle::obtenirVelocite()
///
/// /*Description*/
///
///
/// @return Vecteur3
///
////////////////////////////////////////////////////////////////////////
Vecteur3 NoeudRondelle::obtenirVelocite() const
{
#if BOX2D_PLAY
    Vecteur3 v;
    auto body = getPhysicBody();
    if(body)
    {
        utilitaire::B2VEC_TO_VEC3(v,body->GetLinearVelocity());
    }
    return v;
#elif MANUAL_PHYSICS_DETECTION
    return mVelocite;
#else
    checkf(0, "Appel bizarre sans physique");
    return 0;
#endif
}

void NoeudRondelle::modifierVitesseRotation( const float vitesse )
{
#if BOX2D_PLAY
    auto body = getPhysicBody();
    if(body)
    {
        body->SetAngularVelocity(vitesse);
    }
#elif MANUAL_PHYSICS_DETECTION
    mVitesseRotation = vitesse;
#endif
}

float NoeudRondelle::obtenirVitesseRotation() const
{
#if BOX2D_PLAY
    auto body = getPhysicBody();
    float speed = 0;
    if(body)
    {
        speed = body->GetAngularVelocity();
    }
    return speed;
#elif MANUAL_PHYSICS_DETECTION
    return mVelocite;
#else
    checkf(0, "Appel bizarre sans physique");
    return 0;
#endif
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
