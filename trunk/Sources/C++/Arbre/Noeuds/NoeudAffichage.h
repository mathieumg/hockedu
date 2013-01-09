///////////////////////////////////////////////////////////////////////////
/// @file NoeudAffichage.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NoeudAffichage_H__
#define __ARBRE_NOEUDS_NoeudAffichage_H__

#include "NoeudComposite.h"
#include "HUDElement.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudAffichage
/// @brief Classe qui représente un portail.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudAffichage : public NoeudAbstrait, HUDElement
{
public:
   /// Constructeur à partir du type du noeud.
   NoeudAffichage(const std::string& typeNoeud);
   /// Destructeur.
   ~NoeudAffichage();

   /// Affiche le cube.
   virtual void afficherConcret() const;
   /// Effectue l'animation
   virtual void animer( const float& temps );
   /// Accueil un visiteur
   virtual void accueillirVisiteurNoeud( VisiteurNoeud& v);

   /// Modifie les listes d'affichage
   void modifierListes(const std::string& nomListe);

   /// Reset l'echele a l'origine
   virtual void resetEchelle() {echelleCourante_ = echelleBase_;}

   virtual void peindreElement();

private:
	Vecteur3 echelleBase_;
	
};


#endif // __ARBRE_NOEUDS_NoeudAffichage_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
