///////////////////////////////////////////////////////////////////////////////
/// @file ForceField.cpp
/// @author Michael Ferris
/// @date 2012-03-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "ForceField.h"

#if BOX2D_PLAY
#include <Box2D/Box2D.h>
#include "../../C#/UIHeavyClient/UIHeavyClient/Enum_Declarations.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  ForceField::~ForceField()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ForceField::~ForceField()
{
    if(mPhysicBody)
    {
        mPhysicBody->GetWorld()->DestroyBody(mPhysicBody);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ForceFieldCircle::CreateBody( b2World* world, uint16 categoryAffected )
///
/// Creates the physics body for this field
///
/// @param[in] b2World * world
/// @param[in] uint16 categoryAffected : physics categories of objects affected by the force field
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ForceFieldCircle::CreateBody( b2World* world, unsigned short categoryAffected, const b2Vec2& pos )
{
    checkf(!mPhysicBody,"Creating a new physics body for a force field already defined")
    if(world && mRadius > 0)
    {
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a static body
        myBodyDef.position = pos; //set the starting position
        myBodyDef.angle = 0; //set the starting angle

        mPhysicBody = world->CreateBody(&myBodyDef);
        b2CircleShape circleShape;
        circleShape.m_radius = mRadius;

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
        myFixtureDef.density = 1;

        // Il s'agit ici d'un champ de force qui affecte les rondelles
        myFixtureDef.filter.categoryBits = CATEGORY_FORCE_FIELD;
        myFixtureDef.filter.maskBits = categoryAffected;
        myFixtureDef.isSensor = true;

        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ForceFieldCircle::ApplyForceField()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ForceFieldCircle::ApplyForceField() const
{
    if(mPhysicBody && IsActive())
    {
        STL_ITERATE(mAffectedBodies, it)
        {
            b2Body* body = *it;
            auto forceVector = mPhysicBody->GetPosition() - body->GetPosition();
            /// utilise la longueur du vecteur et le transforme en vecteur unitaire en meme temps

            auto distance = forceVector.Normalize();
            float ratio = 1-(distance/mRadius);
            if(ratio < 0.2f)
            {
                ratio = 0.2f;
            }

            forceVector *= mMaxForce*ratio;
            body->ApplyForceToCenter(forceVector);
        }
    }
}

#endif
