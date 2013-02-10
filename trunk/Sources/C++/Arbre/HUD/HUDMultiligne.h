//////////////////////////////////////////////////////////////////////////////
/// @file HUDMultiligne.h
/// @author Gabriel Couvrette
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HUDElement.h"
#include "HUDTexte.h"

/// Type def d'un vecteur de string
typedef std::vector<std::string> ConteneurLignes;

///////////////////////////////////////////////////////////////////////////////
/// @class HUDMultiligne
/// @brief Classe qui représente un élément de texte multiligne dans le HUD.
///
/// @author Gabriel Couvrette
/// @date 2012-03-09
//////////////////////////////////////////////////////////////////////////////
class HUDMultiligne:public HUDElement
{
public:
	/// Constructeur
	HUDMultiligne(std::string messageMultiligne, Vecteur4f& couleur, std::string fontType="game_over");
	/// Destructeur
	~HUDMultiligne();

private :
	/// Le texte
	std::string texteMultiligne_;
	ConteneurLignes lignes_;//contient les strings des lignes separees en lignes.
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



