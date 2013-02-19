///////////////////////////////////////////////////////////////////////////////
/// @file NoeudAffichage.cpp
/// @author Mathieu Parent
/// @date 2012-03-13
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "NoeudAffichage.h"
#include "FacadeModele.h"
#include "GestionnaireModeles.h"
#include "Utilitaire.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAffichage::NoeudAffichage(const std::string& typeNoeud)
///
/// Ce constructeur ne fait qu'appeler la version de la classe de base
/// et donner des valeurs par défaut aux variables membres.
///
/// @param[in] typeNoeud : Le type du noeud.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudAffichage::NoeudAffichage(const std::string& typeNoeud)
   : NoeudAbstrait(typeNoeud),mModelKey(typeNoeud)
{   
	affiche_ = false;
	echelleCourante_ = Vecteur3(5, 5, 5);
	echelleBase_ = echelleCourante_;
	updateMatrice();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAffichage::~NoeudAffichage()
///
/// Ce destructeur désallouee la liste d'affichage du cube.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
NoeudAffichage::~NoeudAffichage()
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAffichage::afficherConcret() const
///
/// Cette fonction effectue le véritable rendu de l'objet.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAffichage::afficherConcret() const
{
	glColor3f(1, 0, 0);

	// Appel à la version de la classe de base pour l'affichage des enfants.
	NoeudAbstrait::afficherConcret();
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
void NoeudAffichage::animer( const float& temps)
{
	if(estAffiche())
	{
		echelleCourante_[VX]*=(1.0f-(temps*4));
		echelleCourante_[VY]*=(1.0f-(temps*4));
		echelleCourante_[VZ]*=(1.0f-(temps*4));
		updateMatrice();
	}
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudAffichage::acceptVisitor( VisiteurNoeudComposite& v )
///
/// Permet d'indiquer au visiteur le type concret du noeud courant.
///
/// @param[in] VisiteurNoeudComposite & v : Le visiteur du noeud.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAffichage::acceptVisitor( VisiteurNoeud& v )
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAffichage::modifierListes( std::string nomListe )
///
/// Permet de modifier les listes d'affichage du noeud
///
/// @param[in] std::string nomListe : nom de la cle de la liste voulue
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAffichage::modifierListes( const std::string& nomListe )
{
    mModelKey = nomListe;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAffichage::peindreElement()
///
/// Permet de desiner l'objet dans le HUD
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAffichage::peindreElement()
{
	afficher();
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
