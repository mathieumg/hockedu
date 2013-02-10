//////////////////////////////////////////////////////////////////////////////
/// @file HUDElement.cpp
/// @author Gabriel Couvrette, Charles �tienne Lalonde
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
/// Repeindre un �l�ment.
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
/// @param[in] float x : la coordonn�e en x.
/// @param[in] float y : la coordonn�e en y.
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
/// Modification de la taille de l'�l�ment.
///
/// @param[in] float largeur : la longueur de l'�l�ment.
/// @param[in] float hauteur : la hauteur de l'�l�ment.
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
/// Accesseur � la largeur de l'�l�ment.
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
/// Accesseur � la hauteur de l'�l�ment.
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
/// Accesseur � la coordonn�e en x.
///
/// @return : la coordonn�e en x.
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
/// Accesseur � la coordonn�e en y.
///
/// @return : la coordonn�e en y.
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
/// Accesseur au parent de l'�l�ment.
///
/// @return : l'�l�ment parent de this.
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
/// Assigner un nouveau parent � l'�l�ment.
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
/// Constructeur par d�faut.
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
/// Modificateur de la visibilit�.
///
/// @param[in] bool visible : la nouvelle visibilit�.
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
/// Accesseur sur la visibilit�.
///
/// @return : la visibilit� de l'�l�ment.
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
	// L'it�ration doit �tre faite ainsi pour �viter les probl�mes lorsque
	// le desctructeur d'un noeud modifie l'arbre, par exemple en retirant
	// d'autres noeuds.  Il pourrait y avoir une boucle infinie si la
	// desctruction d'un enfant entra�nerait l'ajout d'un autre.
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

