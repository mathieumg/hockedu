///////////////////////////////////////////////////////////////////////////////
/// @file AideCollision.h
/// @brief Ce fichier contient la d�claration de l'espace de nom aidecollision.
///
/// Il contient les d�clarations de fonctions utiles pour le calcul des forces
/// caus�es par les collisions.
///
/// @author Martin Bisson
/// @date 2007-01-10
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_AIDECOLLISION_H__
#define __UTILITAIRE_AIDECOLLISION_H__


#include <Vecteur.h>

class NoeudMuret;

/// Espace de nom contenant des fonctions utiles pour le calcul des forces
/// caus�es par les collisions.
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
      /// Enfoncement de l'objet � l'int�rieur de la collision.
      double    enfoncement;
   };

   DetailsCollision calculerCollisionSegmentSegment( const Vecteur2& d1P1, const Vecteur2& d1P2, const Vecteur2& d2P1, const Vecteur2& d2P2, Vecteur2& intersection );


   /// Calcule la collision d'un objet circulaire avec un segment de droite.
   DetailsCollision calculerCollisionSegment(
      const Vecteur2& point1,
      const Vecteur2& point2,
      const Vecteur2& position,
      double          rayon,
      bool            collisionAvecPoints = true
      );

   /// Calcule la collision d'un objet sph�rique avec un segment de droite.
   DetailsCollision calculerCollisionSegment(
      const Vecteur3& point1,
      const Vecteur3& point2,
      const Vecteur3& position,
      double          rayon,
      bool            collisionAvecPoints = true
      );

   /// Calcule la collision d'un objet circulaire avec un cercle.
   DetailsCollision calculerCollisionCercle(
      const Vecteur2& centreCercle,
      double          rayonCercle,
      const Vecteur2& positionObjet,
      double          rayonObjet
      );

   /// Calcule la collision d'un objet circulaire avec un arc de cercle.
   DetailsCollision calculerCollisionArc(
      const Vecteur2& centreCercle,
      const Vecteur2& pointArc1,
      const Vecteur2& pointArc2,
      const Vecteur2& positionObjet,
      double          rayonObjet
      );

   /// Calcule la collision d'un objet sph�rique avec une sph�re.
   DetailsCollision calculerCollisionSphere(
      const Vecteur3& centreSphere,
      double          rayonSphere,
      const Vecteur3& positionObjet,
      double          rayonObjet
      );

   /// Calcule la force en deux dimensions � partir d'une collision.
   Vecteur2 calculerForceRebondissement2D(
      const DetailsCollision& details,
      double constanteRebondissement
      );

   /// Calcule la force en trois dimensions � partir d'une collision.
   Vecteur3 calculerForceRebondissement3D(
      const DetailsCollision& details,
      double constanteRebondissement
      );

   /// Calcule la force d'amortissement en deux dimensions au cours d'une
   /// collision.
   Vecteur2 calculerForceAmortissement2D(
      const DetailsCollision& details,
      const Vecteur2& vitesse,
      double constanteAmortissement
      );

   /// Calcule la force d'amortissement en trois dimensions au cours d'une
   /// collision.
   Vecteur3 calculerForceAmortissement3D(
      const DetailsCollision& details,
      const Vecteur3& vitesse,
      double constanteAmortissement
      );

   /// Calcule la force en deux dimensions caus�e par la collision d'un objet
   /// circulaire avec un segment de droite.
   Vecteur2 calculerCollisionSegment(
      const Vecteur2&   point1,
      const Vecteur2&   point2,
      const Vecteur2&   position,
      double            rayon,
      bool              collisionAvecPoints,
      double            constanteRebondissement,
      double            constanteAmortissement,
      const Vecteur2&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la force en trois dimensions caus�e par la collision d'un objet
   /// sph�rique avec un segment de droite.
   Vecteur3 calculerCollisionSegment(
      const Vecteur3&   point1,
      const Vecteur3&   point2,
      const Vecteur3&   position,
      double            rayon,
      bool              collisionAvecPoints,
      double            constanteRebondissement,
      double            constanteAmortissement,
      const Vecteur3&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la force en deux dimensions caus�e par la collision d'un objet
   /// circulaire avec un cercle.
   Vecteur2 calculerCollisionCercle(
      const Vecteur2&   centreCercle,
      double            rayonCercle,
      const Vecteur2&   positionObjet,
      double            rayonObjet,
      double            constanteRebondissement,
      double            constanteAmortissement,
      const Vecteur2&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la force en trois dimensions caus�e par la collision d'un objet
   /// sph�rique avec une sph�re.
   Vecteur3 calculerCollisionSphere(
      const Vecteur3&   centreSphere,
      double            rayonSphere,
      const Vecteur3&   positionObjet,
      double            rayonObjet,
      double            constanteRebondissement,
      double            constanteAmortissement,
      const Vecteur3&   vitesse,
      DetailsCollision* retourDetails = 0
      );

   /// Calcule la combinaison de deux constantes de rebondissement.
   double calculerCombinaisonRebondissement(
      double constante1, double constante2
      );

   /// Calcule la combinaison de deux constantes d'amortissement.
   double calculerCombinaisonAmortissement(
      double constante1, double constante2
      );

   /// Calcule s'il y a collision avec ce muret a grande vitesse
   DetailsCollision calculerCollisionCercleSegmentGrandeVitesse(
	   const Vecteur2& anciennePos, 
	   const Vecteur2& nouvellePos, 
	   const double& rayon,
	   const Vecteur2& coinMuret1,
	   const Vecteur2& coinMuret2,
	   Vecteur2& intersection
	   );

   double TimeOfClosestApproach(const Vecteur2& Pa, const Vecteur2& Pb, const Vecteur2& Va, const Vecteur2& Vb, const double& Ra, const double& Rb, bool& collision);



   enum CollisionCase{
	   AUCUNE_COLLISION_GV,
	   COLLISION_COIN_GAUCHE,
	   COLLISION_COIN_DROIT,
	   COLLISION_MUR_COMPLET,
	   COLLISION_CENTRE,
	   COLLISION_ENFONCEE
   };
   enum PositionCollision{POS_COLLISION_GAUCHE,POS_COLLISION_DROITE,POS_COLLISION_MILIEU,POS_COLLISION_FRONT};
   class CollisionGrandeVitesse
   {
   public:
	   CollisionGrandeVitesse():case_(AUCUNE_COLLISION_GV),intersectionMin_(0),distanceMinCarre_(9999999){}
	   ~CollisionGrandeVitesse(){if(!intersectionMin_)delete intersectionMin_;}
	   CollisionGrandeVitesse(const CollisionGrandeVitesse& CGV)
	   {
		   if(CGV.intersectionMin_)
			   intersectionMin_ = new Vecteur2(*CGV.intersectionMin_);
		   case_ = CGV.case_;
		   distanceMinCarre_ = CGV.distanceMinCarre_;
		   positionCollision_ = CGV.positionCollision_;
		   muret_ = CGV.muret_;
	   }
	   void updateCollisionGauche(const double& distanceCarre, const Vecteur2& intersection);
	   void updateCollisionMilieu(const double& distanceCarre, const Vecteur2& intersection);
	   void updateCollisionDroite(const double& distanceCarre, const Vecteur2& intersection);
	   void updateCollisionFront (const double& distanceCarre, const Vecteur2& intersection);
	   void saveMuret( NoeudMuret* muret)
	   {
		   muret_ = muret;
	   }

	   Vecteur2* intersectionMin_;
	   double distanceMinCarre_;
	   CollisionCase case_;
	   PositionCollision positionCollision_;
	   NoeudMuret* muret_;
   };

} // Fin de l'espace de nom aidecollision.


#endif // __UTILITAIRE_AIDECOLLISION_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
