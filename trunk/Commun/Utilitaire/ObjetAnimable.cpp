///////////////////////////////////////////////////////////////////////////////
/// @file ObjetAnimable.cpp
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "ObjetAnimable.h"
#include "GestionnaireAnimations.h"


///////////////////////////////////////////////////////////////////////////////
///
/// @fn ObjetAnimable::ObjetAnimable(  )
///
/// Ce constructeur initialise les valeurs de l'objet
///
/// @return Aucune (constructeur).
///
///////////////////////////////////////////////////////////////////////////////
ObjetAnimable::ObjetAnimable( void )
{
	animationParam1_ = Vecteur3(0, 0, 0);
	animationParam2_ = Vecteur3(0, 0, 0);
	animationParam3_ = Vecteur3(1, 1, 1);
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn ObjetAnimable::~ObjetAnimable(  )
///
/// Ce destructeur delie l'animation de l'objet
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
ObjetAnimable::~ObjetAnimable( void )
{
	GestionnaireAnimations::obtenirInstance()->delierObjet(this);
	
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn ObjetAnimable::updateAttributs( Vecteur3 param1, Vecteur3 param2, Vecteur3 param3 )
///
/// Met a jour les atttibuts de l'objet
/// 
/// @param param1	: nouvelle position
/// @param param2	: nouvel angle
/// @param param3	: nouvelleEchelle
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void ObjetAnimable::updateAttributs( Vecteur3 param1, Vecteur3 param2, Vecteur3 param3 )
{
	animationParam1_ = param1;
	animationParam2_ = param2;
	animationParam3_ = param3;
	animerAnimation();
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn ObjetAnimable::assignerModificateurs( bool modPosition, bool modAngle, bool modEchelle )
///
/// Met a jour les modificateurs de l'objet
/// 
/// @param modParam1: dit si la position doit etre affectee par l'animation
/// @param modParam2: dit si l'angle de rotation doit etre affectee par l'animation
/// @param modParam3: dit si l'echelle doit etre affectee par l'animation
///
/// @return void
///
///////////////////////////////////////////////////////////////////////////////
void ObjetAnimable::assignerModificateurs( bool modParam1, bool modParam2, bool modParam3 )
{
	modParam1_ = modParam1;
	modParam2_ = modParam2;
	modParam3_ = modParam3;
}






///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





