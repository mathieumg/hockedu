//////////////////////////////////////////////////////////////////////////////
/// @file UsinePaquet.h
/// @author Mathieu Parent
/// @date 2012-12-30
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <map>
#include "../Paquets\Paquet.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsinePaquet
/// @brief Classe Factory (Usine) de base (abstraite) qui fournit l'interface aux autres Usines de Paquets
///
/// @author Mathieu Parent
/// @date 2012-12-30
///////////////////////////////////////////////////////////////////////////
class UsinePaquet
{
public:
	/// Destructeur vide d�clar� virtuel pour les classes d�riv�es.
	inline virtual ~UsinePaquet() {};

	/// Fonction � surcharger pour la cr�ation d'un paquet.
	virtual Paquet* creerPaquet() const = 0;

	/// Retourne le nom associ� � l'usine
	inline const std::string& getNom() const;

private:
	/// Le nom associ� � l'usine
	std::string nom_;

protected:
	/// Constructeur qui prend le nom associ� � l'usine.
	UsinePaquet(const std::string& nom) : nom_(nom) {}
	
	
};



////////////////////////////////////////////////////////////////////////
///
/// @fn inline const std::string& UsinePaquet::obtenirNom() const
///
/// Cette fonction retourne une cha�ne repr�sentante le nom associ� �
/// l'usine.
///
/// @return Le nom associ� � l'usine.
///
////////////////////////////////////////////////////////////////////////
inline const std::string& UsinePaquet::getNom() const
{
	return nom_;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




