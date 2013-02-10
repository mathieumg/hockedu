//////////////////////////////////////////////////////////////////////////////
/// @file EvenementClavier.cpp
/// @author Charles Etienne Lalonde
/// @date 2012-01-25
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "EvenementClavier.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn EvenementClavier::EvenementClavier(JNIEnv* env, jobject& evenementClavier)
///
/// Construit un événement clavier avec un objet Java.
///
/// @param[in] JNIEnv* env : L'environnement Java.
/// @param[in] jobject& evenementClavier : L'événement clavier lu en Java.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
EvenementClavier::EvenementClavier(JNIEnv* env, jobject& evenementClavier)
{
	jclass classe = env->GetObjectClass(evenementClavier);
	
	// Appel de mouseEvent.getKeyCode()
	jmethodID getKeyCode = env->GetMethodID(classe, "getKeyCode", "()I");
	touche_ = env->CallIntMethod(evenementClavier, getKeyCode);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn EvenementClavier::EvenementClavier( ToucheClavier touche ) :touche_(touche)
///
/// Construit un événement clavier avec le code d'une touche.
///
/// @param[in] ToucheClavier touche : Le code de la touche.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
EvenementClavier::EvenementClavier( ToucheClavier touche ) :touche_(touche)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn ToucheClavier EvenementClavier::obtenirTouche()
///
/// Accesseur à la touche associée à l'événement.
///
/// @return : La touche du l'événement du clavier.
///
////////////////////////////////////////////////////////////////////////
ToucheClavier EvenementClavier::obtenirTouche()
{
	return touche_;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


