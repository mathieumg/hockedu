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
	NodeModelRender(RazerKey);
	/// Destructeur.
	virtual ~NodeModelRender();

	/// Affiche le cube.
	virtual void renderReal() const;
	/// Effectue l'animation du cube.
	virtual void tick( const float& temps );
    /// On redefini render pour ne pas dessiner cette node sur ipad
    virtual void renderOpenGLES() const;
	
private:

};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
