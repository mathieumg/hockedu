///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierFan.cpp
/// @author Michael Ferris
/// @date 2013-03-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusModifierFan.h"

// ne fait pas de sens d'utiliser un modifier si on ne joue pas
#if BOX2D_PLAY  
#include <Box2D/Box2D.h>
#endif
#include "NoeudAbstrait.h"
#include "Utilitaire.h"
#include "NodeBonus.h"
#include "ForceField.h"
#include "SoundFMOD.h"

const float FanRadius = 8;

#if WIN32
#include "Modele3D.h"
#include "GestionnaireModeles.h"
CreateListDelegateImplementation(Fan)
{
    return RazerGameUtilities::CreateListSphereDefault(pModel,FanRadius);
}

#endif


////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierFan::BonusModifierFan()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierFan::BonusModifierFan():
    BonusModifierAbstract(10),mForceField(NULL),mAngle(0)
{
}



////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierFan::~BonusModifierFan()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierFan::~BonusModifierFan()
{
#if BOX2D_PLAY  
    if(mForceField)
    {
        delete mForceField;
    }
    mForceField = NULL;
#endif
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierFan::Attach( NoeudAbstrait* pPuck )
///
/// Attach a modifier on a node, receiving the puck as entry point,
/// but from it, the modifier can apply itself on anything in the map
///
/// returns true if the modifier is attached on a node
/// returns false otherwise
/// note, returning false doesn't mean the bonus was not applied or refreshed
///
/// @param[in] NoeudAbstrait * pPuck
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierFan::Attach( NoeudRondelle* pPuck )
{
    if(mCreator)
    {
        
#if WIN32 
        SoundFMOD::obtenirInstance()->playEffect(BONUS_FAN_IN_EFFECT); 
#endif

        mOwner = mCreator;
        return mCreator->AddModifier(this);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierFan::Apply()
///
/// Applies the real effect on the node
/// returns false if the bonus finished the execution (no time to live)
/// in case it returns false, complete its execution
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierFan::Apply()
{
    if(mOwner)
    {
#if BOX2D_PLAY 
        auto world = mOwner->getWorld();
        if(world)
        {
            Vecteur3 pos = mOwner->getPosition();
            b2Vec2 posB2;
            utilitaire::VEC3_TO_B2VEC(pos,posB2);
            float radius = FanRadius*utilitaire::ratioWorldToBox2D;

            mForceField = new ForceFieldCircle(radius*6.0f,-0.01f);
            mForceField->CreateBody(world,CATEGORY_PUCK,posB2);

            return true;
        }
#endif
    }

    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierFan::Revert()
///
/// Reverts the effect on the node,
/// return unsed for now
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierFan::Revert()
{
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierFan::render()
///
/// Render specific content for this modifier
/// translation to the node will already be applied
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierFan::render() const
{
#if WIN32

    GLuint listeFan=0,listeFanCase=0;
    GestionnaireModeles::obtenirInstance()->obtenirListe(RAZER_KEY_FAN,listeFan);
    checkf(mOwner);
    if(listeFan)
    {
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        auto pos = mOwner->getPosition();
        glTranslatef(pos[VX],pos[VY],0);
        glRotatef(mAngle,0,0,1);
        glCallList(listeFan);
        glPopAttrib();
        glPopMatrix();
    }
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierFan::Tick( float temps )
///
/// Tick the modifier
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierFan::Tick( float temps )
{
    mAngle += temps*2000.0f;
    BonusModifierAbstract::Tick(temps);
}


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////