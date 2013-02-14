///////////////////////////////////////////////////////////////////////////////
/// @file LumiereAbstraite.h
/// @author Samuel Ledoux
/// @date 2012-03-26
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
//#include "glew.h"
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include <vector>
#include "ObjetAnimable.h"

///////////////////////////////////////////////////////////////////////////
/// @class LumiereAbstraite
/// @brief Classe qui représente une lumière abstraite
///
/// @author Samuel Ledoux
/// @date 2012-03-26
///////////////////////////////////////////////////////////////////////////
class LumiereAbstraite : public ObjetAnimable
{
public:
	/// Constructeur par parametre.
	LumiereAbstraite(GLfloat position[4], GLfloat ambiante[4], GLfloat diffuse[4], GLfloat speculaire[4], int lienOpenGl, GLfloat attenuation[4]);
	/// Destructeur
	virtual ~LumiereAbstraite();
	/// Initialisation des parametres du lighting
	virtual void initLumiere();
	/// Ouvrir la lumiere voulu
	virtual void allumerLumiere();
	/// fermer la lumiere voulu
	virtual void eteindreLumiere();
	/// Anime l'animation
	virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult );

protected:
	int lienOpenGL_;
	bool enabled_;
	Vecteur4 position_;
	Vecteur4 ambiante_;
	Vecteur4 diffuse_;
	Vecteur4 speculaire_;
	Vecteur4 attenuation_; // dernier parametre inutilise
	
};


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
