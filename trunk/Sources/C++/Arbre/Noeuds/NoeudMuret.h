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
/// @brief Classe qui représente un muret statique.
///
/// @author Michael Ferris
/// @date 2013-02-14
///////////////////////////////////////////////////////////////////////////
class NoeudMuret : public NodeWallAbstract
{
public:
    typedef NodeWallAbstract Super;
   /// Constructeur à partir du type du noeud.
   NoeudMuret(const std::string& typeNoeud);
   /// Destructeur.
   virtual ~NoeudMuret();
   
   /// fonction de comparaison de 2 noeuds
   virtual bool equals(NoeudAbstrait* n)
   {
       auto n2 = dynamic_cast<NoeudMuret*>(n);
       return !!n2 && 
           positionCoin1_ == n2->positionCoin1_ && 
           positionCoin2_ == n2->positionCoin2_ && 
           Super::equals(n);
   }


   /// Accesseur du coin1
   virtual const Vecteur3& obtenirCoin1() const;
   /// Accesseur du coin2
   virtual const Vecteur3& obtenirCoin2() const;
   /// Mutateur des coins
   virtual void assignerPositionCoin(int lequel, Vecteur3 position);

   /// Permet de recalculer la position des coin des muret suite a une rotation ou une mise a l'échelle
   virtual void updateCornerPosition();
   /// Creation du noeud XML du muret
   virtual XmlElement* createXmlNode();
   /// Initialisation du NoeudMuret à partir d'un element XML
   virtual bool initFromXml(const XmlElement* element);



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
