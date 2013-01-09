//////////////////////////////////////////////////////////////////////////////
/// @file Projection.h
/// @author DGI
/// @date 2006-12-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_PROJECTION_H__
#define __UTILITAIRE_PROJECTION_H__


#include "Vecteur.h"


namespace vue {


   ////////////////////////////////////////////////////////////////////////
   /// @class Projection
   /// @brief Classe contrôlant la projection du monde à la fenêtre.
   ///
   /// Cette classe offre certaines fonctionnalités commune à chacun des types
   /// de projection (isométrique, perspective, etc.).  Elle définit une
   /// interface que doivent implanter les types concrets de projection,
   /// c'est-à-dire permettre de modifier le facteur de zoom par exemple.
   ///
   /// @author Martin Bisson
   /// @date 2006-12-15
   ////////////////////////////////////////////////////////////////////////
   class Projection
   {
   public:
      /// Constructeur.
      Projection(int xMinCloture, int xMaxCloture,
                 int yMinCloture, int yMaxCloture,
                 double zAvant,  double zArriere,
                 double zoomInMax, double zoomOutMax,
                 double incrementZoom, bool estPerspective);

      /// Destructeur virtuel vide.
      virtual ~Projection() {}

      /// Zoom in, c'est-à-dire un agrandissement.
      virtual void zoomerIn( double increment ) = 0;
      /// Zoom out, c'est-à-dire un rapetissement.
      virtual void zoomerOut( double increment ) = 0;

	  /// Modification de la clôture.
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax ) = 0;
      /// Application de la projection.
      virtual void appliquer() const = 0;


      /// Application de la fenêtre de clôture (appel à glViewport).
      void mettreAJourCloture() const;
	  /// Application de la fenetre de cloture avec mise a jour des coord clotures
	  void mettreAJourCloture(int quelViewPort, int nbTotalViewport = 2);

      /// Application de la projection.
      void mettreAJourProjection() const;

      /// Obtention des dimensions de la fenêtre de clotûre.
      inline Vecteur2i obtenirDimensionCloture() const;
      /// Obtention des coordonnées de la fenêtre de clôture.
      inline void obtenirCoordonneesCloture(
         int& xMin, int& xMax, int& yMin, int& yMax
         ) const;
	  virtual Vecteur2 obtenirDimensionFenetre() const {return Vecteur2(1, 1);} // Pour s'assurer de ne pas obtenir de division par 0

      /// Vérification de si la projection est perspective.
      inline bool estPerspective() const;
	  /// Méthode tests pour visualiser l'état de la fenêtre virtuelle
	  virtual void afficherBordureVirtuelles() const = 0;
	  virtual Vecteur2 convertirDistanceClotureAVirtuelle(const Vecteur2i& v ) const = 0;

	  /// Centrer la fenêtre virtuelle sur un point
      virtual void centrerSurPoint( const Vecteur2i& pointCentre ) = 0;

	  /// Centrer la fenêtre à 0,0
	  virtual void centrerAZero() = 0;

	  virtual void ajusterRapportAspect( bool diminuerLongueur = true ) {mettreAJourProjection();}
	  virtual void ajusterRapportAspect( Vecteur2i dimentionCloture, Vecteur2 dimentionFenetre, bool diminuerLongueur = true ){mettreAJourProjection();}

   protected:
      /// Coordonnée inférieur de la clôture en X.
      int xMinCloture_;
      /// Coordonnée supérieur de la clôture en X.
      int xMaxCloture_;
      /// Coordonnée inférieur de la clôture en Y.
      int yMinCloture_;
      /// Coordonnée supérieur de la clôture en Y.
      int yMaxCloture_;

      /// Avant du volume de visualisation.
      double zAvant_;
      /// Arrière du volume de visualisation.
      double zArriere_;

      /// Facteur maximal de zoom in.
      const double zoomInMax_;
      /// Facteur maximal de zoom out.
      const double zoomOutMax_;
      /// Incrément des zooms.
      const double incrementZoom_;
      /// Vrai si la projection est perspective.
      const bool   estPerspective_;

   };




   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline Vecteur2i Projection::obtenirDimensionCloture() const
   ///
   /// Cette fonction retourne les dimensions de la fenêtre de clôture
   /// associée à cette projection.
   ///
   /// @return Les dimensions de la fenêtre de clôture.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline Vecteur2i Projection::obtenirDimensionCloture() const
   {
      return Vecteur2i(xMaxCloture_ - xMinCloture_, yMaxCloture_ - yMinCloture_);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void Projection::obtenirCoordonneesCloture(int& xMin, int& xMax, int& yMin, int& yMax) const
   ///
   /// Cette fonction retourne les coordonnées de la fenêtre de clôture
   /// associée à cette projection.
   ///
   /// @param[out]  xMin : La variable qui contiendra l'abcsisse minimale.
   /// @param[out]  xMax : La variable qui contiendra l'abcsisse maximale.
   /// @param[out]  yMin : La variable qui contiendra l'ordonnée minimale.
   /// @param[out]  yMax : La variable qui contiendra l'ordonnée maximale.
   ///
   /// @return Les coordonnées de la fenêtre de clôture.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void Projection::obtenirCoordonneesCloture(
      int& xMin, int& xMax, int& yMin, int& yMax
      ) const
   {
      xMin = xMinCloture_;
      xMax = xMaxCloture_;
      yMin = yMinCloture_;
      yMax = yMaxCloture_;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline bool Projection::estPerspective() const
   ///
   /// Cette fonction retourne vrai si la projection est une projection
   /// perspective.
   ///
   /// @return Vrai si la projection est une projection perspective.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline bool Projection::estPerspective() const
   {
      return estPerspective_;
   }


}; // Fin de l'espace de nom vue.


#endif // __UTILITAIRE_PROJECTION_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
