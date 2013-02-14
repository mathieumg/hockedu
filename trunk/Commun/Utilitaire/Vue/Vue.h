//////////////////////////////////////////////////////////////////////////////
/// @file Vue.h
/// @author DGI
/// @date 2006-12-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_VUE_H__
#define __UTILITAIRE_VUE_H__


#include "Camera.h"
#include "Vecteur.h"
#include "AnimationFrame.h"
#include "Animation.h"
#include "GestionnaireAnimations.h"



namespace math {
   class Plan3D;
}


namespace vue {

	
   class Projection;


   ////////////////////////////////////////////////////////////////////////
   /// @class Vue
   /// @brief Classe présentant l'interface commune à toutes les vues.
   ///
   /// Cette classe définit l'interface que devront implanter chacune des
   /// vues, c'est-à-dire comme réagir à certains événements pour modifier
   /// la vue.  Chaque vue concrète sera généralement composée d'une caméra
   /// (classe Camera) et d'une projection (classe Projection).
   ///
   /// Elle offre également certaines fonctionnalités communes à toutes les
   /// vues, comme la possibilité de trouver à quel point correspond, en
   /// coordonnées virtuelles, un point donné en coordonnées de clôture.
   ///
   /// @author Martin Bisson
   /// @date 2006-12-16
   ////////////////////////////////////////////////////////////////////////
   class Vue
   {
   public:
      /// Constructeur à partir d'une caméra
      Vue(const Camera& camera, const Camera& camera2 = Camera(), int nbViewports = 1);
      /// Destructeur virtuel vide.
      virtual ~Vue() {}

      /// Conversion de coordonnées de clôture à coordonnées virtuelles
      bool convertirClotureAVirtuelle(int x, int y, Vecteur3& point) const;
      /// Conversion de coordonnées de clôture à coordonnées virtuelles sur un plan donné
      bool convertirClotureAVirtuelle(int x, int y, const math::Plan3D& plan, Vecteur3& point) const;
	  /// Convertir une distance au format cloture au format virtuel
      virtual Vecteur2 convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2 ) const =0;
	  
	  /// Obtient la caméra associée à cette vue.
      inline Camera& obtenirCamera(int laquelle = 1);
      /// Obtient la caméra associée à cette vue (version constante).
      inline const Camera& obtenirCamera(int laquelle = 1) const;

      // Obtention de la projection
      virtual const Projection& obtenirProjection() const = 0;
      virtual Projection& obtenirProjection() = 0;
      /// Application de la projection
      virtual void appliquerProjection() const = 0;
      /// Application de la caméra
      virtual void appliquerCamera(int laquelle = 1) const = 0;

	  virtual void appliquerViewport(int lequel) {}

      /// Modification de la clotûre
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax ) = 0;

      /// Zoom in, c'est-à-dire un agrandissement.
      virtual void zoomerIn() = 0;
      /// Zoom out, c'est-à-dire un rapetissement.
      virtual void zoomerOut() = 0;
	  /// Zoom in, c'est-à-dire un agrandissement.
	  virtual void zoomerInSmooth() = 0;
	  /// Zoom out, c'est-à-dire un rapetissement.
	  virtual void zoomerOutSmooth() = 0;
      /// Zoom in élastique
      virtual void zoomerInElastique(const Vecteur2i& coin1,
                                     const Vecteur2i& coin2) = 0;
      /// Zoom out élastique
      virtual void zoomerOutElastique(const Vecteur2i& coin1,
                                      const Vecteur2i& coin2) = 0;

      /// Déplacement dans le plan XY par rapport à la vue
      virtual void deplacerXYSouris(float deplacementX, float deplacementY) = 0;
      /// Déplacement dans le plan XY par rapport à la vue
      virtual void deplacerXYSouris(const Vecteur2i& deplacement) = 0;
	  /// Déplacement dans le plan XY par rapport à la vue
	  virtual void deplacerXYFleches(float deplacementX, float deplacementY) = 0;
	  /// Déplacement dans le plan XY par rapport à la vue
	  virtual void deplacerXYFleches(const Vecteur2i& deplacement) = 0;
      /// Déplacement selon l'axe des Z par rapport à la vue
      virtual void deplacerZ(float deplacement) = 0;
      /// Rotation selon les axes des X et des Y par rapport à la vue
      virtual void rotaterXY(float rotationX, float rotationY) = 0;
      /// Rotation selon les axes des X et des Y par rapport à la vue
      virtual void rotaterXY(const Vecteur2i& rotation) = 0;
      /// Rotation selon l'axe des Z par rapport à la vue
      virtual void rotaterZ(float rotation) = 0;

      /// Animation de la vue en fonction du temps
      virtual void animer(float temps);
	  /// Centre la camera a l'origine pour voir la largeur du parametre
	  virtual void centrerCamera(float largeurVue);

	  virtual float obtenirAngleOuvertureProjection() {return 0.0;}
	  /// Retourne le nombre de viewports utilises par la vue
	  int obtenirNbViewports() const { return nbViewports_; }
	  

   protected:
      /// Caméra utilisée pour cette vue
      Camera camera_[2];
	  /// Nombre de viewports
	  int nbViewports_;
	  
   };


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline Camera& Vue::obtenirCamera()
   ///
   /// Cette fonction retourne la caméra associée à cette vue.
   ///
   /// @return La caméra associée à cet objet.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline Camera& Vue::obtenirCamera(int laquelle/* = 1*/)
   {
	   if(laquelle<=nbViewports_)
		   return camera_[laquelle-1];
	   else
		   return camera_[0];
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Camera& Vue::obtenirCamera() const
   ///
   /// Cette fonction retourne la caméra associée à cette vue (version
   /// constante).
   ///
   /// @return La caméra associée à cet objet.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline const Camera& Vue::obtenirCamera(int laquelle/* = 1*/) const
   {
	   if(laquelle<=nbViewports_)
		   return camera_[laquelle-1];
	   else
		   return camera_[0];
   }


}; // Fin de l'espace de nom vue.


#endif // __UTILITAIRE_VUE_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
