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
/// @brief Classe qui repr�sente un noeud permettant de faire de l'affichage
///         version l�g�re d'un noeud.
///         La logique de modification des noeuds doit �tre fait de l'ext�rieur
///
/// @author Michael Ferris
/// @date 2013-03-13
///////////////////////////////////////////////////////////////////////////
class NodeModelRender : public NoeudComposite
{
public:
	/// Constructeur � partir du type du noeud.
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
