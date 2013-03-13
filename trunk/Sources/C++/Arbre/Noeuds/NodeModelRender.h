///////////////////////////////////////////////////////////////////////////
/// @file NodeModelRender.h
/// @author Michael Ferris
/// @date 2013-03-13
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once


#include "NoeudComposite.h"

///////////////////////////////////////////////////////////////////////////
/// @class NodeModelRender
/// @brief Classe qui représente un noeud permettant de faire de l'affichage
///         version légère d'un noeud.
///         La logique de modification des noeuds doit être fait de l'extérieur
///
/// @author Michael Ferris
/// @date 2013-03-13
///////////////////////////////////////////////////////////////////////////
class NodeModelRender : public NoeudComposite
{
public:
	/// Constructeur à partir du type du noeud.
	NodeModelRender(const std::string& typeNoeud);
	/// Destructeur.
	~NodeModelRender();

	/// Affiche le cube.
	virtual void afficherConcret() const;
	/// Effectue l'animation du cube.
	virtual void animer( const float& temps );
	
private:

};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
