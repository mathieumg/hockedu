///////////////////////////////////////////////////////////////////////////////
/// @file NoeudPortail.cpp
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudPortail.h"
#include "FacadeModele.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::NoeudPortail(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::NoeudPortail(const std::string& typeNoeud)
   : NoeudAbstrait(typeNoeud), champAttractionActif_(true)
{   

   FacadeModele::obtenirInstance()->ajouterElementSurTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::~NoeudPortail()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudPortail::~NoeudPortail()
{
	FacadeModele::obtenirInstance()->supprimerElementSurTable(this);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudPortail::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::afficherConcret() const
{
    // Sauvegarde de la matrice.
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudAbstrait::afficherConcret();

    // Restauration de la matrice.
    glPopAttrib();
    glPopMatrix();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCube::animer( const float& temps)
///
/// Cette fonction effectue l'animation du noeud pour un certain
/// intervalle de temps.
///
/// @param[in] temps : Intervalle de temps sur lequel faire l'animation.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::animer( const float& temps)
{
	angle_ = (int)(angle_+temps*1000.0)%360;
	updateMatrice();
   // Appel à la version de la classe de base pour l'animation des enfants.
   NoeudAbstrait::animer(temps);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudPortail::accueillirVisiteurNoeud( VisiteurNoeudComposite& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeudComposite & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudPortail::accueillirVisiteurNoeud( VisiteurNoeud& v )
{
	v.visiterNoeudPortail(this);
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
