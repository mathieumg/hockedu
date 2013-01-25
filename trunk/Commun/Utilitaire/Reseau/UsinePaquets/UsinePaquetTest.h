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
	/// Constructeur par param�tres.
	inline UsinePaquetTest();

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
inline UsinePaquetTest::UsinePaquetTest()
	: UsinePaquet("Test")
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
Paquet* UsinePaquetTest::creerPaquet() const
{
	return new PaquetTest();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////





