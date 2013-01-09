///////////////////////////////////////////////////////////////////////////////
/// @file LumiereSpot.cpp
/// @author Samuel Ledoux
/// @date 2012-03-26
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "LumiereSpot.h"
#include "AnimationFrame.h"
#include "Animation.h"
#include "GestionnaireAnimations.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereSpot::LumiereSpot( GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat spotDirection[3], GLfloat spotExponent, GLfloat spotCutoff )
///
/// Constructeur par paramètre
///
/// @param[in] GLfloat position[4] : position de la lumière
/// @param[in] GLfloat ambiante[4] : composante ambiante de la lumière
/// @param[in] GLfloat diffuse[4] :  composante diffuse de la lumière
/// @param[in] GLfloat speculaire[4] :  composante speculaire de la lumière
/// @param[in] int lienOpenGl : lien vers le define openGL, ex: GL_LIGHT0
/// @param[in] GLfloat spotDirection[3] : direction du spot
/// @param[in] GLfloat spotExponent :  exposant d'intensite de la lumière
/// @param[in] GLfloat spotCutoff : l'angle du spot
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereSpot::LumiereSpot(GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat spotDirection[3], GLfloat spotExponent, GLfloat spotCutoff, GLfloat attenuation[4])
	:LumiereAbstraite(position, ambiante, diffuse, speculaire, lienOpenGl, attenuation)
{
	for(int i = 0 ; i<3 ; i++)
	{
		spotDirection_[i]=spotDirection[i];
	}
	spotExponent_=spotExponent;
	spotCutoff_=spotCutoff;

}
////////////////////////////////////////////////////////////////////////
///
/// @fn  LumiereSpot::~LumiereSpot( void )
///
/// Destructeur
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
LumiereSpot::~LumiereSpot()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereSpot::animerAnimation()
///
/// Permet de créé une animation
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LumiereSpot::animerAnimation()
{
	LumiereAbstraite::animerAnimation();
	if(modParam2_)
	{
		spotCutoff_ = (float)animationParam2_[VY];
	}
	if(modParam3_)
	{
		Vecteur3 delta = animationParam3_-Vecteur3(position_[VX], position_[VY], position_[VZ]);
		delta.normaliser();
		spotDirection_[0] = (float)delta[0];
		spotDirection_[1] = (float)delta[1];
		spotDirection_[2] = (float)delta[2];

	}
	

}
////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereSpot::obtenirNom()
///
/// Permer d'afficher le nom de l'objet
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
std::string LumiereSpot::obtenirNom() const
{
    return "Lumiere Spot";
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void LumiereSpot::initLumiere()
///
/// On reimplemente linitialisation pour un spot puisquil y a 3 autres parametres
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LumiereSpot::initLumiere()
{
	glLightfv(lienOpenGL_, GL_SPOT_DIRECTION,  spotDirection_ );
	// On utilise le parametre exponent pour envoyer au shader si la lumiere est allumee
	glLightf(lienOpenGL_, GL_SPOT_CUTOFF,  spotCutoff_ );
	LumiereAbstraite::initLumiere();
// 
// 	glBegin(GL_LINES);
// 	glVertex3f(position_[0],position_[1],position_[2]);
// 	glVertex3f(position_[0]+(1000*spotDirection_[0]),position_[1]+(1000*spotDirection_[1]),position_[2]+(1000*spotDirection_[2]));
// 	glEnd();
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



