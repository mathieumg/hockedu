//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetMaillet.h
/// @author Mathieu Parent
/// @date 2013-03-04
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "../Paquets/PaquetMaillet.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetMaillet
/// @brief Classe Factory (Usine) pour les paquets de type Maillet
///
/// @author Mathieu Parent
/// @date 2013-03-04
///////////////////////////////////////////////////////////////////////////
class UsinePaquetMaillet : public UsinePaquet
{
public:
    /// Constructeur par paramètres.
    inline UsinePaquetMaillet();

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual Paquet* creerPaquet() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetMaillet::UsinePaquetMaillet(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetMaillet::UsinePaquetMaillet()
	: UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetMaillet::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetMaillet::creerPaquet() const
{
	return new PaquetMaillet();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





