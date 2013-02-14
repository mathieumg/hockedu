///////////////////////////////////////////////////////////////////////////////
/// @file LumiereAmbiante.h
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
/// @class LumiereAmbiante
/// @brief Classe qui représente une lumière ambiante
///
/// @author Samuel Ledoux
/// @date 2012-03-26
///////////////////////////////////////////////////////////////////////////
class LumiereAmbiante : public LumiereAbstraite
{
public:
	/// Constructeur par parametre.
	LumiereAmbiante(GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat attenuation[4]);
	/// Destructeur
	virtual ~LumiereAmbiante();
	/// Anime l'animation
	virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );

	virtual void initLumiere();

	


private:

};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
