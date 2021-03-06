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

// ne fait pas de sens d'utiliser un modifier si on ne joue pas
#if BOX2D_PLAY  
#include <Box2D/Box2D.h>
#endif
#if WIN32
#include "SoundFMOD.h"
#endif
#include "NoeudAbstrait.h"
#include "NoeudRondelle.h"
#include "Terrain.h"
#include "NoeudBut.h"
#include "Utilitaire.h"
#include "NoeudMaillet.h"

const Vecteur3 GoalerSize(3,15,15);

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
#include "NoeudTable.h"


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
        if(field && mallet)
        {
            NoeudBut* goals[2];
            field->getGoals(goals);
            /// on choisit le but de gauche(index 0) si le dernier maillet a frapper est le gauche
            NoeudBut* affectedGoal = goals[!(mallet == field->getLeftMallet())];
            if(affectedGoal)
            {
#if WIN32 
                SoundFMOD::obtenirInstance()->playEffect(BONUS_BLOCK_GOAL_IN_EFFECT); 
#endif

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
        // recherche pour un bonus d�j� pr�sent
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
#if BOX2D_PLAY 
        auto world = mOwner->getWorld();
        if(world)
        {
            float halfLength = (GoalerSize[VX]/2.f)*utilitaire::ratioWorldToBox2D;
            float halfHeight = (GoalerSize[VY]/2.f)*utilitaire::ratioWorldToBox2D;

            b2BodyDef myBodyDef;
            myBodyDef.type = b2_staticBody; //this will be a dynamic body

            getGoalPosition(mPosition,mAngle);
            b2Vec2 posB2;
            utilitaire::VEC3_TO_B2VEC(mPosition,posB2);
            myBodyDef.position.Set(posB2.x,posB2.y); //set the starting position
            myBodyDef.angle = mAngle; //set the starting angle

            mPhysicBody = world->CreateBody(&myBodyDef);
            b2PolygonShape shape;
            shape.SetAsBox(halfLength,halfHeight,b2Vec2(0,0),0);

            b2FixtureDef myFixtureDef;
            myFixtureDef.shape = &shape; //this is a pointer to the shape above
            myFixtureDef.density = 1;
            // Il s'agit ici d'un goaler qui peut entr� en collision avec une rondelle
            myFixtureDef.filter.categoryBits = CATEGORY_BOUNDARY;
            myFixtureDef.filter.maskBits = CATEGORY_PUCK;
            myFixtureDef.restitution = 0.95f;

            mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
            mPhysicBody->SetUserData(this);
            return true;
        }
        // if any fixtures were found, it means that this node had a modifiation applied
        return mFixtures.size() != 0;
#endif
    }

    return false;
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
#if WIN32
    SoundFMOD::obtenirInstance()->playEffect(BONUS_BLOCK_GOAL_OUT_EFFECT); 
#endif

#if BOX2D_PLAY  
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
    GestionnaireModeles::obtenirInstance()->obtenirListe(RAZER_KEY_GOALER,liste);
    if(liste)
    {
        glPushMatrix();
        glDisable(GL_LIGHTING);
#if BOX2D_PLAY  
        glTranslatef(mPosition[VX],mPosition[VY],0);
        glRotatef(utilitaire::RAD_TO_DEG(mAngle),0,0,1);
#endif //BOX2D_PLAY
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
    Vecteur3 aimPosition;
    float aimAngle;
    getGoalPosition(aimPosition,aimAngle);
    Vecteur3 move = aimPosition-mPosition;
    float SpeedSquare = (move/temps).norme();
    static const float MaxSpeed = 150; // in pixel/s
    float ratio = MaxSpeed/SpeedSquare;
    if(ratio < 1)
    {
        move*= ratio;
        aimPosition = mPosition+move;
        aimAngle = mAngle + (aimAngle-mAngle)*ratio;
    }
    mPosition = aimPosition;
    mAngle = aimAngle;
#if BOX2D_PLAY
    if(mPhysicBody)
    {
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(mPosition,posB2);
        mPhysicBody->SetTransform(posB2,mAngle);
    }
#endif
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
        auto pucks = mProtectedGoal->getField()->getPucks();

        Vecteur3 puckPos;
        float closest = 9999999;
        STL_ITERATE((*pucks),p)
        {
            float d = ((*p)->getPosition()-pos).norme2();
            if(d<closest)
            {
                closest = d;
                puckPos = (*p)->getPosition();
            }
        }

        float yPos = 0;
        float xPos = 0;
        // cache value for optim
        const float puckHeight = puckPos[VY];

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

        auto delta = puckPos-pos;
        angle = atan(delta[VY]/delta[VX]);
    }
}
