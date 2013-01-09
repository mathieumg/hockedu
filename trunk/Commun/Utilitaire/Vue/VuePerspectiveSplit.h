//////////////////////////////////////////////////////////////////////////////
/// @file VuePerspectiveSplit.h
/// @author Mathieu Parent
/// @date 2012-04-14
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_VuePerspectiveSplit_H__
#define __UTILITAIRE_VuePerspectiveSplit_H__


#include "Vue.h"
#include "Camera.h"
#include "ProjectionPerspective.h"


namespace vue {


   ////////////////////////////////////////////////////////////////////////
   /// @class VuePerspectiveSplit
   /// @brief Classe concr�te de vue perspective.
   ///
   /// Cette classe implante le comportement attendu d'une vue perspective divisee fixe a l'horizontal
   ///
   /// @author Mathieu Parent
   /// @date 2012-04-14
   ////////////////////////////////////////////////////////////////////////
   class VuePerspectiveSplit : public Vue
   {
   public:
      // Constructeur.
      VuePerspectiveSplit(const Camera& camera,
               int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture,
               double zAvant,  double zArriere,
               double zoomInMax, double zoomOutMax,
               double incrementZoom,
               double xMinFenetre, double xMaxFenetre,
               double yMinFenetre, double yMaxFenetre);

      /// Obtention de la projection.
      virtual const ProjectionPerspective& obtenirProjection() const;
      virtual ProjectionPerspective& obtenirProjection();
      /// Application de la projection.
      virtual void appliquerProjection() const;
      /// Application de la cam�ra.
      virtual void appliquerCamera(int laquelle = 1) const;

      /// Modification de la clot�re.
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax );

      /// Zoom in, c'est-�-dire un agrandissement.
      virtual void zoomerIn();
      /// Zoom out, c'est-�-dire un rapetissement.
      virtual void zoomerOut();
	  /// Zoom in, c'est-�-dire un agrandissement.
	  virtual void zoomerInSmooth();
	  /// Zoom out, c'est-�-dire un rapetissement.
	  virtual void zoomerOutSmooth();
      /// Zoom in �lastique.
      virtual void zoomerInElastique(const Vecteur2i& coin1,
                                     const Vecteur2i& coin2);
      /// Zoom out �lastique.
      virtual void zoomerOutElastique(const Vecteur2i& coin1,
                                      const Vecteur2i& coin2);

      /// D�placement dans le plan XY par rapport � la vue.
      virtual void deplacerXYSouris(double deplacementX, double deplacementY);
      /// D�placement dans le plan XY par rapport � la vue.
      virtual void deplacerXYSouris(const Vecteur2i& deplacement);
	  /// D�placement dans le plan XY par rapport � la vue.
	  virtual void deplacerXYFleches(double deplacementX, double deplacementY);
	  /// D�placement dans le plan XY par rapport � la vue.
	  virtual void deplacerXYFleches(const Vecteur2i& deplacement);
      /// D�placement selon l'axe des Z par rapport � la vue.
      virtual void deplacerZ(double deplacement);
      /// Rotation selon les axes des X et des Y par rapport � la vue.
      virtual void rotaterXY(double rotationX, double rotationY);
      /// Rotation selon les axes des X et des Y par rapport � la vue.
      virtual void rotaterXY(const Vecteur2i& rotation);
      /// Rotation selon l'axe des Z par rapport � la vue.
      virtual void rotaterZ(double rotation);

	  virtual Vecteur2 convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const;

	  virtual Vecteur3 convertirClotureAVirtuelle( const Vecteur2i& v ) const;

	  virtual double obtenirAngleOuvertureProjection();

	  virtual void appliquerViewport( int lequel );

   private:
      /// Projection utilis�e pour cette vue.
      ProjectionPerspective projection_;

   };


}; // Fin de l'espace de nom vue.


#endif // __UTILITAIRE_VuePerspectiveSplit_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
