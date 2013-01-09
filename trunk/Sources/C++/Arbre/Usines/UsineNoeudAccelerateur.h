///////////////////////////////////////////////////////////////////////////
/// @file UsineNoeudAccelerateur.h
/// @author Samuel Ledoux
/// @date 2012-2-7
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once


#include "UsineNoeud.h"
#include "NoeudAccelerateur.h"


///////////////////////////////////////////////////////////////////////////
/// @class UsineNoeudAccelerateur
/// @brief Classe qui représente une usine capable de créer des noeuds de
///        type NoeudAccelerateur.
///
/// @author Samuel Ledoux
/// @date 2012-02-7
///////////////////////////////////////////////////////////////////////////
class UsineNoeudAccelerateur : public UsineNoeud
{
public:
	/// Constructeur par paramètres.
	inline UsineNoeudAccelerateur(const std::string& nom);

	/// Fonction à surcharger pour la création d'un noeud.
	inline virtual NoeudAbstrait* creerNoeud() const;


};


////////////////////////////////////////////////////////////////////////
///
/// @fn UsineNoeudAccelerateur::UsineNoeudAccelerateur(const std::string& nom)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par défaut aux variables membres..
///
/// @param[in] nom   : Le nom de l'usine qui correspond au type de noeuds créés.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
inline UsineNoeudAccelerateur::UsineNoeudAccelerateur(const std::string& nom)
	: UsineNoeud(nom)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAbstrait* UsineNoeudAccelerateur::creerNoeud() const
///
/// Cette fonction retourne un noeud nouvellement créé du type produit
/// par cette usine, soit un accelerateur.
///
/// @return Le noeud nouvellement créé.
///
////////////////////////////////////////////////////////////////////////
NoeudAbstrait* UsineNoeudAccelerateur::creerNoeud() const
{
	return new NoeudAccelerateur(obtenirNom());
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
