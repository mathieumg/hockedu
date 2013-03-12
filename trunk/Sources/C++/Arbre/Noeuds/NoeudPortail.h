///////////////////////////////////////////////////////////////////////////
/// @file NoeudPortail.h
/// @author Mathieu Parent
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDPORTAIL_H__
#define __ARBRE_NOEUDS_NOEUDPORTAIL_H__

#include "NoeudComposite.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudPortail
/// @brief Classe qui représente un portail.
///
/// @author Mathieu Parent
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudPortail : public NoeudAbstrait
{
public:
    typedef NoeudAbstrait Super;
   /// Constructeur à partir du type du noeud.
   NoeudPortail(const std::string& typeNoeud);
   /// Destructeur.
   ~NoeudPortail();

   /// fonction de comparaison de 2 noeuds
   virtual bool equals(NoeudAbstrait* n)
   {
       auto n2 = dynamic_cast<NoeudPortail*>(n);
       return !!n2 && mIsAttractionFieldActive == n2->mIsAttractionFieldActive && Super::equals(n);
   }
   /// Affiche le cube.
   virtual void afficherConcret() const;
   /// Fonction appeler dans afficher concret pour faire le
   /// rendu OpenGL, uniquement utilisé sous APPLE.
   /// utiliser les liste d'affichage pour windows
   virtual void renderOpenGLES() const;
   /// Effectue l'animation
   virtual void animer( const float& temps );
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v);

   /// Accesseur et modificateur sur l'activité du champ d'attraction
   inline bool isAttractionFieldActive() const{return mIsAttractionFieldActive;}
   void setIsAttractionFieldActive(const bool actif){mIsAttractionFieldActive=actif;}

   /// Recreates the physics body according to current attributes
   virtual void updatePhysicBody();

   static const float DEFAULT_RADIUS;
private:
	bool mIsAttractionFieldActive;
	
};


#endif // __ARBRE_NOEUDS_NOEUDPORTAIL_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
