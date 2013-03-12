//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetAuthentificationServeurJeu.h
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "..\Paquets\PaquetAuthentificationServeurJeu.h"
#include "UsinePaquet.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetLoginInfo
/// @brief Classe Factory (Usine) pour les paquets de type LoginInfo
///
/// @author Mathieu Parent
/// @date 2013-02-11
///////////////////////////////////////////////////////////////////////////
class UsinePaquetAuthentificationServeurJeu : public UsinePaquet
{
public:
    /// Constructeur par param�tres.
    inline UsinePaquetAuthentificationServeurJeu();

	/// Fonction � surcharger pour la cr�ation d'un noeud.
	inline virtual Paquet* creerPaquet() const;
	
	
};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetLoginInfo::UsinePaquetLoginInfo(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets cr��s.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetAuthentificationServeurJeu::UsinePaquetAuthentificationServeurJeu()
	: UsinePaquet()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetLoginInfo::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement cr�� du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement cr��.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetAuthentificationServeurJeu::creerPaquet() const
{
	return new PaquetAuthentificationServeurJeu();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





