///////////////////////////////////////////////////////////////////////////////
/// @file LumiereSpot.h
/// @author Samuel Ledoux
/// @date 2012-03-26
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "LumiereAbstraite.h"

///////////////////////////////////////////////////////////////////////////
/// @class LumiereSpot
/// @brief Classe qui repr�sente une lumi�re spot
///
/// @author Samuel Ledoux
/// @date 2012-03-26
///////////////////////////////////////////////////////////////////////////
class LumiereSpot : public LumiereAbstraite
{
public:
	/// Constructeur par parametre.
	LumiereSpot(GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat spotDirection[3], GLfloat spotExponent, GLfloat spotCutoff, GLfloat attenuation[4]);
	/// Destructeur
	virtual ~LumiereSpot();
	/// Anime l'animation
	virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );
	/// On reimplemente linitialisation pour un spot puisquil y a 3 autres parametres
	virtual void initLumiere();

private:
	/// propri�t�s de la lumi�re
	GLfloat spotDirection_[3];
	GLfloat spotExponent_;
	GLfloat spotCutoff_;
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
