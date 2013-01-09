///////////////////////////////////////////////////////////////////////////////
/// @file LumiereAbstraite.cpp
/// @author Samuel Ledoux
/// @date 2012-03-26
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "LumiereAbstraite.h"
////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereAbstraite::LumiereAbstraite( GLfloat position, GLfloat ambiante, GLfloat diffuse, GLfloat speculaire, int lienOpenGl )
///
/// Constructeur par paramètre d'une lumière abstraite
///
/// @param[in] GLfloat position : la position de la lumière dans l'espace
/// @param[in] GLfloat ambiante : la composante ambiante de l'intensite
/// @param[in] GLfloat diffuse : la composante diffuse de l'intensite
/// @param[in] GLfloat speculaire : la composante speculaire de l'intensite
/// @param[in] int lienOpenGl : le define openGL de la lumiere, ex : GL_LIGHT0
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereAbstraite::LumiereAbstraite( GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat attenuation[4] )
{
	// On copie tous les tableaux et le int qui représente la lumière
	for(int i = 0 ; i<4 ; i++)
	{
		position_[i]=position[i];
		ambiante_[i]=ambiante[i];
		diffuse_[i]=diffuse[i];
		speculaire_[i]=speculaire[i];
		attenuation_[i]=attenuation[i];
		if(attenuation[0]+attenuation[1]+attenuation[2]<1.0f) // On met une attenuation minimale pour ne pas effectuer de division par 0 dans le shader par defaut d'OpenGL
			attenuation_[0] = 1.0f;
	}
	lienOpenGL_=lienOpenGl;

}
////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereAbstraite::~LumiereAbstraite()
///
/// Destructeur
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereAbstraite::~LumiereAbstraite()
{

}
////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereAbstraite::allumerLumiere()
///
/// On allume la lumière
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LumiereAbstraite::allumerLumiere()
{
	enabled_ = true;
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereAbstraite::eteindreLumiere()
///
/// On éteint la lumière
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LumiereAbstraite::eteindreLumiere()
{
	enabled_ = false;
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereAbstraite::initLumiere( int lum )
///
/// initialisation des composantes de la lumière dans openGL
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LumiereAbstraite::initLumiere()
{
	glLightfv(lienOpenGL_, GL_POSITION,  position_ );
	glLightfv(lienOpenGL_, GL_AMBIENT,  ambiante_ );
	glLightfv(lienOpenGL_, GL_DIFFUSE,  diffuse_ );
	glLightfv(lienOpenGL_, GL_SPECULAR,  speculaire_ );
	glLightf(lienOpenGL_, GL_CONSTANT_ATTENUATION,  attenuation_[0] );
	glLightf(lienOpenGL_, GL_LINEAR_ATTENUATION,  attenuation_[1] );
	glLightf(lienOpenGL_, GL_QUADRATIC_ATTENUATION,  attenuation_[2] );

	if(enabled_)
	{
		glEnable(lienOpenGL_);
		glLightf(lienOpenGL_, GL_SPOT_EXPONENT,  1.0f );
	}
	else
	{
		glDisable(lienOpenGL_);
		glLightf(lienOpenGL_, GL_SPOT_EXPONENT,  0.0f );
	}

}



void LumiereAbstraite::animerAnimation()
{
	if(modParam1_)
	{
		
		ambiante_[0]	 = (float)animationParam1_[0];
		ambiante_[1]	 = (float)animationParam1_[1];
		ambiante_[2]	 = (float)animationParam1_[2];
		diffuse_[0]		 = (float)animationParam1_[0];
		diffuse_[1]		 = (float)animationParam1_[1];
		diffuse_[2]		 = (float)animationParam1_[2];
	}

	if(modParam2_)
	{
		if(animationParam2_[VX]<0.5)
			eteindreLumiere();
		else
			allumerLumiere();
	}

	
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


