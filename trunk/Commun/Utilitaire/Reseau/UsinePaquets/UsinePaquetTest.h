//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquetTest.h
/// @author Mathieu Parent
/// @date 2013-01-324
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "UsinePaquet.h"
#include "..\Paquets\PaquetTest.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquetTest
/// @brief Classe Factory (Usine) pour les paquets de type test
///
/// @author Mathieu Parent
/// @date 2013-01-24
///////////////////////////////////////////////////////////////////////////
class UsinePaquetTest : public UsinePaquet
{
public:
	/// Constructeur par paramètres.
	inline UsinePaquetTest();

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual Paquet* creerPaquet() const;
	
	
};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsinePaquetString::UsinePaquetString(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de paquets créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsinePaquetTest::UsinePaquetTest()
	: UsinePaquet("Test")
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Paquet* UsinePaquetString::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
Paquet* UsinePaquetTest::creerPaquet() const
{
	return new PaquetTest();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





