///////////////////////////////////////////////////////////////////////////////
/// @file Plan3D.cpp
/// @author DGI
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "Plan3D.h"


namespace math {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Plan3D::Plan3D(const Vecteur3& normale, const Vecteur3& pointDuPlan)
   ///
   /// Permet de construire un plan à partir d'une normale au plan et d'un
   /// point sur le plan.  La normale doit être non nulle.
   ///
   /// @param[in]  normale     : normale au plan
   /// @param[in]  pointDuPlan : un point sur le plan
   ///
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   Plan3D::Plan3D(const Vecteur3& normale, const Vecteur3& pointDuPlan)
      : normale_(normale) ,
        d_(-(normale[0]*pointDuPlan[0] + normale[1]*pointDuPlan[1] + normale[2]*pointDuPlan[2]) )
   {
      if (normale.estNul())
         throw ("Impossible de construire un plan puisqu'il n'a pas de normale");
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Plan3D::lireParam(double& a, double& b, double& c, double& d) const
   ///
   /// Lire les 4 coefficients qui définissent un plan en 3D.
   /// \f$ Ax + By + Cz + D = 0 \f$
   ///
   /// @param[out] a : coefficient @a A
   /// @param[out] b : coefficient @a B
   /// @param[out] c : coefficient @a C
   /// @param[out] d : coefficient @a D
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void Plan3D::lireParam(double& a, double& b, double& c, double& d) const
   {
      a = normale_[0];
      b = normale_[1];
      c = normale_[2];
      d = d_;
   }


} // Fin de l'espace de nom math.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
