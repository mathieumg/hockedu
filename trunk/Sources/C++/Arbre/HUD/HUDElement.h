//////////////////////////////////////////////////////////////////////////////
/// @file HUDElement.h
/// @author Gabriel Couvrette, Charles Étienne Lalonde
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
/// @brief Classe qui représente un élément pouvant être tracé dans le HUD.
///
/// @author Gabriel Couvrette, Charles Étienne Lalonde
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

	/// Modifier les propriétés
	void modifierPosition(float x, float y);
	void modifierTaille( float width, float height);

	/// Accéder au propriété
	float obtenirLargeur();
	float obtenirHauteur();
	float obtenirX();
	float obtenirY();

	/// Ajout d'un élément
	void add(HUDElement* element);
	/// Suppression d'un element
	void supprimerElement(HUDElement* element);
	/// Permet d'enlever tous les enfants de l'element
	void vider();
	/// Obtenir le parent de l'élément
	HUDElement* obtenirParent();
	/// Modifier le parent de l'élément
	void modifierParent(HUDElement* parent);

	/// Visibilité de l'élément
	virtual bool estVisible();
	void modifierVisibilite(bool visible);

protected:
	/// Appliquer la couleur
	virtual void peindreElement(){};
	/// Position et taille
	float x_, y_, largeur_, hauteur_;
private:
	/// Visibilité
	bool estVisible_;
	/// Enfants de l'éléments
	ListeEnfants listeEnfants_;
	/// Parent de l'élément
	HUDElement* parent_;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////




