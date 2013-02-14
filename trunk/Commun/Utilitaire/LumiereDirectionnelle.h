///////////////////////////////////////////////////////////////////////////////
/// @file LumiereDirectionnelle.h
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
/// @class LumiereDirectionnelle
/// @brief Classe qui repr�sente une lumi�re directionnelle
///
/// @author Samuel Ledoux
/// @date 2012-03-26
///////////////////////////////////////////////////////////////////////////
class LumiereDirectionnelle : public LumiereAbstraite
{
public:
	/// Constructeur par parametre.
	LumiereDirectionnelle(GLfloat direction[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat attenuation[4]);
	/// Destructeur
	virtual ~LumiereDirectionnelle();
	/// Anime l'animation
	virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );
	/// Initialisation de la lumi�re
	virtual void initLumiere();



private:

};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


