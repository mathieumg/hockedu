////////////////////////////////////////////////////////////////////////////////////
/// @file ProjectionPerspective.h
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_ProjectionPerspective_H__
#define __UTILITAIRE_ProjectionPerspective_H__


#include "Projection.h"
class GestionnaireEvenementsTest;

namespace vue {


   ////////////////////////////////////////////////////////////////////////
   /// @class ProjectionPerspective
   /// @brief Classe implantant une projection orthogonale.
   ///
   /// Cette classe implante l'interface de projection définie par la
   /// classe de base Projection et ajoute certaines fonctionnalitées
   /// spécifiques à la projection orthogonale, comme le zoom autour d'un
   /// point en particulier et le zoom élastique.
   ///
   /// @author Martin Bisson
   /// @date 2006-12-15
   ////////////////////////////////////////////////////////////////////////
   class ProjectionPerspective : public Projection
   {
   public:
	   friend GestionnaireEvenementsTest;

      /// Constructeur.
      ProjectionPerspective(int xMinCloture, int xMaxCloture,
                      int yMinCloture, int yMaxCloture,
                      float zAvant,  float zArriere,
                      float zoomInMax, float zoomOutMax,
                      float incrementZoom,
                      float xMinFenetre, float xMaxFenetre,
                      float yMinFenetre, float yMaxFenetre);


      /// Zoom in, c'est-à-dire un agrandissement.
      virtual void zoomerIn( float increment );
      /// Zoom out, c'est-à-dire un rapetissement.
      virtual void zoomerOut( float increment );
      /// Modification de la clôture.
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax );
      /// Application de la projection.
      virtual void appliquer() const;


      /// Zoom out élastique, sur un rectangle.
      void zoomerOut( const Vecteur2i& coin1, const Vecteur2i& coin2);
      /// Zoom in élatique, sur un rectangle.
      void zoomerIn( const Vecteur2i& coin1, const Vecteur2i& coin2);
	  /// Translater la fenêtre virtuelle d'un pourcentage en @a X ou en @a Y
      void translater( float deplacementX, float deplacementY );
      /// Translater la fenêtre virtuelle d'un vecteur
      void translater( const Vecteur2i& deplacement );
      /// Centrer la fenêtre virtuelle sur un point
      virtual void centrerSurPoint( const Vecteur2i& pointCentre );

      /// Obtenir les coordonnées de la fenêtre virtuelle.
      inline void obtenirCoordonneesFenetreVirtuelle(
         float& xMin, float& xMax, float& yMin, float& yMax
         ) const;
	  /// Obtention des dimensions de la fenêtre de clotûre.
	  virtual inline Vecteur2 obtenirDimensionFenetre() const;

	  /// Méthode tests pour visualiser l'état de la fenêtre virtuelle
	  void afficherBordureVirtuelles() const;
	  /// Convertir des distances entre la fenêtre de clôture et la fenêtre
	  /// virtuelle.
	  Vecteur2 convertirDistanceClotureAVirtuelle(const Vecteur2i& v ) const;

	  /// Centrer la fenêtre à 0,0
	  virtual void centrerAZero();

	  /// Accesseur de l'angle d'ouverture
	  float obtenirAngleOuverture() const { return angleOuverture_; }
	  void modifierAngleOuverture(float val) { angleOuverture_ = val; }
	  
	  /// Ajuste la fenêtre virtuelle pour respecter le rapport d'aspect.
	  virtual void ajusterRapportAspect( bool diminuerLongueur = true );
	  virtual void ajusterRapportAspect( Vecteur2i dimentionCloture, Vecteur2 dimentionFenetre, bool diminuerLongueur = true );

   private:
      
      /// Convertir des coordonnées entre la fenêtre de clôture et la fenêtre
      /// virtuelle.
      void convertirClotureAVirtuelle(int i, int j, float& x, float& y) const;
	  /// Convertir des coordonnées entre la fenêtre de clôture et la fenêtre
	  /// virtuelle.
	  Vecteur2 convertirClotureAVirtuelle(const Vecteur2i& v ) const;
	  /// Borne supérieure en X de la fenêtre virtuelle.
      float xMaxFenetre_;
      /// Borne inférieure en Y de la fenêtre virtuelle.
      float yMinFenetre_;
      /// Borne supérieure en Y de la fenêtre virtuelle.
      float yMaxFenetre_;
	  /// Borne inférieure en X de la fenêtre virtuelle.
	  float xMinFenetre_;
	  /// Angle d'ouverture de la camera en degres
	  float angleOuverture_;
	  
   };




   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void ProjectionPerspective::obtenirCoordonneesFenetreVirtuelle(float& xMin, float& xMax, float& yMin, float& yMax) const
   ///
   /// Cette fonction retourne les coordonnées de la fenêtre virtuelle
   /// associée à cette projection.
   ///
   /// @param[out]  xMin : La variable qui contiendra l'abcsisse minimale.
   /// @param[out]  xMax : La variable qui contiendra l'abcsisse maximale.
   /// @param[out]  yMin : La variable qui contiendra l'ordonnée minimale.
   /// @param[out]  yMax : La variable qui contiendra l'ordonnée maximale.
   ///
   /// @return Les coordonnées de la fenêtre virtuelle.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void ProjectionPerspective::obtenirCoordonneesFenetreVirtuelle(
      float& xMin, float& xMax, float& yMin, float& yMax
      ) const
   {
      xMin = xMinFenetre_;
      xMax = xMaxFenetre_;
      yMin = yMinFenetre_;
      yMax = yMaxFenetre_;
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline Vecteur2i ProjectionPerspective::obtenirDimensionFenetre() const
   ///
   /// Cette fonction retourne les dimensions de la fenêtre virtuelle
   /// associée à cette projection.
   ///
   /// @return Les dimensions de la fenêtre virtuelle.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline Vecteur2 ProjectionPerspective::obtenirDimensionFenetre() const
   {
	   return Vecteur2(xMaxFenetre_ - xMinFenetre_, yMaxFenetre_ - yMinFenetre_);
   }


}; // Fin de l'espace de nom vue.


#endif // __UTILITAIRE_ProjectionPerspective_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
