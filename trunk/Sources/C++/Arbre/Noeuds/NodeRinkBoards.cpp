///////////////////////////////////////////////////////////////////////////////
/// @file NodeRinkBoards.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NodeRinkBoards.h"
#include "NoeudPoint.h"
#include "NoeudBut.h"
#include "Utilitaire.h"

#if BOX2D_INTEGRATED
#include <Box2D/Box2D.h>
#endif

#if MIKE_DEBUG_
PRAGMA_DISABLE_OPTIMIZATION
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeRinkBoards::NodeRinkBoards( NoeudPoint* n1, NoeudPoint* n2 )
///
/// Constructeur par paramètre.
///
/// @param[in] NoeudPoint * n1 : premier point associé
/// @param[in] NoeudPoint * n2 : deuxieme point associé
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeRinkBoards::NodeRinkBoards( NoeudPoint* n1, NoeudPoint* n2 ):
    Super(RazerGameUtilities::NAME_RINK_BOARD)
{
    setScale(Vecteur3(1,0,0));
    setRecordable(false);
    setVisible(false);

    init(n1->mPosition,n2->mPosition,n1,n2);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeRinkBoards::NodeRinkBoards( NoeudPoint* n, NoeudBut* but, bool haut )
///
/// constructeur par paramètre
///
/// @param[in] NoeudPoint * n : point relié
/// @param[in] NoeudBut * but : But relié
/// @param[in] bool haut : indique si on ce lie à la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NodeRinkBoards::NodeRinkBoards( NoeudPoint* n, NoeudBut* but, bool haut ):
    Super(RazerGameUtilities::NAME_RINK_BOARD)
{
    setScale(Vecteur3(1,0,0));
    setRecordable(false);
    setVisible(false);

    init(n->mPosition,
        haut? but->mTopPosition : but->mBottomPosition,
        n,
        but);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeRinkBoards::NodeRinkBoards( NoeudBut* but, NoeudPoint* n, bool haut )
///
/// constructeur par paramètre
///
/// @param[in] NoeudPoint * n : point relié
/// @param[in] NoeudBut * but : But relié
/// @param[in] bool haut : indique si on ce lie à la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NodeRinkBoards::NodeRinkBoards( NoeudBut* but, NoeudPoint* n, bool haut ):
    Super(RazerGameUtilities::NAME_RINK_BOARD)
{
    setScale(Vecteur3(1,0,0));
    setRecordable(false);
    setVisible(false);

    init(haut? but->mTopPosition : but->mBottomPosition,
        n->mPosition,
        n,
        but);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeRinkBoards::~NodeRinkBoards()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeRinkBoards::~NodeRinkBoards()
{

}


void NodeRinkBoards::updatePhysicBody()
{
#if BOX2D_INTEGRATED
    auto world = getWorld();
    if(world)
    {
        clearPhysicsBody();

        float halfLength = mScale[VX]*DEFAULT_SIZE[VX]/2.f*utilitaire::ratioWorldToBox2D;
        float halfHeight = 0;

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a dynamic body
        myBodyDef.angle = 0; //set the starting angle

        const Vecteur3& pos = getPosition();
        b2Vec2 posB2;
        utilitaire::VEC3_TO_B2VEC(pos,posB2);
        myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position

        mPhysicBody = world->CreateBody(&myBodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(halfLength,halfHeight,b2Vec2(0,0),utilitaire::DEG_TO_RAD(mAngle));

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &shape; //this is a pointer to the shape above
        myFixtureDef.density = 1;
        if(IsInGame())
        {
            myFixtureDef.filter.categoryBits = CATEGORY_BOUNDARY;
            myFixtureDef.filter.maskBits = CATEGORY_PUCK | CATEGORY_MALLET;
        }
        
        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        //     mPhysicBody->SetUserData(this);
        //     mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
    }
#endif

}



#if MIKE_DEBUG_
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////