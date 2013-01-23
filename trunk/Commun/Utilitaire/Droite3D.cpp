///////////////////////////////////////////////////////////////////////////////
/// @file Droite3D.cpp
/// @author DGI
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#include "Droite3D.h"
#include "Plan3D.h"
#include "Utilitaire.h"


namespace math {
   
   
   ///////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Droite3D::Droite3D(const Vecteur3& point1, const Vecteur3& point2)
   ///
   /// Constructeur d'une droite 3D à partir de 2 points. \n
   /// Une droite dans l'espace 3d passant par 2 points
   /// @f$  ( P_1, P_2) @f$ est définie par l'équation :
   ///
   ///  \f[  \frac {x - x_1 } {x_2 - x_1} =  \frac { y - y_1 } {y_2 - y_1} =  \frac { z - z_1 } {z_2 - z_1}    \f]
   /// \n\n
   ///
   /// @param[in] point1 : Un point sur la droite.
   /// @param[in] point2 : Un autre point sur la droite (différent du premier
   ///                     sinon il y a une levée d'exception).
   ///
   /// @return Aucune (constructeur).
   ///
   ///////////////////////////////////////////////////////////////////////////////
   Droite3D::Droite3D(const Vecteur3& point1, const Vecteur3& point2)
      : direction_( point1, point2 ) , pointDroite_(point1)
   {
      if ( direction_.estNul() )
         throw ( "Impossible de construire une droite 3d puisque le vecteur de direction est nul." );
   }


   ///////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool Droite3D::intersection(const Plan3D& planCoupe, Vecteur3& intersection)
   ///
   /// Cette fonction permet de trouver l'intersection entre une droite et un plan
   /// dans l'espace 3D. La droite ne doit pas être parallèle au plan. \n
   ///
   ///  Si
   ///  @f$ a @f$
   ///  est différent de zéro, alors l'intersection est donnée par :\n
   ///
   ///   \f[ x = \left ( { \frac {\left ( \frac { ( B  b + C  c)  x_0} {a} - B  y_0 - C  z_0 - D \right)}  { ( A + B \frac  {b} {a} + C \frac{c} {b})} }\right) \f]
   ///   \f[ y = \left ( { \frac {\left ( C \frac {c}{b} y_0 - C  z_0 - D \right)}  { ( B + C \frac  {c} {b}) } }\right) \f]
   ///   \f[ z = \frac{c} {a} (x - x_0) + z_0  \f]
   ///
   ///  Si
   ///  @f$ b @f$
   ///  est différent de zéro, alors l'intersection est donnée par :\n
   ///
   ///   \f[ x = x_0 \f]
   ///   \f[ y = \frac{c} {b} (x - x_0) + y_0 \f]
   ///   \f[ z = \frac{c} {b} (y - y_0) + z_0  \f]
   ///
   ///  Sinon, l'intersection est donnée par :\n
   ///
   ///   \f[  x = x_0 \f]
   ///   \f[  y = y_0 \f]
   ///   \f[  z = \frac{(D + A x_0 + B y_0)} {C}   \f]
   ///
   ///  @param[in]  planCoupe : Le plan avec lequel on veut trouver l'intersection.
   ///  @param[out] intersection : Le résultat de l'intersection @f$ (s, y, z) @f$ .
   ///
   ///  @return Faux si la droite est parallèle au plan, donc si donc
   ///          l'intersection ne peut être trouvée, vrai autrement.
   ///
   ///////////////////////////////////////////////////////////////////////////////
   bool Droite3D::intersection( const Plan3D& planCoupe, Vecteur3& intersection )
   {
      // Initialisation de variables
      const float x0   = pointDroite_[0];
      const float y0   = pointDroite_[1];
      const float z0   = pointDroite_[2];

      const float a    = direction_[0];
      const float b    = direction_[1];
      const float c    = direction_[2];

      float A, B, C, D;
      planCoupe.lireParam( A, B, C, D );

      //Le résultat
      float x, y, z;

      // On regarde si le plan et la droite sont parallèles.
      bool bParalleles = utilitaire::EGAL_ZERO(
         produitScalaire( planCoupe.lireNormale(), direction_ )
         );

      if ( !bParalleles ) {
         // On doit faire attention aux divisions par 0
         if ( !utilitaire::EGAL_ZERO ( a ) ) {
            x = ( ( B * b + C * c ) * x0 / a - B * y0 - C * z0 - D ) / ( A + B * b / a + C * c / a );
            y = b / a * ( x - x0 ) + y0;
            z = c / a * ( x - x0 ) + z0;
         } else if ( !utilitaire::EGAL_ZERO ( b ) ) {
            y = ( C * c / b * y0 - A * x0 - C * z0 - D ) / ( B + C * c / b );
            x = x0;
            z = c / b * ( y - y0 ) + z0;
         } else {
            x = x0;
            y = y0;
            z = -( D + A * x0 + B * y0 ) / C;
         }

         intersection[0] = x;
         intersection[1] = y;
         intersection[2] = z;
      }

      return !bParalleles;
   }


   ///////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool Droite3D::intersectionSegment( const Vecteur3& point1, const Vecteur3& point2)
   ///
   /// Cette fonction permet de trouver l'intersection entre un segment de droite
   /// défini par 2 points @f$ (P_1, P_2) @f$ et une droite dans l'espace 3D.
   ///
   /// @param[in] point1 : Le premier point du segment.
   /// @param[in] point2 : Le deuxième point permettant de définir le segment de droite.
   ///
   /// @return    bool : false => la droite est parallèle au plan,
   ///                            donc l'intersection ne peut être trouvée.
   ///
   /// @date  2006-02-21  Modification suite aux changements dans Vecteur3
   ///
   ///////////////////////////////////////////////////////////////////////////////
   bool Droite3D::intersectionSegment(const Vecteur3& point1,
                                      const Vecteur3& point2)
   {
      Vecteur3 pointHaut;
      Vecteur3 pointBas;

      // Si le y n'est pas au niveau du x
      if ( ( pointDroite_[1] < point1[1] ) && ( pointDroite_[1] < point2[1] ) ) {
         return false;
      }
      if ( ( pointDroite_[1] > point1[1] ) && ( pointDroite_[1] > point2[1] ) ) {
         return false;
      }

      // Si les 2 X sont > ou < que le x du point
      if ( ( pointDroite_[0] > point1[0] ) &&
           ( pointDroite_[0] > point2[0] ) ) {
         return false;
      }
      else if ( ( pointDroite_[0] < point1[0] ) &&
                ( pointDroite_[0] < point2[0] ) ) {
         return true;
      }

      // Test lorsque le segment croise les deux axes;
      // On trouve les points et on les translate
      if ( point1[1] > point2[1] ) {
         pointHaut = point1;
         pointBas  = point2;
      }
      else {
         pointHaut = point2;
         pointBas  = point1;
      }
      pointHaut[0] =  pointHaut[0] - pointDroite_[0];
      pointHaut[1] =  pointHaut[1] - pointDroite_[1];
      pointBas[0]  =  pointBas[0]  - pointDroite_[0];
      pointBas[1]  =  pointBas[1]  - pointDroite_[1];

      // L'équation de la droite
      float m = ( pointHaut[1] - pointBas[1] ) / ( pointHaut[0] - pointBas[0] );
      float b = pointHaut[1] - pointHaut[0] * m;

      // Lorsque y = 0
      float x = ( -1.0f * b ) / m;

      if ( x > 0.0 ) {
         return true;
      }

      return false;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn float Droite3D::distancePoint( const Vecteur3& centre )
   ///
   /// Calcule la distance euclidienne entre la droite et un point.
   ///
   /// @param[in] centre : Point à partir duquel la distance doit être calculée.
   ///
   /// @return Distance du point à la droite.
   ///
   ////////////////////////////////////////////////////////////////////////
   float Droite3D::distancePoint( const Vecteur3& centre )
   {
      // En 2D
      const float ad = direction_[1];
      const float bd = -1.0f * direction_[0];
      const float cd = -1.0f * direction_[1] * pointDroite_[0] +
                        direction_[0] * pointDroite_[1];

      float num = fabs( ad * centre[0] + bd * centre[1] + cd );
      float den = sqrt( pow ( ad, 2 ) + pow ( bd, 2 ) );

      // En 3D
      const Vecteur3 centreNul(centre[0], centre[1], 0.0);
      const Vecteur3 vect( pointDroite_, centreNul );
      const Vecteur3 mult = produitVectoriel( vect, direction_ );
      num = mult.norme();
      den = direction_.norme();

      return ( num / den );
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vecteur3 Droite3D::perpendiculaireDroite(const Vecteur3& point)
   ///
   /// On trace la perpendicaulaire entre le point et le droite et on trouve
   /// le point d'intersection.
   ///
   /// @param[in] point : Un point dans l'espace d'où est calculée la
   ///                    perpendiculaire.
   ///
   /// @return Le point de rencontre entre la droite et la perpendiculaire.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur3 Droite3D::perpendiculaireDroite( const Vecteur3& point )
   {
      const Vecteur3& a   = direction_;
      const Vecteur3& p1  = pointDroite_;
      const Vecteur3  orig( 0.0, 0.0, 0.0 );
      const Vecteur3  op1( orig, p1 );

      const float op1x = p1[0];
      const float op1y = p1[1];
      const float op1z = p1[2];

      const float op0x = point[0];
      const float op0y = point[1];
      const float op0z = point[2];

      const float ax   = a[0];
      const float ay   = a[1];
      const float az   = a[2];

      const float num = ax * ( op1x - op0x ) +
                         ay * ( op1y - op0y ) +
                         az * ( op1z - op0z );
      const float den = -1.0f * ( pow ( ax, 2 ) +
                                  pow ( ay, 2 ) +
                                  pow ( az, 2 ) );

      const float r = num / den;

      const Vecteur3 oq = op1 + a * r;

      return oq;
   }


} // Fin de l'espace de nom math.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
