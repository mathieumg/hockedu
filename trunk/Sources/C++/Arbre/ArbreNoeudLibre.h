///////////////////////////////////////////////////////////////////////////////
/// @file ArbreNoeudLibre.h
/// @author Michael Ferris
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "NoeudComposite.h"

///////////////////////////////////////////////////////////////////////////
/// @class ArbreNoeudLibre
/// @brief Représente composite en forme d'arbre ayant des noeuds dont 
///		il ne doit pas s'occupe de la libération
///		Redéfinition de tous les méthodes qui libère la mémoire des noeuds.
///
/// @author Michael Ferris
/// @date 2012-03-16
///////////////////////////////////////////////////////////////////////////
class ArbreNoeudLibre :
	public NoeudComposite
{
public:
	/// Vide le noeud de ses enfants sans libérer la mémoire.
	virtual void empty();
    /// Efface le noeud du conteneur de noeud, le noeud n'est pas libéré
    virtual bool erase( const NoeudAbstrait* noeud );
};


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////




