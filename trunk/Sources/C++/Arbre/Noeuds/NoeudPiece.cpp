///////////////////////////////////////////////////////////////////////////
/// @file NoeudPiece.cpp
/// @author Michael Ferris
/// @date 2012-04-12
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#include "NoeudPiece.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPiece::NoeudPiece(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe et base
/// et donner des valeurs par d�faut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NoeudPiece::NoeudPiece(const std::string& typeNoeud)
   : NoeudComposite(typeNoeud)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPiece::~NoeudPiece()
///
/// Ce destructeur d�sallouee la liste d'affichage du cube.
///
/// @return void.
///
////////////////////////////////////////////////////////////////////////
NoeudPiece::~NoeudPiece()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPiece::afficherConcret(  )
///
/// Cette fonction effectue le v�ritable rendu de l'objet.
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPiece::afficherConcret() const
{
	// Appel � la version de la classe de base pour l'affichage des enfants.
	NoeudComposite::afficherConcret();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::animer( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps. L'animation est laisser au parent.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPiece::animer( const float& temps)
{
   
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
