///////////////////////////////////////////////////////////////////////////
/// @file NoeudMuret.h
/// @author Michael Ferris
/// @date 2013-02-14
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_NOEUDS_NOEUDMURET_H__
#define __ARBRE_NOEUDS_NOEUDMURET_H__


#include "NodeWallAbstract.h"

///////////////////////////////////////////////////////////////////////////
/// @class NoeudMuret
/// @brief Classe qui repr�sente un muret statique.
///
/// @author Michael Ferris
/// @date 2013-02-14
///////////////////////////////////////////////////////////////////////////
class NoeudMuret : public NodeWallAbstract
{
public:
    typedef NodeWallAbstract Super;
   /// Constructeur � partir du type du noeud.
   NoeudMuret(const std::string& typeNoeud);
   /// Destructeur.
   ~NoeudMuret();
   
   /// Accesseur du coin1
   virtual Vecteur3 obtenirCoin1();
   /// Accesseur du coin2
   virtual Vecteur3 obtenirCoin2();
   /// Mutateur des coins
   virtual void assignerPositionCoin(int lequel, Vecteur3 position);

   /// Permet de recalculer la position des coin des muret suite a une rotation ou une mise a l'�chelle
   void majPosCoins();
   /// Creation du noeud XML du muret
   virtual XmlElement* creerNoeudXML();
   /// Initialisation du NoeudMuret � partir d'un element XML
   virtual bool initialiser(const XmlElement* element);

   /// Permet de modifier l'angle du muret et garde les coins a jour
   virtual void assignerAngle(const float& angle);
   /// Mutateur des facteurs d'echelle et conserve les coins a jour
   virtual void modifierEchelleCourante(const Vecteur3& echelleCourante);
   /// Assigne la position relative du noeud et conserve les coins a jour
   inline virtual void assignerPositionRelative( const Vecteur3& positionRelative);

protected:
private:
   /// Coin1 du muret
   Vecteur3 positionCoin1_;
   /// Coin2 du muret
   Vecteur3 positionCoin2_;
};


#endif // __ARBRE_NOEUDS_NOEUDMURET_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
