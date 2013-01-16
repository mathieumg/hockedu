#include "VisiteurFunction.h"



////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurFunction::VisiteurFunction( std::function<void (NoeudAbstrait*)> )
///
/// constructor
///
/// @param[in] std::function<void NoeudAbstrait *> : function à executer sur chaque noeud
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurFunction::VisiteurFunction( std::function<void (NoeudAbstrait*)> pFunction)
{
    mFunction = pFunction;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurFunction::~VisiteurFunction( void )
///
/// Destructor
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurFunction::~VisiteurFunction(void)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurFunction::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// /*Description*/
///
/// @param[in] NoeudAbstrait * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurFunction::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
    mFunction(noeud);
}
