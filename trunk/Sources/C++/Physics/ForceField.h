///////////////////////////////////////////////////////////////////////////////
/// @file ForceField.h
/// @author Michael Ferris
/// @date 2013-03-28
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <set>

/// Force fields are used with Box2D
#if BOX2D_PLAY

class b2Body;
class b2World;
struct b2Vec2;

/// !!!! Every coords are in Box2D's system, must convert values before applying it to these objects

///////////////////////////////////////////////////////////////////////////
/// @class ForceField
/// @brief Object containing the info to create and apply a force field in the game
///
/// @author Michael Ferris
/// @date 2013-03-28
///////////////////////////////////////////////////////////////////////////
class ForceField
{
public:
    ForceField():
        mPhysicBody(NULL),mActive(true)
    {
        
    }
    virtual ~ForceField();
    /// Creates the physics body for this field
    /// categoryAffected : physicis categories of objects affected by the force field
    virtual void CreateBody(b2World* world, unsigned short categoryAffected, const b2Vec2& pos) = 0;

    void AddAffectedBody(b2Body* pBody)
    {
        if(pBody)
        {
            mAffectedBodies.insert(pBody);
        }
    }
    void RemoveAffectedBody(b2Body* pBody)
    {
        mAffectedBodies.erase(pBody);
    }

    /// Applies forces on affected bodies
    virtual void ApplyForceField() const = 0;
protected:
    b2Body* mPhysicBody;
    /// affected bodies will have a force applied on them every tick of the world
    std::set<b2Body*> mAffectedBodies;
    bool mActive;


public:
    /// Accessors of mPhysicBody
    inline b2Body* getPhysicsBody() const { return mPhysicBody; }
    inline void setPhysicsBody(b2Body* pVal) { mPhysicBody = pVal; }

    /// Accessors of mAffectedBodies
    inline const std::set<b2Body*>& getAffectedBodies() const { return mAffectedBodies; }
    inline void setAffectedBodies(const std::set<b2Body*>& pVal) { mAffectedBodies = pVal; }

    /// Accessors of mActive
    inline bool IsActive() const { return mActive; }
    inline void setActive( bool pVal) { mActive = pVal; }

};


///////////////////////////////////////////////////////////////////////////
/// @class ForceFieldCircle
/// @brief Force field represented by a circle where the force is applied from the center 
///         of the circle.
///
/// @author Michael Ferris
/// @date 2013-03-28
///////////////////////////////////////////////////////////////////////////
class ForceFieldCircle : public ForceField
{
public:
    ForceFieldCircle(float radius,float maxForce):
        mRadius(radius),mMaxForce(maxForce)
    {

    }
    /// Creates the physics body for this field
    /// categoryAffected : physicis categories of objects affected by the force field
    virtual void CreateBody(b2World* world, unsigned short categoryAffected, const b2Vec2& pos);

    /// Applies forces on affected bodies
    virtual void ApplyForceField() const;
private:
    const float mRadius;
    const float mMaxForce;
};




#endif





