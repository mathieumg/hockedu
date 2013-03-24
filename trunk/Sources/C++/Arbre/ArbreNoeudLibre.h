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
/// @brief Repr�sente composite en forme d'arbre ayant des noeuds dont 
///		il ne doit pas s'occupe de la lib�ration
///		Red�finition de tous les m�thodes qui lib�re la m�moire des noeuds.
///
/// @author Michael Ferris
/// @date 2012-03-16
///////////////////////////////////////////////////////////////////////////
class ArbreNoeudLibre :
	public NoeudComposite
{
public:
	/// Vide le noeud de ses enfants sans lib�rer la m�moire.
	virtual void empty();
    /// Efface le noeud du conteneur de noeud, le noeud n'est pas lib�r�
    virtual bool erase( const NoeudAbstrait* noeud );
};


////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////




