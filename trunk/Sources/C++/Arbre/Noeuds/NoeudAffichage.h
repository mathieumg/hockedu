///////////////////////////////////////////////////////////////////////////
/// @file NoeudAffichage.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NoeudAffichage_H__
#define __ARBRE_NOEUDS_NoeudAffichage_H__

#include "NoeudComposite.h"
#include "HUDElement.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudAffichage
/// @brief Classe qui repr�sente un Noeud a etre afficher dans le HUD.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudAffichage : public NoeudAbstrait, HUDElement
{
public:
   /// Constructeur � partir du type du noeud.
   NoeudAffichage();
   /// Destructeur.
   virtual ~NoeudAffichage();

   /// Affiche le cube.
   virtual void renderReal() const;
   /// Effectue l'animation
   virtual void tick( const float& temps );
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v);
   
   /// Reset l'echele a l'origine
   virtual void resetEchelle() 
   {
       mScale = echelleBase_;
   }

   virtual void peindreElement();

private:
	Vecteur3 echelleBase_;	
};


#endif // __ARBRE_NOEUDS_NoeudAffichage_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
