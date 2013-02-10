//////////////////////////////////////////////////////////////////////////////
/// @file HUDElement.cpp
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDElement.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////
///
/// @fn  HUDElement::~HUDElement()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDElement::~HUDElement()
{
	vider();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::repeindre()
///
/// Repeindre un élément.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDElement::repeindre()
{
	if(estVisible())
	{
		peindreElement();
		for(int i=0;i<(int)listeEnfants_.size();++i)
		{
			listeEnfants_[i]->repeindre();
		}
	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::add(HUDElement *element)
///
/// Ajout d'un enfant.
///
/// @param[in] HUDElement *element : le nouvel enfant.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDElement::add(HUDElement *element)
{
	element->modifierParent(this);
	listeEnfants_.push_back(element);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::modifierPosition(float x, float y)
///
/// Modification de la position.
///
/// @param[in] float x : la coordonnée en x.
/// @param[in] float y : la coordonnée en y.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDElement::modifierPosition(float x, float y)
{
	x_=x;
	y_=y;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::modifierTaille( float largeur, float hauteur)
///
/// Modification de la taille de l'élément.
///
/// @param[in] float largeur : la longueur de l'élément.
/// @param[in] float hauteur : la hauteur de l'élément.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDElement::modifierTaille( float largeur, float hauteur)
{
	largeur_=largeur;
	hauteur_=hauteur;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float HUDElement::obtenirLargeur()
///
/// Accesseur à la largeur de l'élément.
///
/// @return : la largeur.
///
////////////////////////////////////////////////////////////////////////
float HUDElement::obtenirLargeur()
{
	if(obtenirParent() != NULL)
		return largeur_*obtenirParent()->obtenirLargeur();
	return largeur_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float HUDElement::obtenirHauteur()
///
/// Accesseur à la hauteur de l'élément.
///
/// @return la hauteur.
///
////////////////////////////////////////////////////////////////////////
float HUDElement::obtenirHauteur()
{
	if(obtenirParent() != NULL)
		return hauteur_*obtenirParent()->obtenirHauteur();
	return hauteur_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float HUDElement::obtenirX()
///
/// Accesseur à la coordonnée en x.
///
/// @return : la coordonnée en x.
///
////////////////////////////////////////////////////////////////////////
float HUDElement::obtenirX()
{
	if(obtenirParent() != NULL)
		return obtenirParent()->obtenirX() + obtenirParent()->obtenirLargeur()*x_;
	return x_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn float HUDElement::obtenirY()
///
/// Accesseur à la coordonnée en y.
///
/// @return : la coordonnée en y.
///
////////////////////////////////////////////////////////////////////////
float HUDElement::obtenirY()
{
	if(obtenirParent() != NULL)
		return obtenirParent()->obtenirY() + obtenirParent()->obtenirHauteur()*y_;
	return y_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDElement* HUDElement::obtenirParent()
///
/// Accesseur au parent de l'élément.
///
/// @return : l'élément parent de this.
///
////////////////////////////////////////////////////////////////////////
HUDElement* HUDElement::obtenirParent()
{
	return parent_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::modifierParent( HUDElement* parent )
///
/// Assigner un nouveau parent à l'élément.
///
/// @param[in] HUDElement* parent : le nouveau parent.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDElement::modifierParent( HUDElement* parent )
{
	parent_ = parent;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDElement::HUDElement()
///
/// Constructeur par défaut.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
HUDElement::HUDElement()
:x_(0), y_(0), largeur_(1), hauteur_(1), parent_(NULL), estVisible_(true)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::modifierVisibilite(bool visible)
///
/// Modificateur de la visibilité.
///
/// @param[in] bool visible : la nouvelle visibilité.
///
/// @return : aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDElement::modifierVisibilite(bool visible)
{
	estVisible_=visible;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool HUDElement::estVisible()
///
/// Accesseur sur la visibilité.
///
/// @return : la visibilité de l'élément.
///
////////////////////////////////////////////////////////////////////////
bool HUDElement::estVisible()
{
	return estVisible_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::supprimerElement( HUDElement* element )
///
/// Suppression d'un element
///
/// @param[in] HUDElement * element
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDElement::supprimerElement( HUDElement* element )
{
	ListeEnfants::iterator iter = find(listeEnfants_.begin(),listeEnfants_.end(),element);
	if(iter != listeEnfants_.end())
	{
		delete *iter;
		*iter = 0;
		listeEnfants_.erase(iter);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDElement::vider()
///
/// Permet d'enlever tous les enfants de l'element
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void HUDElement::vider()
{
	// L'itération doit être faite ainsi pour éviter les problèmes lorsque
	// le desctructeur d'un noeud modifie l'arbre, par exemple en retirant
	// d'autres noeuds.  Il pourrait y avoir une boucle infinie si la
	// desctruction d'un enfant entraînerait l'ajout d'un autre.
	while (!listeEnfants_.empty()) {
		HUDElement* enfantAEffacer = listeEnfants_.front();
		listeEnfants_.erase(listeEnfants_.begin());
		delete enfantAEffacer;
		enfantAEffacer = 0;
	}
}



///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////

