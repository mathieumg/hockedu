//////////////////////////////////////////////////////////////////////////////
/// @file EvenementClavier.h
/// @author Charles Etienne Lalonde
/// @date 2012-01-25
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#if WITH_JAVA  
#include "jni.h"
#endif //WITH_JAVA  

typedef int ToucheClavier;

///////////////////////////////////////////////////////////////////////////
/// @class EvenementClavier
/// @brief Classe qui sert d'interface des événements du clavier provenant de Java
///
/// @author Charles Etienne
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class EvenementClavier
{
public:
#if WITH_JAVA  
	/// Constructeurs par objet Java
	EvenementClavier(JNIEnv* env, jobject& evenementClavier);
#endif //WITH_JAVA  

	/// Constructeur par paramètres
	EvenementClavier(ToucheClavier touche);

	/// Retourne la touche affectée par cet événement
	ToucheClavier obtenirTouche();

private:
	/// Touche du clavier
	ToucheClavier touche_;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


