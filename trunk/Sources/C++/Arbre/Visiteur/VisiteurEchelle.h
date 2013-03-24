//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurEchelle.h
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"

///////////////////////////////////////////////////////////////////////////
/// @class VisiteurEchelle
/// @brief Visiteur pour la mise à l'échelle d'un noeud
///
/// @author Mathieu Parent
/// @date 2012-02-06
///////////////////////////////////////////////////////////////////////////
class VisiteurEchelle :
	public VisiteurNoeud
{
public:
	/// Constructeur par paramètre
	VisiteurEchelle(float facteur, int axe = -1);
	/// Destructeur
	virtual ~VisiteurEchelle(void);


	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Visitation d'un noeud muret
	virtual void visiterNoeudMuret( NodeWallAbstract* noeud );
	/// Visitation d'un noeud but
	virtual void visiterNoeudBut( NoeudBut* noeud );
	/// Visitation d'un noeud maillet
	virtual void visiterNoeudMaillet( NoeudMaillet* noeud );
	/// Visitation d'un noeud portail
	virtual void visiterNoeudPortail( NoeudPortail* noeud );
	/// Visitation d'un noeud rondelle
	virtual void visiterNoeudRondelle( NoeudRondelle* noeud );
	/// Visitation d'un noeud table
	virtual void visiterNoeudTable( NoeudTable* noeud );
	/// Visitation d'un noeud point
	virtual void visiterNoeudPoint( NoeudPoint* noeud );
	/// Visitation d'un noeud accélérateur
	virtual void visiterNoeudAccelerateur( NoeudAccelerateur* noeud );
    /// Visitation d'un noeud point de control
    virtual void visiterNodeControlPoint( NodeControlPoint* noeud );
private:
	/// Facteur de mise à l'échelle
	float facteur_;
	/// Axe selon lequel sera fait la mise à l'échelle
	int axe_;
	
	/// Diverses constantes pour la gestion de la taille des objets
	static const float FACTEUR_ECHELLE; // Facteur d'echelle
	static const float FACTEUR_ECHELLE_MAX; // Facteur d'echelle maximal
	static const float FACTEUR_ECHELLE_MIN; // Facteur d'echelle minimal
	
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

