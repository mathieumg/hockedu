//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetGameConnection.h
/// @author Mathieu Parent
/// @date 2013-03-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "../Paquets/PaquetGameConnection.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetGameConnection
/// @brief Classe Factory (Usine) pour les paquets de type GameConnection
///
/// @author Mathieu Parent
/// @date 2013-03-17
///////////////////////////////////////////////////////////////////////////
class UsinePaquetGameConnection : public UsinePaquet
{
public:
    /// Constructeur par paramètres.
    inline UsinePaquetGameConnection();

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual Paquet* creerPaquet() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetGameConnection::UsinePaquetGameStatus()
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetGameConnection::UsinePaquetGameConnection()
	: UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetGameConnection::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetGameConnection::creerPaquet() const
{
	return new PaquetGameConnection();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





