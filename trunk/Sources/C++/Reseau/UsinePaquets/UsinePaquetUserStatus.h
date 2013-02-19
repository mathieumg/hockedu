//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetUserStatus.h
/// @author Mathieu Parent
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "../Paquets/PaquetUserStatus.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetUserStatus
/// @brief Classe Factory (Usine) pour les paquets de type UserStatus
///
/// @author Mathieu Parent
/// @date 2013-01-28
///////////////////////////////////////////////////////////////////////////
class UsinePaquetUserStatus : public UsinePaquet
{
public:
    /// Constructeur par param�tres.
    inline UsinePaquetUserStatus();

	/// Fonction � surcharger pour la cr�ation d'un noeud.
	inline virtual Paquet* creerPaquet() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetString::UsinePaquetString(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets cr��s.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetUserStatus::UsinePaquetUserStatus()
	: UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetString::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement cr�� du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetUserStatus::creerPaquet() const
{
	return new PaquetUserStatus();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





