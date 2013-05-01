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
    Super(RazerGameUtilities::NAME_RINK_BOARD), mPoint1(n1),mPoint2(n2)
{
    initBase();
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

        auto pos1 = obtenirCoin1();
        auto pos2 = obtenirCoin2();
        NoeudPoint* p1 = mPoint1, *p2 = mPoint2;
        if(IsInGame())
        {
            /// algo pour mixer les 2 edges colinaire ensemble
            static const unsigned int tolerance = 1;
            unsigned int angle = (unsigned int)mAngle;
            angle %=180;
            if((angle+tolerance < tolerance*2) || ((angle-180)+tolerance < tolerance*2))
            {
                if(p1 && p2)
                {
                    if(p1->obtenirTypePosNoeud() == POSITION_HAUT_MILIEU || p1->obtenirTypePosNoeud() == POSITION_BAS_MILIEU)
                    {
                        p1 = p2->obtenirPointSym();
                        pos1 = p1->getPosition();
                    }
                    else
                    {
                        return;
                    }
                }
            }
        }

       /* if(p1)
        {
            auto type = p1->obtenirTypePosNoeud();
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
        if(p2)
        {
            auto type = p2->obtenirTypePosNoeud();
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
        }*/

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody; //this will be a dynamic body
        myBodyDef.angle = 0; //set the starting angle
        mPhysicBody = world->CreateBody(&myBodyDef);


        b2Vec2 pos1B2,pos2B2;
        utilitaire::VEC3_TO_B2VEC(pos1,pos1B2);
        utilitaire::VEC3_TO_B2VEC(pos2,pos2B2);

        b2EdgeShape shape;
        shape.Set(pos1B2,pos2B2);

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