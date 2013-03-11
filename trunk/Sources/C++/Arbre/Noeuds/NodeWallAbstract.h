///////////////////////////////////////////////////////////////////////////
/// @file NodeWallAbstract.h
/// @author Michael Ferris
/// @date 2013-02-14
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NODEWALLABSTRACT_H__
#define __ARBRE_NOEUDS_NODEWALLABSTRACT_H__


#include "NoeudComposite.h"

///////////////////////////////////////////////////////////////////////////
/// @class NodeWallAbstract
/// @brief Classe qui représente la définition de base d'un muret.
///
/// @author Michael Ferris
/// @date 2013-02-14
///////////////////////////////////////////////////////////////////////////
class NodeWallAbstract : public NoeudComposite
{
public:
    typedef NoeudComposite Super;
   /// Constructeur à partir du type du noeud.
   NodeWallAbstract(const std::string& typeNoeud);
   /// Destructeur.
   ~NodeWallAbstract();


   /// fonction de comparaison de 2 noeuds
   virtual bool equals(NoeudAbstrait* n)
   {
       auto n2 = dynamic_cast<NodeWallAbstract*>(n);
       return !!n2 && 
           coefRebond_ == n2->coefRebond_ && 
           Super::equals(n);
   }
   /// accessor to the key to retrieve the list and/or 3D Model
   virtual const std::string& get3DModelKey() const;

   /// Affiche le muret.
   virtual void afficherConcret() const;
   /// Accesseur du coin1
   virtual const Vecteur3& obtenirCoin1() const = 0;
   /// Accesseur du coin2
   virtual const Vecteur3& obtenirCoin2() const = 0;
   /// updates angle, scale and position according to its corners
   virtual void updateWallProperties();

   /// Permet d'assigner les attribut nécessaire à la collision
   virtual void assignerAttributVisiteurCollision(VisiteurCollision* v);

   /// Creation du noeud XML du muret
   virtual XmlElement* creerNoeudXML();
   /// Initialisation du NodeWallAbstract à partir d'un element XML
   virtual bool initialiser(const XmlElement* element);
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v );

   /// Recreates the physics body according to current attributes
   virtual void updatePhysicBody();
   static const Vecteur3 DEFAULT_SIZE;
protected:
private:
   /// Coefficient different different pour chaque muret, sauvegarde en XML
   float coefRebond_;

   /// Accessors
public:
    /// Accesseur de coefRebond_
    float getReboundRatio() const { return coefRebond_; }
    void setReboundRatio(float val) { coefRebond_ = val; }
};


#endif // __ARBRE_NOEUDS_NodeWallAbstract_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
