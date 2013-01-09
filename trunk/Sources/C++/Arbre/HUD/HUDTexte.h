//////////////////////////////////////////////////////////////////////////////
/// @file HUDTexte.h
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "phont.h"
#include "HUDElement.h"
#include <string>
enum TypeTexte {TEXTE, POINTAGE_JOUEUR_GAUCHE, POINTAGE_JOUEUR_DROIT, NOM_JOUEUR_GAUCHE, NOM_JOUEUR_DROIT, TEMPS_JEU, NOM_VAINQUEUR, NOM_OUTIL_MODE_EDITION, AIDE_OUTIL_MODE_EDITION, TOURNOI_JOUEUR};

///////////////////////////////////////////////////////////////////////////
/// @class HUDTexte
/// @brief Classe qui représente un élément pouvant être tracé dans le HUD.
///
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-09
///////////////////////////////////////////////////////////////////////////
class HUDTexte: public HUDElement
{
public : 
	/// Constructeurs par paramètres
	HUDTexte(std::string texte, Vecteur4f& couleur, std::string& typeFont=std::string("game_over"));
	HUDTexte(TypeTexte typeTexte, Vecteur4f& couleur, std::string& typeFont=std::string("game_over"));
	HUDTexte(unsigned int& idPartie, unsigned int& joueur, Vecteur4f& couleur = Vecteur4f(0.0f, 0.0f, 0.0f, 1.0f), Vecteur4f& couleurSiGagnantPartie = Vecteur4f(1.0f, 0.0f, 0.0f, 1.0f),std::string& typeFont=std::string("game_over"));

	/// Appliquer une couleur
	virtual void peindreElement();

    /// Accessors of message_
    inline std::string getMessage() const { return message_; }
    inline void setMessage(const std::string& val) { message_ = val; }
private :
	/// Le texte à afficher
	std::string message_;

    /// Le type de texte
	TypeTexte typeTexte_;
	/// La police de caractère
	Phont textRenderer_;
	/// La couleur du texte
	Vecteur4f couleur_;
	Vecteur4f couleurSiGagnantPartie_;
	unsigned int indexPartie_;
	unsigned int indexJoueur_;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////



