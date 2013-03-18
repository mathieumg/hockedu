//////////////////////////////////////////////////////////////////////////////
/// @file EvenementRouletteSouris.h
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
#include "Vecteur.h"


///////////////////////////////////////////////////////////////////////////
/// @class EvenementRouletteSouris
/// @brief Classe qui sert d'interface des événements de la roulette de la souris provenant de Java
///
/// @author Charles Etienne Lalonde
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class EvenementRouletteSouris
{
public:
#if WITH_JAVA  
	/// Constructeurs par paramètre
	EvenementRouletteSouris(JNIEnv* env, jobject& evenementRouletteSouris);
#endif //WITH_JAVA  
	EvenementRouletteSouris(int nbCoche):nbCoches_(nbCoche){}

	/// Retourne le nombre de coches de la roulette de la souris
	/// Si le nombre de coche est négatif, c'est qu'on roule vers l'avant.
	int obtenirNbCoches();
private:
	/// Nombre de coche de la roulette de la souris
	int nbCoches_;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



