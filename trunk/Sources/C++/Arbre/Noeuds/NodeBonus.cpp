///////////////////////////////////////////////////////////////////////////
/// @file NodeBonus.cpp
/// @author Michael Ferris
/// @date 2012-04-12
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#include "NodeBonus.h"
#include "Terrain.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "Utilitaire.h"
#include "VisiteurNoeud.h"
#include "BonusStratAbstract.h"

const Vecteur3 DEFAULT_SIZE = Vecteur3(15, 15, 3);

#if WIN32
#include "Modele3D.h"
#include "GestionnaireModeles.h"
CreateListDelegateImplementation(EmptyBonus)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = DEFAULT_SIZE / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}
#endif
#include "BonusStratGoThroughWall.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn NodeBonus::NodeBonus(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeBonus::NodeBonus(const std::string& typeNoeud)
   : Super(typeNoeud),mMinTimeSpawn(10.5f), mMaxTimeSpawn(30.f),mStrat(NULL)
{
    // temp workaround, l'édition va le considérer comme un cercle pour un moment
    setDefaultRadius(DEFAULT_SIZE[VX]);


    forceFullUpdate();
    ResetTimeLeft();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NodeBonus::~NodeBonus()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NodeBonus::~NodeBonus()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::renderReal(  )
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::renderReal() const
{
	// Appel à la version de la classe de base pour l'affichage des enfants.
	Super::renderReal();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::playTick( float temps )
///
/// node tick received when actually playing the game (simulation running)
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::playTick( float temps)
{
    Super::playTick(temps);
    if(mStrat)
    {
        mStrat->Tick(temps);
    }
    else
    {
        auto terrain = getField();
        if(terrain && terrain->IsGameField())
        {
            // game tick
            mSpawnTimeLeft -= temps;
            if(mSpawnTimeLeft < 0)
            {
                ResetTimeLeft();
                mStrat = new BonusStratGoThroughWall();
                setVisible(true);
                // activate collision on strat creation
                activate(true);
            }

        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::ExecuteBonus( class NoeudRondelle* )
///
/// /*Description*/
///
/// @param[in] class NoeudRondelle *
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::ExecuteBonus( class NoeudRondelle* rondelle )
{
    if(mStrat)
    {
        // execute the strategie
        mStrat->Execute(rondelle);

        // then remove it since it can only be used once
        delete mStrat;
        mStrat = NULL;
    }
    setVisible(false);
    activate(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const std::string& NodeBonus::get3DModelKey()
///
/// Retrieves the key to find the 
///
///
/// @return const std::string&
///
////////////////////////////////////////////////////////////////////////
const std::string& NodeBonus::get3DModelKey() const
{
    if(mStrat)
    {
        return mStrat->get3DModelKey();
    }
    return RazerGameUtilities::NAME_BONUS;
}

const std::string& BonusStratAbstract::get3DModelKey()
{
    return RazerGameUtilities::NAME_BONUS;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::forceFullUpdate()
///
/// Recreates everything needed for the game
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::forceFullUpdate()
{
    Super::forceFullUpdate();
    if(IsInGame())
    {
        setVisible(!!mStrat);
#if BOX2D_INTEGRATED
        if(mPhysicBody)
        {
            mPhysicBody->SetActive(!!mStrat);
        }
#endif
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::updatePhysicBody()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        float halfLength = mScale[VX]*DEFAULT_SIZE[VX]/2.f*utilitaire::ratioWorldToBox2D;
        float halfHeight = mScale[VY]*DEFAULT_SIZE[VY]/2.f*utilitaire::ratioWorldToBox2D;

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a dynamic body
        myBodyDef.position.Set(0, 0); //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPhysicBody = world->CreateBody(&myBodyDef);
        b2PolygonShape shape;
        const Vecteur3& pos = getPosition();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position
        shape.SetAsBox(halfLength,halfHeight,b2Vec2(posB2.x,posB2.y),utilitaire::DEG_TO_RAD(mAngle));

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &shape; //this is a pointer to the shape above
        myFixtureDef.density = 1;
        // Il s'agit ici d'un bonus qui peut entré en collision avec une rondelle
        myFixtureDef.filter.categoryBits = CATEGORY_BONUS;
        myFixtureDef.filter.maskBits = CATEGORY_PUCK;

        // Le sensor indique qu'on va recevoir la callback de collision avec la rondelle sans vraiment avoir de collision
        myFixtureDef.isSensor = true;

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        //mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
        mPhysicBody->SetActive(!IsInGame());
    }
#endif
}

void NodeBonus::renderOpenGLES() const
{
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    Super::renderOpenGLES();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeBonus::ResetTimeLeft()
///
/// creates a random value before spawning the next bonus
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeBonus::ResetTimeLeft()
{
    // precision 2 decimale
    int min = (int)(mMinTimeSpawn*100.f);
    int max = (int)(mMaxTimeSpawn*100.f);

    mSpawnTimeLeft = (rand()%(max-min)+min)/100.f;

}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
