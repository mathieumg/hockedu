///////////////////////////////////////////////////////////////////////////////
/// @file LumiereDirectionnelle.cpp
/// @author Samuel Ledoux
/// @date 2012-03-26
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "LumiereDirectionnelle.h"
#include "glew.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereDirectionnelle::LumiereDirectionnelle( GLfloat direction[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl )
///
/// Constructeur par paramètre
///
/// @param[in] GLfloat direction[4] : direction de la lumière
/// @param[in] GLfloat ambiante[4] : composante ambiante
/// @param[in] GLfloat diffuse[4] : composante diffuse
/// @param[in] GLfloat speculaire[4] :  composante speculaire
/// @param[in] int lienOpenGl : lien sur le define openGL, ex: GL_LIGHT0
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereDirectionnelle::LumiereDirectionnelle(GLfloat direction[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat attenuation[4])
	:LumiereAbstraite(direction, ambiante, diffuse, speculaire, lienOpenGl, attenuation)
{
	position_[3] = 0.0f;
}
////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereDirectionnelle::~LumiereDirectionnelle( void )
///
/// Destructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereDirectionnelle::~LumiereDirectionnelle()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereDirectionnelle::initLumiere()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LumiereDirectionnelle::initLumiere()
{
	LumiereAbstraite::initLumiere();
}



///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
