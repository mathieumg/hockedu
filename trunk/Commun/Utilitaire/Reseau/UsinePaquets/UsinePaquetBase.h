//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetBase.h
/// @author Mathieu Parent
/// @date 2013-01-09
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "Paquets\PaquetBase.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetBase
/// @brief Classe Factory (Usine) pour les paquets qui contiennent les headers de base
///
/// @author Mathieu Parent
/// @date 2013-01-09
///////////////////////////////////////////////////////////////////////////
class UsinePaquetBase : public UsinePaquet
{
public:
	/// Constructeur par paramètres.
	inline UsinePaquetBase();

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual Paquet* creerPaquet() const;
	
	
};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetBase::UsinePaquetBase()
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetBase::UsinePaquetBase()
	: UsinePaquet("Base")
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetBase::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetBase::creerPaquet() const
{
	return new PaquetBase(getNom());
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
