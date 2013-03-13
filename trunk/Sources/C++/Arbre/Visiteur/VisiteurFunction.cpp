#include "VisiteurFunction.h"



////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurFunction::VisiteurFunction( VisiteurFunctionFunction pFunction )
///
/// constructor
///
/// @param[in] VisiteurFunctionFunction : function à executer sur chaque noeud
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurFunction::VisiteurFunction( VisiteurFunctionFunction pFunction, void * pUserData /*= NULL*/):
    mFunction(pFunction),mUserData(pUserData)
{
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
    mFunction(noeud,mUserData);
}
