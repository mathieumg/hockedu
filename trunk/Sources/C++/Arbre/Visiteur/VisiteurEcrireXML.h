//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurEcrireXML.h
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"

class TiXmlElement;

///////////////////////////////////////////////////////////////////////////
/// @class VisiteurEcrireXML
/// @brief Visiteur l'écriture des noeuds dans un fichier XML
///
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
///////////////////////////////////////////////////////////////////////////
class VisiteurEcrireXML :
	public VisiteurNoeud
{
public:
	/// Constructeur par défaut
	VisiteurEcrireXML(void);
	/// Destructeur
	~VisiteurEcrireXML(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );
	/// Visitation d'un noeud muret
	virtual void visiterNoeudMuret( NoeudMuret* noeud );
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

	/// Accesseur de la racine
	TiXmlElement* obtenirRacine( );
	/// Réinitialisation de l'arbre
	void reinitialiserRacine();
private:
	/// Les éléments XML
	TiXmlElement* racine_, *parent_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

