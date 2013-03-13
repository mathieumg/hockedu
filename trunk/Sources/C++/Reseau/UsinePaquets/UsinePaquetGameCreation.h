//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetGameStatus.h
/// @author Charles Etienne Lalonde
/// @date 2013-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "../Paquets/PaquetGameCreation.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetGameStatus
/// @brief Classe Factory (Usine) pour les paquets de type GameStatus
///
/// @author Mathieu Parent
/// @date 2013-02-17
///////////////////////////////////////////////////////////////////////////
class UsinePaquetGameCreation : public UsinePaquet
{
public:
    /// Constructeur par param�tres.
    inline UsinePaquetGameCreation();

	/// Fonction � surcharger pour la cr�ation d'un noeud.
	inline virtual Paquet* creerPaquet() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetGameStatus::UsinePaquetGameStatus()
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres..
///
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetGameCreation::UsinePaquetGameCreation()
	: UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetGameStatus::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement cr�� du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetGameCreation::creerPaquet() const
{
	return new PaquetGameCreation();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





