///////////////////////////////////////////////////////////////////////////////
/// @file LumiereAmbiante.cpp
/// @author Samuel Ledoux
/// @date 2012-03-26
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "LumiereAmbiante.h"
#include "glew.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereAmbiante::LumiereAmbiante( GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl )
///
/// Constructeur par param�tre
///
/// @param[in] GLfloat position[4] : la position 
/// @param[in] GLfloat ambiante[4] : composante ambiante de la lumi�re
/// @param[in] GLfloat diffuse[4] : composante diffuse de la lumi�re
/// @param[in] GLfloat speculaire[4] :  composante speculaire de la lumi�re
/// @param[in] int lienOpenGl : lien vers le define openGL, ex : GL_LIGHT0
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereAmbiante::LumiereAmbiante(GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat attenuation[4])
	:LumiereAbstraite(position, ambiante, diffuse, speculaire, lienOpenGl, attenuation)
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereAmbiante::~LumiereAbstraite()
///
/// Destructeur
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereAmbiante::~LumiereAmbiante()
{
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereAmbiante::initLumiere()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LumiereAmbiante::initLumiere()
{
	LumiereAbstraite::initLumiere();
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




