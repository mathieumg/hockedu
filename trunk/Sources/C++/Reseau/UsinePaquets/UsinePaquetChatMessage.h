//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetChatMessage.h
/// @author Mathieu Parent
/// @date 2013-01-26
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include "Reseau\UsinePaquets\UsinePaquet.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetChatMessage
/// @brief Classe Factory (Usine) pour les paquets de type ChatMessage
///
/// @author Mathieu Parent
/// @date 2013-01-26
///////////////////////////////////////////////////////////////////////////
class UsinePaquetChatMessage : public UsinePaquet
{
public:
    /// Constructeur par param�tres.
    inline UsinePaquetChatMessage();

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
inline UsinePaquetChatMessage::UsinePaquetChatMessage()
	: UsinePaquet("ChatMessage")
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
Paquet* UsinePaquetChatMessage::creerPaquet() const
{
	return new PaquetChatMessage();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





