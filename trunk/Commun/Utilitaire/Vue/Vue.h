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
   /// @brief Classe pr�sentant l'interface commune � toutes les vues.
   ///
   /// Cette classe d�finit l'interface que devront implanter chacune des
   /// vues, c'est-�-dire comme r�agir � certains �v�nements pour modifier
   /// la vue.  Chaque vue concr�te sera g�n�ralement compos�e d'une cam�ra
   /// (classe Camera) et d'une projection (classe Projection).
   ///
   /// Elle offre �galement certaines fonctionnalit�s communes � toutes les
   /// vues, comme la possibilit� de trouver � quel point correspond, en
   /// coordonn�es virtuelles, un point donn� en coordonn�es de cl�ture.
   ///
   /// @author Martin Bisson
   /// @date 2006-12-16
   ////////////////////////////////////////////////////////////////////////
   class Vue
   {
   public:
      /// Constructeur � partir d'une cam�ra
      Vue(const Camera& camera, const Camera& camera2 = Camera(), int nbViewports = 1);
      /// Destructeur virtuel vide.
      virtual ~Vue() {}

      /// Conversion de coordonn�es de cl�ture � coordonn�es virtuelles
      bool convertirClotureAVirtuelle(int x, int y, Vecteur3& point) const;
      /// Conversion de coordonn�es de cl�ture � coordonn�es virtuelles sur un plan donn�
      bool convertirClotureAVirtuelle(int x, int y, const math::Plan3D& plan, Vecteur3& point) const;
	  /// Convertir une distance au format cloture au format virtuel
      virtual Vecteur2 convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2 ) const =0;
	  
	  /// Obtient la cam�ra associ�e � cette vue.
      inline Camera& obtenirCamera(int laquelle = 1);
      /// Obtient la cam�ra associ�e � cette vue (version constante).
      inline const Camera& obtenirCamera(int laquelle = 1) const;

      // Obtention de la projection
      virtual const Projection& obtenirProjection() const = 0;
      virtual Projection& obtenirProjection() = 0;
      /// Application de la projection
      virtual void appliquerProjection() const = 0;
      /// Application de la cam�ra
      virtual void appliquerCamera(int laquelle = 1) const = 0;

	  virtual void appliquerViewport(int lequel) {}

      /// Modification de la clot�re
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax ) = 0;

      /// Zoom in, c'est-�-dire un agrandissement.
      virtual void zoomerIn() = 0;
      /// Zoom out, c'est-�-dire un rapetissement.
      virtual void zoomerOut() = 0;
	  /// Zoom in, c'est-�-dire un agrandissement.
	  virtual void zoomerInSmooth() = 0;
	  /// Zoom out, c'est-�-dire un rapetissement.
	  virtual void zoomerOutSmooth() = 0;
      /// Zoom in �lastique
      virtual void zoomerInElastique(const Vecteur2i& coin1,
                                     const Vecteur2i& coin2) = 0;
      /// Zoom out �lastique
      virtual void zoomerOutElastique(const Vecteur2i& coin1,
                                      const Vecteur2i& coin2) = 0;

      /// D�placement dans le plan XY par rapport � la vue
      virtual void deplacerXYSouris(float deplacementX, float deplacementY) = 0;
      /// D�placement dans le plan XY par rapport � la vue
      virtual void deplacerXYSouris(const Vecteur2i& deplacement) = 0;
	  /// D�placement dans le plan XY par rapport � la vue
	  virtual void deplacerXYFleches(float deplacementX, float deplacementY) = 0;
	  /// D�placement dans le plan XY par rapport � la vue
	  virtual void deplacerXYFleches(const Vecteur2i& deplacement) = 0;
      /// D�placement selon l'axe des Z par rapport � la vue
      virtual void deplacerZ(float deplacement) = 0;
      /// Rotation selon les axes des X et des Y par rapport � la vue
      virtual void rotaterXY(float rotationX, float rotationY) = 0;
      /// Rotation selon les axes des X et des Y par rapport � la vue
      virtual void rotaterXY(const Vecteur2i& rotation) = 0;
      /// Rotation selon l'axe des Z par rapport � la vue
      virtual void rotaterZ(float rotation) = 0;

      /// Animation de la vue en fonction du temps
      virtual void animer(float temps);
	  /// Centre la camera a l'origine pour voir la largeur du parametre
	  virtual void centrerCamera(float largeurVue);

	  virtual float obtenirAngleOuvertureProjection() {return 0.0;}
	  /// Retourne le nombre de viewports utilises par la vue
	  int obtenirNbViewports() const { return nbViewports_; }
	  

   protected:
      /// Cam�ra utilis�e pour cette vue
      Camera camera_[2];
	  /// Nombre de viewports
	  int nbViewports_;
	  
   };


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline Camera& Vue::obtenirCamera()
   ///
   /// Cette fonction retourne la cam�ra associ�e � cette vue.
   ///
   /// @return La cam�ra associ�e � cet objet.
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
   /// Cette fonction retourne la cam�ra associ�e � cette vue (version
   /// constante).
   ///
   /// @return La cam�ra associ�e � cet objet.
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
