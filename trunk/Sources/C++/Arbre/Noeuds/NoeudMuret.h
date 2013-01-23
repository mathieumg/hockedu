///////////////////////////////////////////////////////////////////////////
/// @file NoeudMuret.h
/// @author Samuel Ledoux
/// @date 2012-01-25
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDMURET_H__
#define __ARBRE_NOEUDS_NOEUDMURET_H__


#include "NoeudComposite.h"


///////////////////////////////////////////////////////////////////////////
/// @class NoeudMuret
/// @brief Classe qui repr�sente un muret.
///
/// @author Samuel Ledoux
/// @date 2012-01-25
///////////////////////////////////////////////////////////////////////////
class NoeudMuret : public NoeudComposite
{
public:
   /// Constructeur � partir du type du noeud.
   NoeudMuret(const std::string& typeNoeud, bool estSurTable = true);
   /// Destructeur.
   ~NoeudMuret();
   /// Assigne la position relative du noeud.
   inline virtual void assignerPositionRelative( const Vecteur3& positionRelative);
   /// Affiche le muret.
   virtual void afficherConcret() const;
   /// Effectue l'animation
   virtual void animer( const float& temps );
   /// Accueil un visiteur
   virtual void acceptVisitor( VisiteurNoeud& v);
   /// Accesseur du coin1
   virtual Vecteur3 obtenirCoin1();
   /// Accesseur du coin2
   virtual Vecteur3 obtenirCoin2();
   /// Mutateur des coins
   virtual void assignerPositionCoin(int lequel, Vecteur3 position);
   /// Retourne le rayon du noeud
   virtual float obtenirRayon() const;
   /// Permet d'assigner les attribut n�cessaire � la collision
   virtual void assignerAttributVisiteurCollision(VisiteurCollision* v);

   /// Accesseur de coefRebond_
   float obtenirCoefRebond() const { return coefRebond_; }
   void modifierCoefRebond(float val) { coefRebond_ = val; }
   /// Permet de recalculer la position des coin des muret suite a une rotation
   void majPosCoins();
   /// Creation du noeud XML du muret
   virtual TiXmlElement* creerNoeudXML();
   /// Initialisation du NoeudMuret � partir d'un element XML
   virtual bool initialiser(const TiXmlElement* element);
   /// Permet de calculer l'angle du muret selon ces coins
   void trouverAngle();
   /// Permet de modifier l'angle du muret et garde les coins a jour
   virtual void assignerAngle(const float& angle);
   /// Mutateur des facteurs d'echelle et conserve les coins a jour
   virtual void modifierEchelleCourante(const Vecteur3& echelleCourante);
   /// Recreates the physics body according to current attributes
   virtual void updatePhysicBody();
protected:
private:
   /// Coin1 du muret
   Vecteur3 positionCoin1_;
   /// Coin2 du muret
   Vecteur3 positionCoin2_;

   /// Coefficient different different pour chaque muret, sauvegarde en XML
   float coefRebond_;
};


#endif // __ARBRE_NOEUDS_NOEUDMURET_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
