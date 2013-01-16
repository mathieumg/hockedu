//////////////////////////////////////////////////////////////////////////////
/// @file EvenementSouris.h
/// @author Michael Ferris
/// @date 2012-01-24
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <jni.h>
#include "Vecteur.h"

/// Type de bouton de la souris
enum BoutonSouris{
	AUCUN_BOUTON_SOURIS	,
	BOUTON_SOURIS_GAUCHE,
	BOUTON_SOURIS_MILIEU,
	BOUTON_SOURIS_DROIT	,
	NB_BOUTON_SOURIS	};
///////////////////////////////////////////////////////////////////////////
/// @class EvenementSouris
/// @brief Classe qui sert d'interface des événements souris provenant de Java
///
/// @author Michael Ferris
/// @date 2012-01-24
///////////////////////////////////////////////////////////////////////////
class EvenementSouris
{
public:
	/// Constructeur par paramètre
	EvenementSouris(JNIEnv* env, jobject& evenementSouris);
	/// Constructeur par paramètre
	EvenementSouris(Vecteur2i position,BoutonSouris bouton):position_(position),bouton_(bouton){}
	/// Constructeur de copie
	EvenementSouris(const EvenementSouris& me);
	/// Destructeur
	~EvenementSouris(void);
	/// Retourne le type de bouton de la souris
	BoutonSouris obtenirBouton() const;
	/// Retourne la position de la souris
	inline const Vecteur2i& obtenirPosition() const
    {
        return position_;
    }
private:
	/// Attributs de la souris
	Vecteur2i position_;
	BoutonSouris bouton_;

	/// Constructeur par défaut.  Déclaré mais non défini pour éviter qu'il soit
	/// généré par le compilateur.
	EvenementSouris();
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


