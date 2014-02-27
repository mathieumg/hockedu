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
   NoeudPortail();
   /// Destructeur.
   virtual ~NoeudPortail();

   /// fonction de comparaison de 2 noeuds
   virtual bool equals(NoeudAbstrait* n)
   {
       auto n2 = dynamic_cast<NoeudPortail*>(n);
       return !!n2 && mIsAttractionFieldActive == n2->mIsAttractionFieldActive && Super::equals(n);
   }
   /// Affiche le cube.
   virtual void renderReal() const;
   /// Fonction appeler dans afficher concret pour faire le
   /// rendu OpenGL, uniquement utilisé sous APPLE.
   /// utiliser les liste d'affichage pour windows
   virtual void renderOpenGLES() const;
   /// Effectue l'animation
   virtual void tick( const float& temps );
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v);

   /// Accesseur et modificateur sur l'activité du champ d'attraction
   bool isAttractionFieldActive() const;
   void setIsAttractionFieldActive(const bool actif);

   /// Recreates the physics body according to current attributes
   virtual void updatePhysicBody();

   /// Accessors of mAttractionForce
   inline float getAttractionForce() const { return mAttractionForce; }
   inline void setAttractionForce( float pVal) { mAttractionForce = pVal; }

   /// Assigne la position relative du noeud.
   virtual void setPosition( const Vecteur3& positionRelative );

   /// Creation du noeud XML du Noeud
   virtual XmlElement* createXmlNode();
   /// Initialisation du NoeudAbstrait à partir d'un element XML
   virtual bool initFromXml(const XmlElement* element);

   static const float DEFAULT_RADIUS;
private:
	bool mIsAttractionFieldActive;

    /// Not USED Yet
	float mAttractionForce;
    class ForceField* mForceField;

};


#endif // __ARBRE_NOEUDS_NOEUDPORTAIL_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
