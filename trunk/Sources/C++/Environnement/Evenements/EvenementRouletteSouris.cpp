//////////////////////////////////////////////////////////////////////////////
/// @file EvenementRouletteSouris.cpp
/// @author Charles Etienne Lalonde
/// @date 2012-01-25
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "EvenementRouletteSouris.h"

#if WITH_JAVA  
////////////////////////////////////////////////////////////////////////
///
/// @fn EvenementRouletteSouris::EvenementRouletteSouris(JNIEnv* env, jobject& evenementRouletteSouris)
///
/// Contructeur par événement Java.
///
/// @param[in] JNIEnv* env : L'environnement Java.
/// @param[in] jobject& evenementRouletteSouris : L'événement envoyé par le Java.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
EvenementRouletteSouris::EvenementRouletteSouris(JNIEnv* env, jobject& evenementRouletteSouris)
{
	jclass classe = env->GetObjectClass(evenementRouletteSouris);
	
	//Appel de mouseWheelEvent.getWheelRotation()
	jmethodID getWheelRotation = env->GetMethodID(classe, "getWheelRotation", "()I");
	nbCoches_ = env->CallIntMethod(evenementRouletteSouris, getWheelRotation);
}
#endif //WITH_JAVA  

////////////////////////////////////////////////////////////////////////
///
/// @fn int EvenementRouletteSouris::obtenirNbCoches()
///
/// Accesseur au nombre de coches dont la roulette a bougé.
///
/// @return : Le nombre de coches.
///
////////////////////////////////////////////////////////////////////////
int EvenementRouletteSouris::obtenirNbCoches()
{
	return nbCoches_;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


