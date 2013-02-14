//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetLoginInfo.h
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquets\UsinePaquet.h"
#include "..\Paquets\PaquetLoginInfo.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetLoginInfo
/// @brief Classe Factory (Usine) pour les paquets de type LoginInfo
///
/// @author Mathieu Parent
/// @date 2013-02-11
///////////////////////////////////////////////////////////////////////////
class UsinePaquetLoginInfo : public UsinePaquet
{
public:
    /// Constructeur par paramètres.
    inline UsinePaquetLoginInfo();

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual Paquet* creerPaquet() const;
	
	
};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetLoginInfo::UsinePaquetLoginInfo(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetLoginInfo::UsinePaquetLoginInfo()
	: UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetLoginInfo::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetLoginInfo::creerPaquet() const
{
	return new PaquetLoginInfo();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





