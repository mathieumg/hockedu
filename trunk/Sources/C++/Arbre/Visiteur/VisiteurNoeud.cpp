//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurNoeud.cpp
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurNoeud.h"
#include "NoeudComposite.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "NoeudTable.h"
#include "NoeudRondelle.h"
#include "NoeudPortail.h"
#include "NoeudMaillet.h"
#include "NoeudBut.h"
#include "NodeWallAbstract.h"
#include "NodeControlPoint.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurNoeud::VisiteurNoeud()
///
/// Constructeur par défaut
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurNoeud::VisiteurNoeud(void)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurNoeud::~VisiteurNoeud()
///
/// Destructeur vide
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurNoeud::~VisiteurNoeud(void)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudComposite( NoeudComposite* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudComposite * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudComposite( NoeudComposite* noeud )
{
    visiterNoeudAbstrait(noeud);
    unsigned int nbrEnfant = noeud->obtenirNombreEnfants();
    for (unsigned int i=0; i<nbrEnfant; ++i)
    {
        noeud->chercher(i)->acceptVisitor(*this);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// /*Description*/
///
/// @param[in] NodeWallAbstract * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudMuret( NodeWallAbstract* noeud )
{
    noeud->NodeWallAbstract::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudBut( NoeudBut* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudBut * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudBut( NoeudBut* noeud )
{
    noeud->NoeudBut::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudMaillet * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudMaillet( NoeudMaillet* noeud )
{
    noeud->NoeudMaillet::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudPortail( NoeudPortail* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudPortail * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudPortail( NoeudPortail* noeud )
{
    noeud->NoeudPortail::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudRondelle * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudRondelle( NoeudRondelle* noeud )
{
    noeud->NoeudRondelle::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudTable( NoeudTable* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudTable * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudTable( NoeudTable* noeud )
{
    noeud->NoeudTable::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudPoint( NoeudPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudPoint( NoeudPoint* noeud )
{
    noeud->NoeudPoint::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudAccelerateur * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
    noeud->NoeudAccelerateur::Super::acceptVisitor(*this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurNoeud::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurNoeud::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    noeud->NodeControlPoint::Super::acceptVisitor(*this);
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
