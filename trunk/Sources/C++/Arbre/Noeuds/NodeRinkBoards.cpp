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

#ifdef MIKE_DEBUG
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


#ifdef MIKE_DEBUG
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////