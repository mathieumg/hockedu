//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurEcrireXML.h
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VisiteurNoeud.h"
#include "XMLUtils.h"


///////////////////////////////////////////////////////////////////////////
/// @class VisiteurEcrireXML
/// @brief Visiteur l'�criture des noeuds dans un fichier XML
///
/// @author Michael Ferris, Vincent Lemire
/// @date 2012-01-27
///////////////////////////////////////////////////////////////////////////
class VisiteurEcrireXML :
	public VisiteurNoeud
{
public:
	/// Constructeur par d�faut
	VisiteurEcrireXML(void);
	/// Destructeur
	~VisiteurEcrireXML(void);

	/// Visitation d'un noeud abstrait
	virtual void visiterNoeudAbstrait( NoeudAbstrait* noeud );
	/// Visitation d'un noeud composite
	virtual void visiterNoeudComposite( NoeudComposite* noeud );

	/// Accesseur de la racine
	XmlElement* obtenirRacine( );
	/// R�initialisation de l'arbre
	void reinitialiserRacine();
private:
	/// Les �l�ments XML
	XmlElement* racine_, *parent_;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

