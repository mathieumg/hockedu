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
   /// Cette classe implante l'interface de projection d�finie par la
   /// classe de base Projection et ajoute certaines fonctionnalit�es
   /// sp�cifiques � la projection orthogonale, comme le zoom autour d'un
   /// point en particulier et le zoom �lastique.
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


      /// Zoom in, c'est-�-dire un agrandissement.
      virtual void zoomerIn( float increment );
      /// Zoom out, c'est-�-dire un rapetissement.
      virtual void zoomerOut( float increment );
      /// Modification de la cl�ture.
      virtual void redimensionnerFenetre( const Vecteur2i& coinMin,
                                          const Vecteur2i& coinMax );
      /// Application de la projection.
      virtual void appliquer() const;


      /// Zoom out �lastique, sur un rectangle.
      void zoomerOut( const Vecteur2i& coin1, const Vecteur2i& coin2);
      /// Zoom in �latique, sur un rectangle.
      void zoomerIn( const Vecteur2i& coin1, const Vecteur2i& coin2);
	  /// Translater la fen�tre virtuelle d'un pourcentage en @a X ou en @a Y
      void translater( float deplacementX, float deplacementY );
      /// Translater la fen�tre virtuelle d'un vecteur
      void translater( const Vecteur2i& deplacement );
      /// Centrer la fen�tre virtuelle sur un point
      virtual void centrerSurPoint( const Vecteur2i& pointCentre );

      /// Obtenir les coordonn�es de la fen�tre virtuelle.
      inline void obtenirCoordonneesFenetreVirtuelle(
         float& xMin, float& xMax, float& yMin, float& yMax
         ) const;
	  /// Obtention des dimensions de la fen�tre de clot�re.
	  virtual inline Vecteur2 obtenirDimensionFenetre() const;

	  /// M�thode tests pour visualiser l'�tat de la fen�tre virtuelle
	  void afficherBordureVirtuelles() const;
	  /// Convertir des distances entre la fen�tre de cl�ture et la fen�tre
	  /// virtuelle.
	  Vecteur2 convertirDistanceClotureAVirtuelle(const Vecteur2i& v ) const;

	  /// Centrer la fen�tre � 0,0
	  virtual void centrerAZero();

	  /// Accesseur de l'angle d'ouverture
	  float obtenirAngleOuverture() const { return angleOuverture_; }
	  void modifierAngleOuverture(float val) { angleOuverture_ = val; }
	  
	  /// Ajuste la fen�tre virtuelle pour respecter le rapport d'aspect.
	  virtual void ajusterRapportAspect( bool diminuerLongueur = true );
	  virtual void ajusterRapportAspect( Vecteur2i dimentionCloture, Vecteur2 dimentionFenetre, bool diminuerLongueur = true );

   private:
      
      /// Convertir des coordonn�es entre la fen�tre de cl�ture et la fen�tre
      /// virtuelle.
      void convertirClotureAVirtuelle(int i, int j, float& x, float& y) const;
	  /// Convertir des coordonn�es entre la fen�tre de cl�ture et la fen�tre
	  /// virtuelle.
	  Vecteur2 convertirClotureAVirtuelle(const Vecteur2i& v ) const;
	  /// Borne sup�rieure en X de la fen�tre virtuelle.
      float xMaxFenetre_;
      /// Borne inf�rieure en Y de la fen�tre virtuelle.
      float yMinFenetre_;
      /// Borne sup�rieure en Y de la fen�tre virtuelle.
      float yMaxFenetre_;
	  /// Borne inf�rieure en X de la fen�tre virtuelle.
	  float xMinFenetre_;
	  /// Angle d'ouverture de la camera en degres
	  float angleOuverture_;
	  
   };




   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void ProjectionPerspective::obtenirCoordonneesFenetreVirtuelle(float& xMin, float& xMax, float& yMin, float& yMax) const
   ///
   /// Cette fonction retourne les coordonn�es de la fen�tre virtuelle
   /// associ�e � cette projection.
   ///
   /// @param[out]  xMin : La variable qui contiendra l'abcsisse minimale.
   /// @param[out]  xMax : La variable qui contiendra l'abcsisse maximale.
   /// @param[out]  yMin : La variable qui contiendra l'ordonn�e minimale.
   /// @param[out]  yMax : La variable qui contiendra l'ordonn�e maximale.
   ///
   /// @return Les coordonn�es de la fen�tre virtuelle.
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
   /// Cette fonction retourne les dimensions de la fen�tre virtuelle
   /// associ�e � cette projection.
   ///
   /// @return Les dimensions de la fen�tre virtuelle.
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
