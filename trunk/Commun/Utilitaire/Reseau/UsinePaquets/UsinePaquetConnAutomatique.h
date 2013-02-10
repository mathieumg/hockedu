//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetConnAutomatique.h
/// @author Mathieu Parent
/// @date 2013-01-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "Paquets\PaquetConnAutomatique.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetConnAutomatique
/// @brief Classe Factory (Usine) pour les paquets de type connAutomatique
///
/// @author Mathieu Parent
/// @date 2013-01-13
///////////////////////////////////////////////////////////////////////////
class UsinePaquetConnAutomatique : public UsinePaquet
{
public:
	/// Constructeur par paramètres.
	inline UsinePaquetConnAutomatique();

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual Paquet* creerPaquet() const;
	
	
};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetString::UsinePaquetString(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetConnAutomatique::UsinePaquetConnAutomatique()
	: UsinePaquet("ConnAutomatique")
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetString::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetConnAutomatique::creerPaquet() const
{
	return new PaquetConnAutomatique(getNom());
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





