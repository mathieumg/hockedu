///////////////////////////////////////////////////////////////////////////////
/// @file Droite3D.h
/// @author DGI
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_DROITE3D_H__
#define __UTILITAIRE_DROITE3D_H__


#include "Vecteur.h"


namespace math {
   
   
   class Plan3D;


   ///////////////////////////////////////////////////////////////////////////
   /// @class Droite3D
   /// @brief Classe qui représente une droite en 3 dimensions.
   ///
   /// Classe qui permet d'avoir une droite en 3D. \n
   /// Une droite dans l'espace 3D est définie par l'équation :
   ///
   ///  \f[  \frac {x - x_0 } {a} =  \frac { y - y_0 } {b} =  \frac { z - z_0 } {c}    \f]
   /// où le point @f$ (x_0, y_0, z_0) @f$ est un point de la droite
   /// et le vecteur de direction @f$ (a, b, c) @f$ est un vecteur parallèle
   /// à la droite.\n \n
   /// Une droite dans l'espace 3D passant par 2 points @f$  ( P_1, P_2) @f$ est définie par l'équation :
   ///
   ///  \f[  \frac {x - x_1 } {x_2 - x_1} =  \frac { y - y_1 } {y_2 - y_1} =  \frac { z - z_1 } {z_2 - z_1}    \f]
   /// \n\n
   /// Cette classe contient des méthodes pour :
   ///    @li construire une droite entre 2 points @f$ (P_1, P_2) @f$ ou avec un point @f$ P @f$ sur la droite
   ///      et un vecteur de direction @f$ (a, b, c) @f$;
   ///    @li trouver l'intersection entre une droite et un plan ;
   ///    @li intersectionSegment ;
   ///    @li distancePoint ;
   ///    @li perpendiculaireDroite;
   ///    @li les méthodes d'accès.
   ///
   /// @author DGI-2990
   /// @date 2005-09-27
   ///////////////////////////////////////////////////////////////////////////
   class Droite3D
   {
   public:
      /// Constructeur.
      Droite3D(const Vecteur3& point1, const Vecteur3& point2);

      /// Trouve l'intersection entre la droite et un plan.
       bool      intersection(const Plan3D& planCoupe, Vecteur3& intersection);
      /// Trouve l'intersection entre la droite et un segment.
      bool      intersectionSegment(const Vecteur3& point1, const Vecteur3& point2);
      /// Calcule la distance entre un point et la droite.
      float    distancePoint(const Vecteur3& centre);
      /// Trouve le point de rencontre entre la droite et une perpendiculaire à partir d'un point.
      Vecteur3 perpendiculaireDroite(const Vecteur3& point);

      /// Avoir le vecteur directeur de la droite.
      inline const Vecteur3& lireVecteur() const;
      /// Avoir un point de la droite.
      inline const Vecteur3& lirePoint() const;


   private:
      /// @f$ (x_0, y_0, z_0) @f$
      const Vecteur3 pointDroite_;
      /// @f$ (a, b, c) @f$
      const Vecteur3 direction_;

   };


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Vecteur3& Droite3D::lireVecteur() const
   ///
   /// Cette fonction retourne le vecteur directeur de la droite.
   ///
   /// @return Le vecteur directeur de la droite.
   ///
   ////////////////////////////////////////////////////////////////////////////
   inline const Vecteur3& Droite3D::lireVecteur() const
   {
      return direction_;
   }


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Vecteur3& Droite3D::lirePoint() const
   ///
   /// Cette fonction retourne un point quelconque de la droite.
   ///
   /// @return Un point quelconque de la droite.
   ///
   ////////////////////////////////////////////////////////////////////////////
   inline const Vecteur3& Droite3D::lirePoint() const
   {
      return pointDroite_;
   }


} // Fin de l'espace de nom math.


#endif // __UTILITAIRE_DROITE3D_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
