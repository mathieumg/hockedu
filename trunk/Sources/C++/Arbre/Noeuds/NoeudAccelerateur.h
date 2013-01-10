///////////////////////////////////////////////////////////////////////////////
/// @file NoeudAccelerateur.h
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "NoeudComposite.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudAccelerateur
/// @brief Classe qui représente un accélérateur.
///
/// @author Samuel Ledoux
/// @date 2012-01-27
///////////////////////////////////////////////////////////////////////////
class NoeudAccelerateur : public NoeudAbstrait
{
public:
	/// Constructeur à partir du type du noeud.
	NoeudAccelerateur(const std::string& typeNoeud);
	/// Destructeur.
	~NoeudAccelerateur();

	/// Affiche l'accelerateur.
	virtual void afficherConcret() const;
	/// Effectue l'animation de l'accelerateur.
	virtual void animer( const float& temps );

	virtual void accueillirVisiteurNoeud( VisiteurNoeud& v );

	/// Accesseur de bonusAccel_
	double obtenirBonusAccel() const { return bonusAccel_; }
	void modifierBonusAccel(double val) { bonusAccel_ = val; }

	/// Creation du noeud XML de l'accelerateur
	virtual TiXmlElement* creerNoeudXML();
	/// Initialisation du NoeudAccelerateur à partir d'un element XML
	virtual bool initialiser(const TiXmlElement* element);

	/// Accesseur de activer_
	bool estActiver() const { return activer_; }
	/// Modificateur de activer_
	void modifierActiver(bool val);

	/// Application de la physique de la rondelle
	virtual void gestionCollision( const float& temps );

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();

private:
	/// Acceleration fournie par les bonus accelerateurs
	double bonusAccel_;
	bool activer_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////