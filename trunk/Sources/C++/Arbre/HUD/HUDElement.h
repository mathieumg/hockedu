//////////////////////////////////////////////////////////////////////////////
/// @file HUDElement.h
/// @author Gabriel Couvrette, Charles �tienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "AideGL.h"

class HUDElement;
typedef std::vector<HUDElement*> ListeEnfants;

///////////////////////////////////////////////////////////////////////////
/// @class HUDElement
/// @brief Classe qui repr�sente un �l�ment pouvant �tre trac� dans le HUD.
///
/// @author Gabriel Couvrette, Charles �tienne Lalonde
/// @date 2012-03-09
///////////////////////////////////////////////////////////////////////////
class HUDElement
{
public:
	/// Constructeur
	HUDElement();
	/// Destructeur
	~HUDElement();
	/// Modification de la couleur
	void repeindre();

	/// Modifier les propri�t�s
	void modifierPosition(float x, float y);
	void modifierTaille( float width, float height);

	/// Acc�der au propri�t�
	float obtenirLargeur();
	float obtenirHauteur();
	float obtenirX();
	float obtenirY();

	/// Ajout d'un �l�ment
	void add(HUDElement* element);
	/// Suppression d'un element
	void supprimerElement(HUDElement* element);
	/// Permet d'enlever tous les enfants de l'element
	void vider();
	/// Obtenir le parent de l'�l�ment
	HUDElement* obtenirParent();
	/// Modifier le parent de l'�l�ment
	void modifierParent(HUDElement* parent);

	/// Visibilit� de l'�l�ment
	virtual bool estVisible();
	void modifierVisibilite(bool visible);

protected:
	/// Appliquer la couleur
	virtual void peindreElement(){};
	/// Position et taille
	float x_, y_, largeur_, hauteur_;
private:
	/// Visibilit�
	bool estVisible_;
	/// Enfants de l'�l�ments
	ListeEnfants listeEnfants_;
	/// Parent de l'�l�ment
	HUDElement* parent_;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




