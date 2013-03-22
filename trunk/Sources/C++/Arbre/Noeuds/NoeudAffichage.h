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
/// @brief Classe qui représente un Noeud a etre afficher dans le HUD.
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
   virtual void renderReal() const;
   /// Effectue l'animation
   virtual void tick( const float& temps );
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v);

   /// accessor to the key to retrieve the list and/or 3D Model
   virtual const std::string& get3DModelKey() const {return mModelKey;}

   /// Modifie les listes d'affichage
   void modifierListes(const std::string& nomListe);

   /// Reset l'echele a l'origine
   virtual void resetEchelle() 
   {
       mScale = echelleBase_;
   }

   virtual void peindreElement();

private:
	Vecteur3 echelleBase_;
    std::string mModelKey;
	
};


#endif // __ARBRE_NOEUDS_NoeudAffichage_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
