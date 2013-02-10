//////////////////////////////////////////////////////////////////////////////
/// @file HUDMultiligne.cpp
/// @author Gabriel Couvrette
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDMultiligne.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDMultiligne::HUDMultiligne(std::string messageMultiligne, Vecteur4f& couleur, std::string fontType)
///
/// Constructeur par paramètres.
///
/// @param[in] std::string messageMultiligne : le texte.
/// @param[in] Vecteur4& couleur : la couleur de la surface.
/// @param[in] std::string fontType : la police de caractère.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDMultiligne::HUDMultiligne(std::string messageMultiligne, Vecteur4f& couleur, std::string fontType)
{
	texteMultiligne_=messageMultiligne;
	int indexDebutLigne=0;
	int i=0;
	for(;i<(int)texteMultiligne_.size();++i)
	{
		if(texteMultiligne_[i]=='\n')
		{
			std::string substring(texteMultiligne_.substr(indexDebutLigne, i-indexDebutLigne));
			lignes_.push_back(substring);
			indexDebutLigne=i+1;
		}
	}
	if(i!=indexDebutLigne)
	{
			std::string substring(texteMultiligne_.substr(indexDebutLigne, i-indexDebutLigne));
			lignes_.push_back(substring);
	}

	for(i=0;i<lignes_.size();++i)
	{
		HUDTexte* elementTexte=new HUDTexte(lignes_[i], couleur, fontType);
		elementTexte->modifierPosition(0, 1.0f-(lignes_.size()-i)*0.05f);
		add(elementTexte);
	}
}

	/**

	std::string texteMultiligne_;
	ConteneurLignes lignes_;//contient les strings des lignes separees en lignes.

	**/

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////