//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetPortal.h
/// @author Vincent Lemire
/// @date 2013-04-05
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "../Paquets/PaquetPortal.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetPortal
/// @brief Classe Factory (Usine) pour les paquets de type Portal
///
/// @author Vincent Lemire
/// @date 2013-03-04
///////////////////////////////////////////////////////////////////////////
class UsinePaquetPortal : public UsinePaquet
{
public:
    /// Constructeur par paramètres.
    inline UsinePaquetPortal();

    /// Fonction à surcharger pour la création d'un noeud.
    inline virtual Paquet* creerPaquet() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetPortal::UsinePaquetPortal(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetPortal::UsinePaquetPortal()
    : UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetPortal::creerPaquet() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetPortal::creerPaquet() const
{
    return new PaquetPortal();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





