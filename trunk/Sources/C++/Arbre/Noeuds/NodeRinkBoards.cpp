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
/// Constructeur par param�tre.
///
/// @param[in] NoeudPoint * n1 : premier point associ�
/// @param[in] NoeudPoint * n2 : deuxieme point associ�
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeRinkBoards::NodeRinkBoards( NoeudPoint* n1, NoeudPoint* n2 ):
    Super(RazerGameUtilities::NAME_RINK_BOARD), mPoint1(n1),mPoint2(n2)
{
    initBase();
    init(n1->mPosition,n2->mPosition,n1,n2);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeRinkBoards::NodeRinkBoards( NoeudPoint* n, NoeudBut* but, bool haut )
///
/// constructeur par param�tre
///
/// @param[in] NoeudPoint * n : point reli�
/// @param[in] NoeudBut * but : But reli�
/// @param[in] bool haut : indique si on ce lie � la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NodeRinkBoards::NodeRinkBoards( NoeudPoint* n, NoeudBut* but, bool haut ):
    Super(RazerGameUtilities::NAME_RINK_BOARD), mPoint1(n),mPoint2(NULL)
{
    initBase();

    init(n->mPosition,
        haut? but->mTopPosition : but->mBottomPosition,
        n,
        but);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeRinkBoards::NodeRinkBoards( NoeudBut* but, NoeudPoint* n, bool haut )
///
/// constructeur par param�tre
///
/// @param[in] NoeudPoint * n : point reli�
/// @param[in] NoeudBut * but : But reli�
/// @param[in] bool haut : indique si on ce lie � la partie du haut du but
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
NodeRinkBoards::NodeRinkBoards( NoeudBut* but, NoeudPoint* n, bool haut ):
    Super(RazerGameUtilities::NAME_RINK_BOARD), mPoint1(NULL),mPoint2(n)
{
    initBase();
    init(haut? but->mTopPosition : but->mBottomPosition,
        n->mPosition,
        n,
        but);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeRinkBoards::initBase()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeRinkBoards::initBase()
{
    /// les noeuds points ne peuvent etre supprimer
    mFlags.SetFlag(false,NODEFLAGS_CAN_BE_DELETED);

    setSkinKey(RAZER_KEY_WALL);
    setScale(Vecteur3(1,1,1));
    setRecordable(false);
#if WIN32
    setVisible(false);
#endif
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

//         const Vecteur3& pos = getPosition();
//         b2Vec2 posB2;
//         utilitaire::VEC3_TO_B2VEC(pos,posB2);
//         myBodyDef.position.Set(posB2.x, posB2.y); //set the starting position

        mPhysicBody = world->CreateBody(&myBodyDef);


        auto pos1 = obtenirCoin1();
        auto pos2 = obtenirCoin2();
        

        if(mPoint1)
        {
            auto type = mPoint1->obtenirTypePosNoeud();
            switch (type)
            {
            case POSITION_HAUT_GAUCHE:
            case POSITION_HAUT_DROITE:
            case POSITION_BAS_GAUCHE:
            case POSITION_BAS_DROITE:
                pos1 += (pos1-pos2)/5.f;
            default:
                break;
            }
        }
        if(mPoint2)
        {
            auto type = mPoint2->obtenirTypePosNoeud();
            switch (type)
            {
            case POSITION_HAUT_GAUCHE:
            case POSITION_HAUT_DROITE:
            case POSITION_BAS_GAUCHE:
            case POSITION_BAS_DROITE:
                pos2 += (pos2-pos1)/5.f;
            default:
                break;
            }
        }

        b2Vec2 pos1B2,pos2B2;
        utilitaire::VEC3_TO_B2VEC(pos1,pos1B2);
        utilitaire::VEC3_TO_B2VEC(pos2,pos2B2);

        b2EdgeShape shape;
        shape.Set(pos1B2,pos2B2);
        //shape.SetAsBox(halfLength,halfHeight,b2Vec2(0,0),utilitaire::DEG_TO_RAD(mAngle));

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &shape; //this is a pointer to the shape above
        myFixtureDef.density = 1;
        myFixtureDef.restitution = getReboundRatio();
        RazerGameUtilities::ApplyFilters(myFixtureDef,RAZER_KEY_RINK_BOARD,IsInGame());

        
        mPhysicBody->CreateFixture(&myFixtureDef); //add a fixture to the body
        mPhysicBody->SetUserData(this);
        //     mPhysicBody->mSynchroniseTransformWithUserData = NoeudAbstrait::SynchroniseTransformFromB2CallBack;
    }
#endif

}



////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeRinkBoards::updateCornerPosition()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeRinkBoards::updateCornerPosition()
{
    // Do nothing, the rink board cannot be directly edited. All cal to modify it
    // should come from the nodes linked to it 
}



#if MIKE_DEBUG_
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////