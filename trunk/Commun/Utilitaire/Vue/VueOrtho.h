//////////////////////////////////////////////////////////////////////////////
/// @file VueOrtho.h
/// @author DGI
/// @date 2006-12-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_VUEORTHO_H__
#define __UTILITAIRE_VUEORTHO_H__


#include "Vue.h"
#include "Camera.h"
#include "ProjectionOrtho.h"


namespace vue {


   ////////////////////////////////////////////////////////////////////////
   /// @class VueOrtho
   /// @brief Classe concrète de vue orthogonale.
   ///
   /// Cette classe implante le comportement attendu d'une vue orthogonale.
   ///
   /// @author Martin Bisson
   /// @date 2006-12-16
   ////////////////////////////////////////////////////////////////////////
   class VueOrtho : public Vue
   {
   public:
      // Constructeur.
      VueOrtho(const Camera& camera,
               int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture,
               float zAvant,  float zArriere,
               float zoomInMax, float zoomOutMax,
               float incrementZoom,
               float xMinFenetre, float xMaxFenetre,
               float yMinFenetre, float yMaxFenetre);

      /// Obtention de la projection.
      virtual const ProjectionOrtho& obtenirProjection() const;
      virtual ProjectionOrtho& obtenirProjection();
      /// Application de la projection.
      virtual void appliquerProjection() const;
      /// Application de la caméra.
      virtual void appliquerCamera(int laquelle = 1) const;

      /// Modification de la clotûre.
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax );

      /// Zoom in, c'est-à-dire un agrandissement.
      virtual void zoomerIn();
      /// Zoom out, c'est-à-dire un rapetissement.
      virtual void zoomerOut();
	  /// Zoom in, c'est-à-dire un agrandissement.
	  virtual void zoomerInSmooth();
	  /// Zoom out, c'est-à-dire un rapetissement.
	  virtual void zoomerOutSmooth();
      /// Zoom in élastique.
      virtual void zoomerInElastique(const Vecteur2i& coin1,
                                     const Vecteur2i& coin2);
      /// Zoom out élastique.
      virtual void zoomerOutElastique(const Vecteur2i& coin1,
                                      const Vecteur2i& coin2);

      /// Déplacement dans le plan XY par rapport à la vue.
      virtual void deplacerXYSouris(float deplacementX, float deplacementY);
      /// Déplacement dans le plan XY par rapport à la vue.
      virtual void deplacerXYSouris(const Vecteur2i& deplacement);
	  /// Déplacement dans le plan XY par rapport à la vue.
	  virtual void deplacerXYFleches(float deplacementX, float deplacementY);
	  /// Déplacement dans le plan XY par rapport à la vue.
	  virtual void deplacerXYFleches(const Vecteur2i& deplacement);
      /// Déplacement selon l'axe des Z par rapport à la vue.
      virtual void deplacerZ(float deplacement);
      /// Rotation selon les axes des X et des Y par rapport à la vue.
      virtual void rotaterXY(float rotationX, float rotationY);
      /// Rotation selon les axes des X et des Y par rapport à la vue.
      virtual void rotaterXY(const Vecteur2i& rotation);
      /// Rotation selon l'axe des Z par rapport à la vue.
      virtual void rotaterZ(float rotation);

	  virtual Vecteur2 convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const;

	  virtual Vecteur3 convertirClotureAVirtuelle( const Vecteur2i& v ) const;
   private:
      /// Projection utilisée pour cette vue.
      ProjectionOrtho projection_;

   };


}; // Fin de l'espace de nom vue.


#endif // __UTILITAIRE_VUEORTHO_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
