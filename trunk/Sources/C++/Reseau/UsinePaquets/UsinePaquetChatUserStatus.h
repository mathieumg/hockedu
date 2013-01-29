//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetChatUserStatus.h
/// @author Mathieu Parent
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Reseau\UsinePaquets\UsinePaquet.h"
#include "..\Paquets\PaquetChatUserStatus.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetChatUserStatus
/// @brief Classe Factory (Usine) pour les paquets de type ChatUserStatus
///
/// @author Mathieu Parent
/// @date 2013-01-28
///////////////////////////////////////////////////////////////////////////
class UsinePaquetChatUserStatus : public UsinePaquet
{
public:
    /// Constructeur par param�tres.
    inline UsinePaquetChatUserStatus();

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
inline UsinePaquetChatUserStatus::UsinePaquetChatUserStatus()
	: UsinePaquet("ChatUserStatus")
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
Paquet* UsinePaquetChatUserStatus::creerPaquet() const
{
	return new PaquetChatUserStatus();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





