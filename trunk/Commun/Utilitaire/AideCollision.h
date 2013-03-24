///////////////////////////////////////////////////////////////////////////////
/// @file AideCollision.h
/// @brief Ce fichier contient la déclaration de l'espace de nom aidecollision.
///
/// Il contient les déclarations de fonctions utiles pour le calcul des forces
/// causées par les collisions.
///
/// @author Martin Bisson
/// @date 2007-01-10
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_AIDECOLLISION_H__
#define __UTILITAIRE_AIDECOLLISION_H__


#include "Vecteur.h"

class NodeWallAbstract;

/// Espace de nom contenant des fonctions utiles pour le calcul des forces
/// causées par les collisions.
namespace aidecollision {


   /// Type de collisions possibles avec un segment.
   enum Collision {
      COLLISION_AUCUNE = 0 ,
      COLLISION_SEGMENT_PREMIERPOINT ,
      COLLISION_SEGMENT ,
      COLLISION_SEGMENT_DEUXIEMEPOINT ,
      COLLISION_SPHERE ,
      COLLISION_ARC
   };

   /// Structure contenant les informations d'une collision.
   class DetailsCollision {
   public:
	   DetailsCollision():type(COLLISION_AUCUNE){}
      /// Type de collision.
      Collision type;
      /// Direction de la collision.
      Vecteur3  direction;
      /// Enfoncement de l'objet à l'intérieur de la collision.
      float    enfoncement;
   };

   DetailsCollision calculerCollisionSegmentSegment( const Vecteur2& d1P1, const Vecteur2& d1P2, const Vecteur2& d2P1, const Vecteur2& d2P2, Vecteur2& intersection );


   /// Calcule la collision d'un objet circulaire avec un segment de droite.
   DetailsCollision calculerCollisionSegment(
      const Vecteur2& point1,
      const Vecteur2& point2,
      const Vecteur2& position,
      float          rayon,
      bool            collisionAvecPoints = true
      );

   /// Calcule la collision d'un objet sphérique avec un segment de droite.
   DetailsCollision calculerCollisionSegment(
      const Vecteur3& point1,
      const Vecteur3& point2,
      const Vecteur3& position,
      float          rayon,
      bool            collisionAvecPoints = true
      );

   /// Calcule la collision d'un objet circulaire avec un cercle.
   DetailsCollision calculerCollisionCercle(
      const Vecteur2& centreCercle,
      float          rayonCercle,
      const Vecteur2& positionObjet,
      float          rayonObjet
      );

   /// Calcule la collision d'un objet circulaire avec un arc de cercle.
   DetailsCollision calculerCollisionArc(
      const Vecteur2& centreCercle,
      const Vecteur2& pointArc1,
      const Vecteur2& pointArc2,
      const Vecteur2& positionObjet,
      float          rayonObjet
      );

   /// Calcule la collision d'un objet sphérique avec une sphère.
   DetailsCollision calculerCollisionSphere(
      const Vecteur3& centreSphere,
      float          rayonSphere,
      const Vecteur3& positionObjet,
      float          rayonObjet
      );

   /// Calcule la force en deux dimensions à partir d'une collision.
   Vecteur2 calculerForceRebondissement2D(
      const DetailsCollision& details,
      float constanteRebondissement
      );

   /// Calcule la force en trois dimensions à partir d'une collision.
   Vecteur3 calculerForceRebondissement3D(
      const DetailsCollision& details,
      float constanteRebondissement
      );

   /// Calcule la force d'amortissement en deux dimensions au cours d'une
   /// collision.
   Vecteur2 calculerForceAmortissement2D(
      const DetailsCollision& details,
      const Vecteur2& vitesse,
      float constanteAmortissement
      );

   /// Calcule la force d'amortissement en trois dimensions au cours d'une
   /// collision.
   Vecteur3 calculerForceAmortissement3D(
      const DetailsCollision& details,
      const Vecteur3& vitesse,
      float constanteAmortissement
      );

   /// Calcule la force en deux dimensions causée par la collision d'un objet
   /// circulaire avec un segment de droite.
   Vecteur2 calculerCollisionSegment(
      const Vecteur2&   point1,
      const Vecteur2&   point2,
      const Vecteur2&   position,
      float            rayon,
      bool              collisionAvecPoints,
      float            constanteRebondissement,
      float            constanteAmortissement,
      const Vecteur2&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la force en trois dimensions causée par la collision d'un objet
   /// sphérique avec un segment de droite.
   Vecteur3 calculerCollisionSegment(
      const Vecteur3&   point1,
      const Vecteur3&   point2,
      const Vecteur3&   position,
      float            rayon,
      bool              collisionAvecPoints,
      float            constanteRebondissement,
      float            constanteAmortissement,
      const Vecteur3&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la force en deux dimensions causée par la collision d'un objet
   /// circulaire avec un cercle.
   Vecteur2 calculerCollisionCercle(
      const Vecteur2&   centreCercle,
      float            rayonCercle,
      const Vecteur2&   positionObjet,
      float            rayonObjet,
      float            constanteRebondissement,
      float            constanteAmortissement,
      const Vecteur2&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la force en trois dimensions causée par la collision d'un objet
   /// sphérique avec une sphère.
   Vecteur3 calculerCollisionSphere(
      const Vecteur3&   centreSphere,
      float            rayonSphere,
      const Vecteur3&   positionObjet,
      float            rayonObjet,
      float            constanteRebondissement,
      float            constanteAmortissement,
      const Vecteur3&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la combinaison de deux constantes de rebondissement.
   float calculerCombinaisonRebondissement(
      float constante1, float constante2
      );

   /// Calcule la combinaison de deux constantes d'amortissement.
   float calculerCombinaisonAmortissement(
      float constante1, float constante2
      );

   /// Calcule s'il y a collision avec ce muret a grande vitesse
   DetailsCollision calculerCollisionCercleSegmentGrandeVitesse(
	   const Vecteur2& anciennePos, 
	   const Vecteur2& nouvellePos, 
	   const float& rayon,
	   const Vecteur2& coinMuret1,
	   const Vecteur2& coinMuret2,
	   Vecteur2& intersection
	   );

   float TimeOfClosestApproach(const Vecteur2& Pa, const Vecteur2& Pb, const Vecteur2& Va, const Vecteur2& Vb, const float& Ra, const float& Rb, bool& collision);



} // Fin de l'espace de nom aidecollision.


#endif // __UTILITAIRE_AIDECOLLISION_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
