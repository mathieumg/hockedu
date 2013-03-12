//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetRondelle.h
/// @author Mathieu Parent
/// @date 2013-03-04
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "../Paquets/PaquetRondelle.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetRondelle
/// @brief Classe Factory (Usine) pour les paquets de type Rondelle
///
/// @author Mathieu Parent
/// @date 2013-03-04
///////////////////////////////////////////////////////////////////////////
class UsinePaquetRondelle : public UsinePaquet
{
public:
    /// Constructeur par param�tres.
    inline UsinePaquetRondelle();

	/// Fonction � surcharger pour la cr�ation d'un noeud.
	inline virtual Paquet* creerPaquet() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetRondelle::UsinePaquetRondelle(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets cr��s.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetRondelle::UsinePaquetRondelle()
	: UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetRondelle::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement cr�� du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetRondelle::creerPaquet() const
{
	return new PaquetRondelle();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





