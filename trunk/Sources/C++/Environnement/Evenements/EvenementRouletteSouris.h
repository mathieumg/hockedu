//////////////////////////////////////////////////////////////////////////////
/// @file EvenementRouletteSouris.h
/// @author Charles Etienne Lalonde
/// @date 2012-01-25
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "jni.h"
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
	/// Constructeurs par paramètre
	EvenementRouletteSouris(JNIEnv* env, jobject& evenementRouletteSouris);
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



