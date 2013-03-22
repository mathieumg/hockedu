//////////////////////////////////////////////////////////////////////////////
/// @file EvenementSouris.h
/// @author Michael Ferris
/// @date 2012-01-24
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vecteur.h"
#include "Enum_Declarations.h"

/// Type de bouton de la souris

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
    EvenementSouris();
	/// Constructeur par paramètre
	EvenementSouris(void* env, void* evenementSouris);
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

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


