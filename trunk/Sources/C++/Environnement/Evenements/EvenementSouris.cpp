//////////////////////////////////////////////////////////////////////////////
/// @file EvenementSouris.cpp
/// @author Michael Ferris
/// @date 2012-01-24
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "EvenementSouris.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn EvenementSouris( JNIEnv* env, jobject& evenementSouris )
///
/// Constructeur par param�tre qui cr�er des objets C++ � partir de
/// l'environnement Java pour facilit� l'utilisation
///
/// @param[in] env : pointeur vers l'environnement Java
/// @param[in] evenementSouris : objet java contenant l'information de l'�v�nement souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
EvenementSouris::EvenementSouris( JNIEnv* env, jobject& evenementSouris )
{
	jclass classe = env->GetObjectClass(evenementSouris);
	jmethodID getButton = env->GetMethodID(classe, "getButton", "()I");
	bouton_ = BoutonSouris(env->CallIntMethod(evenementSouris, getButton));

	//Appel de evenementSouris.getX()
	jmethodID getX = env->GetMethodID(classe, "getX", "()I");
	jint x = env->CallIntMethod(evenementSouris, getX);

	//Appel de evenementSouris.getY()
	jmethodID getY = env->GetMethodID(classe, "getY", "()I");
	jint y = env->CallIntMethod(evenementSouris, getY);
	
	position_ = Vecteur2i(x,y);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn EvenementSouris( const EvenementSouris& evenementSouris )
///
/// Constructeur par copie
///
/// @param[in] evenementSouris : objet d'�v�nement souris � copier
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
EvenementSouris::EvenementSouris( const EvenementSouris& evenementSouris )
{
	if(this != &evenementSouris)
	{
		position_ = evenementSouris.position_;
		bouton_ = evenementSouris.bouton_;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn EvenementSouris::~EvenementSouris(void)
///
/// Destructeur vide
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
EvenementSouris::~EvenementSouris(void)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn BoutonSouris EvenementSouris::getButton() const
///
/// permet d'obtenir la position de la souris sur l'�cran
///
///
/// @return la position de la souris
///
////////////////////////////////////////////////////////////////////////
BoutonSouris EvenementSouris::obtenirBouton() const
{
	return bouton_;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


