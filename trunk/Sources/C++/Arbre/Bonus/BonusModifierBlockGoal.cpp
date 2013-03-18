///////////////////////////////////////////////////////////////////////////////
/// @file BonusModifierBlockGoal.cpp
/// @author Michael Ferris
/// @date 2013-03-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "BonusModifierBlockGoal.h"
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#include "NoeudAbstrait.h"
#include "NoeudRondelle.h"
#include "Terrain.h"
#include "NoeudBut.h"
#include "Utilitaire.h"
#include "NoeudMaillet.h"


Vecteur3 GoalerSize(3,15,15);

#if WIN32
#include "Modele3D.h"
#include "GestionnaireModeles.h"
CreateListDelegateImplementation(Goaler)
{
    Vecteur3 coinMin,coinMax;
    pModel->calculerBoiteEnglobante(coinMin,coinMax);
    Vecteur3 delta = coinMax - coinMin;
    delta = GoalerSize / delta;

    pModel->assignerFacteurAgrandissement(delta);
    return GestionnaireModeles::CreerListe(pModel);
}
#endif


////////////////////////////////////////////////////////////////////////
///
/// @fn  BonusModifierBlockGoal::BonusModifierBlockGoal()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
BonusModifierBlockGoal::BonusModifierBlockGoal():
    BonusModifierAbstract(10), mPhysicBody(NULL)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierBlockGoal::Attach( NoeudAbstrait* pPuck )
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
bool BonusModifierBlockGoal::Attach( NoeudRondelle* pPuck )
{
    if(pPuck)
    {
        auto mallet = pPuck->getLastHittingMallet();
        auto field = pPuck->getField();
        if(field)
        {
            NoeudBut* goals[2];
            field->getGoals(goals);
            /// on choisit le but de gauche(index 0) si le dernier maillet a frapper est le gauche
            NoeudBut* affectedGoal = goals[!(mallet == field->getLeftMallet())];
            if(affectedGoal)
            {
                mPuckPos = &pPuck->getPosition();
                mProtectedGoal = affectedGoal;
                mOwner = mallet;
                return mOwner->AddModifier(this);
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierBlockGoal::Apply()
///
/// Applies the real effect on the node
/// returns false if the bonus finished the execution (no time to live)
/// in case it returns false, complete its execution
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierBlockGoal::Apply()
{
    if(mOwner)
    {
        auto existingModifiers = mOwner->GetModifiers();
        // recherche pour un bonus déjà présent
        for(auto it = existingModifiers.begin(); it != existingModifiers.end(); ++it)
        {
            BonusModifierBlockGoal* modifier = dynamic_cast<BonusModifierBlockGoal*>(*it);
            if(modifier && modifier != this)
            {
                // resets time to live on the modifier
                modifier->mTimeToLive = mTimeToLive;
                /// return false to indicate that the bonus was not added to the node, even though the 
                /// bonus itself was applied.
                return false;
            }
        }
#if BOX2D_INTEGRATED 
        auto world = mOwner->getWorld();
        if(world)
        {
            float halfLength = (GoalerSize[VX]/2.f)*utilitaire::ratioWorldToBox2D;
            float halfHeight = (GoalerSize[VY]/2.f)*utilitaire::ratioWorldToBox2D;

            b2BodyDef myBodyDef;
            myBodyDef.type = b2_staticBody; //this will be a dynamic body
            myBodyDef.position.Set(0, 0); //set the starting position
            myBodyDef.angle = 0; //set the starting angle

            mPhysicBody = world->CreateBody(&myBodyDef);
            b2PolygonShape shape;
            Vecteur3 pos;
            shape.SetAsBox(halfLength,halfHeight,b2Vec2(0,0),0);

            b2FixtureDef myFixtureDef;
            myFixtureDef.shape = &shape; //this is a pointer to the shape above
            myFixtureDef.density = 1;
            // Il s'agit ici d'un goaler qui peut entré en collision avec une rondelle
            myFixtureDef.filter.categoryBits = CATEGORY_GOALIE;
            myFixtureDef.filter.maskBits = CATEGORY_PUCK;
            
            mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
            mPhysicBody->SetUserData(this);

            float angle;
            getGoalPosition(pos,angle);
            b2Vec2 posB2;
            utilitaire::VEC3_TO_B2VEC(pos,posB2);
            mPhysicBody->SetTransform(posB2,angle); //set the starting position

            return true;
        }

#endif
    }

    // if any fixtures were found, it means that this node had a modifiation applied
    return mFixtures.size() != 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool BonusModifierBlockGoal::Revert()
///
/// Reverts the effect on the node,
/// return unsed for now
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool BonusModifierBlockGoal::Revert()
{
#if BOX2D_INTEGRATED  
    if(mPhysicBody)
    {
        mPhysicBody->GetWorld()->DestroyBody(mPhysicBody);
    }

#endif
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierBlockGoal::render()
///
/// Render specific content for this modifier
/// translation to the node will already be applied
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierBlockGoal::render() const
{
#if WIN32

    GLuint liste;
    GestionnaireModeles::obtenirInstance()->obtenirListe(RazerGameUtilities::NAME_GOALER,liste);
    if(liste)
    {
        glPushMatrix();
        glDisable(GL_LIGHTING);
#if BOX2D_INTEGRATED  
        auto transform = mPhysicBody->GetTransform();
        Vecteur3 pos;
        utilitaire::B2VEC_TO_VEC3(pos,transform.p);
        glTranslatef(pos[VX],pos[VY],0);
        glRotatef(utilitaire::RAD_TO_DEG(transform.q.GetAngle()),0,0,1);
#endif //BOX2D_INTEGRATED
        glCallList(liste);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

#endif
 

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void BonusModifierBlockGoal::Tick( float temps )
///
/// Tick the modifier
///
/// @param[in] float temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void BonusModifierBlockGoal::Tick( float temps )
{

    Vecteur3 pos;
    float angle;
    getGoalPosition(pos,angle);
    b2Vec2 posB2;
    utilitaire::VEC3_TO_B2VEC(pos,posB2);
    mPhysicBody->SetTransform(posB2,angle);
    BonusModifierAbstract::Tick(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur3 BonusModifierBlockGoal::getGoalPosition()
///
/// /*Description*/
///
///
/// @return Vecteur3
///
////////////////////////////////////////////////////////////////////////
void BonusModifierBlockGoal::getGoalPosition( Vecteur3& pos, float& angle ) const
{
    if(mProtectedGoal)
    {
        pos = mProtectedGoal->getPosition();
        
        float yPos = 0;
        float xPos = 0;
        // cache value for optim
        const float puckHeight = (*mPuckPos)[VY];

        if(puckHeight > 0)
        {
            /// puck en haut
            
            auto posHaut = mProtectedGoal->obtenirPositionHaut();
            yPos = posHaut[VY];
            xPos = posHaut[VX];
            if(puckHeight < yPos)
            {
                /// la puck est entre le haut du but et le centre
                yPos = puckHeight;

                posHaut -= pos;
                /// le but ne peut avoir son point du haut a la meme place que son centre
                /// donc posHaut[VY] != 0
                float ratio = posHaut[VX]/posHaut[VY];
                xPos = ratio*yPos+pos[VX];
            }
        }
        else
        {
            /// puck en bas

            auto posBas = mProtectedGoal->obtenirPositionBas();
            yPos = posBas[VY];
            xPos = posBas[VX];
            if(puckHeight > yPos)
            {
                /// la puck est entre le haut du but et le centre
                yPos = puckHeight;

                posBas -= pos;
                /// le but ne peut avoir son point du haut a la meme place que son centre
                /// donc posBas[VY] != 0
                float ratio = posBas[VX]/posBas[VY];
                xPos = ratio*yPos+pos[VX];
            }
        }

        // deplacer le goaler devant le but
        if(mProtectedGoal->obtenirJoueur() == 1)xPos += 10;
        else xPos -= 10;

        pos[VX] = xPos;
        pos[VY] = yPos;

        auto delta = (*mPuckPos)-pos;
        angle = atan(delta[VY]/delta[VX]);
    }
}
