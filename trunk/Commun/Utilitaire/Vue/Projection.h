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
   /// @brief Classe contr�lant la projection du monde � la fen�tre.
   ///
   /// Cette classe offre certaines fonctionnalit�s commune � chacun des types
   /// de projection (isom�trique, perspective, etc.).  Elle d�finit une
   /// interface que doivent implanter les types concrets de projection,
   /// c'est-�-dire permettre de modifier le facteur de zoom par exemple.
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

      /// Zoom in, c'est-�-dire un agrandissement.
      virtual void zoomerIn( double increment ) = 0;
      /// Zoom out, c'est-�-dire un rapetissement.
      virtual void zoomerOut( double increment ) = 0;

	  /// Modification de la cl�ture.
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax ) = 0;
      /// Application de la projection.
      virtual void appliquer() const = 0;


      /// Application de la fen�tre de cl�ture (appel � glViewport).
      void mettreAJourCloture() const;
	  /// Application de la fenetre de cloture avec mise a jour des coord clotures
	  void mettreAJourCloture(int quelViewPort, int nbTotalViewport = 2);

      /// Application de la projection.
      void mettreAJourProjection() const;

      /// Obtention des dimensions de la fen�tre de clot�re.
      inline Vecteur2i obtenirDimensionCloture() const;
      /// Obtention des coordonn�es de la fen�tre de cl�ture.
      inline void obtenirCoordonneesCloture(
         int& xMin, int& xMax, int& yMin, int& yMax
         ) const;
	  virtual Vecteur2 obtenirDimensionFenetre() const {return Vecteur2(1, 1);} // Pour s'assurer de ne pas obtenir de division par 0

      /// V�rification de si la projection est perspective.
      inline bool estPerspective() const;
	  /// M�thode tests pour visualiser l'�tat de la fen�tre virtuelle
	  virtual void afficherBordureVirtuelles() const = 0;
	  virtual Vecteur2 convertirDistanceClotureAVirtuelle(const Vecteur2i& v ) const = 0;

	  /// Centrer la fen�tre virtuelle sur un point
      virtual void centrerSurPoint( const Vecteur2i& pointCentre ) = 0;

	  /// Centrer la fen�tre � 0,0
	  virtual void centrerAZero() = 0;

	  virtual void ajusterRapportAspect( bool diminuerLongueur = true ) {mettreAJourProjection();}
	  virtual void ajusterRapportAspect( Vecteur2i dimentionCloture, Vecteur2 dimentionFenetre, bool diminuerLongueur = true ){mettreAJourProjection();}

   protected:
      /// Coordonn�e inf�rieur de la cl�ture en X.
      int xMinCloture_;
      /// Coordonn�e sup�rieur de la cl�ture en X.
      int xMaxCloture_;
      /// Coordonn�e inf�rieur de la cl�ture en Y.
      int yMinCloture_;
      /// Coordonn�e sup�rieur de la cl�ture en Y.
      int yMaxCloture_;

      /// Avant du volume de visualisation.
      double zAvant_;
      /// Arri�re du volume de visualisation.
      double zArriere_;

      /// Facteur maximal de zoom in.
      const double zoomInMax_;
      /// Facteur maximal de zoom out.
      const double zoomOutMax_;
      /// Incr�ment des zooms.
      const double incrementZoom_;
      /// Vrai si la projection est perspective.
      const bool   estPerspective_;

   };




   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline Vecteur2i Projection::obtenirDimensionCloture() const
   ///
   /// Cette fonction retourne les dimensions de la fen�tre de cl�ture
   /// associ�e � cette projection.
   ///
   /// @return Les dimensions de la fen�tre de cl�ture.
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
   /// Cette fonction retourne les coordonn�es de la fen�tre de cl�ture
   /// associ�e � cette projection.
   ///
   /// @param[out]  xMin : La variable qui contiendra l'abcsisse minimale.
   /// @param[out]  xMax : La variable qui contiendra l'abcsisse maximale.
   /// @param[out]  yMin : La variable qui contiendra l'ordonn�e minimale.
   /// @param[out]  yMax : La variable qui contiendra l'ordonn�e maximale.
   ///
   /// @return Les coordonn�es de la fen�tre de cl�ture.
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
