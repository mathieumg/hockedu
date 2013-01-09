////////////////////////////////////////////////////////////////////////////////////
/// @file Plan3D.h
/// @author DGI
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////

#ifndef __UTILITAIRE_PLAN3D_H__
#define __UTILITAIRE_PLAN3D_H__


#include "Vecteur.h"


namespace math {
   
   
   ///////////////////////////////////////////////////////////////////////////
   /// @class Plan3D
   /// @brief Définition de la classe qui crée un plan en 3 dimensions.
   ///
   /// Classe qui permet d'avoir un plan en 3D.\n
   /// Un plan est défini par \f$ Ax + By + Cz + D = 0 \f$ où \f$ A, B \f$ et \f$ C  \f$ sont les
   /// composantes en  \f$ x, y \f$ et \f$ z  \f$ d'un vecteur normal au plan.
   ///
   /// @author DGI-2990
   /// @date 2005-09-27
   ///////////////////////////////////////////////////////////////////////////
   class Plan3D
   {
   public:
      /// Constructeur.
      Plan3D(const Vecteur3& normale, const Vecteur3& pointDuPlan);

      /// Lire la normale du plan.
      inline const Vecteur3& lireNormale() const;

      /// Lire les 4 paramètres qui définissent un plan en 3D.
      void lireParam(double& a, double& b, double& c, double& d) const;


   private:
      /// La normale du plan.
      const Vecteur3 normale_;

      /// Le décalage du plan.
      const double   d_;

   };


   ///////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Vecteur3& Plan3D::lireNormale() const
   ///
   /// Cette fonction retourne le vecteur normal au plan.
   ///
   /// @return Le vecteur normal au plan.
   ///
   ///////////////////////////////////////////////////////////////////////////////
   inline const Vecteur3& Plan3D::lireNormale() const
   {
      return normale_;
   }


} // fin de l'espace de nom math.


#endif // __UTILITAIRE_PLAN3D_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
