////////////////////////////////////////////////////////////////////////////////////
/// @file VueOrtho.cpp
/// @author DGI
/// @date 2006-12-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "Utilitaire.h"
#include "VueOrtho.h"


namespace vue {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn VueOrtho::VueOrtho(const Camera& camera, int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture, float zAvant, float zArriere, float zoomInMax, float zoomOutMax, float incrementZoom, float xMinFenetre, float xMaxFenetre, float yMinFenetre, float yMaxFenetre)
   ///
   /// Constructeur d'une vue orthogonale.  Ne fait que créer les objets
   /// Projection et Camera correspondant.
   ///
   /// @param[in] camera        : Caméra à utiliser au départ pour cette vue.
   /// @param[in] xMinCloture   : coordonnée minimale en @a x de la clôture.
   /// @param[in] xMaxCloture   : coordonnée maximale en @a x de la clôture.
   /// @param[in] yMinCloture   : coordonnée minimale en @a y de la clôture.
   /// @param[in] yMaxCloture   : coordonnée maximale en @a y de la clôture.
   /// @param[in] zAvant        : distance du plan avant (en @a z).
   /// @param[in] zArriere      : distance du plan arrière (en @a z).
   /// @param[in] zoomInMax     : facteur de zoom in maximal.
   /// @param[in] zoomOutMax    : facteur de zoom out maximal.
   /// @param[in] incrementZoom : distance du plan arrière (en @a z).
   /// @param[in] xMinFenetre   : coordonnée minimale en @a x de la fenêtre
   ///                            virtuelle.
   /// @param[in] xMaxFenetre   : coordonnée maximale en @a x de la fenêtre
   ///                            virtuelle.
   /// @param[in] yMinFenetre   : coordonnée minimale en @a y de la fenêtre
   ///                            virtuelle.
   /// @param[in] yMaxFenetre   : coordonnée maximale en @a y de la fenêtre
   ///                            virtuelle.
   /// 
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   VueOrtho::VueOrtho(const Camera& camera,
                      int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture,
                      float zAvant,  float zArriere,
                      float zoomInMax, float zoomOutMax,
                      float incrementZoom,
                      float xMinFenetre, float xMaxFenetre,
                      float yMinFenetre, float yMaxFenetre) :
      Vue(camera),
      projection_(xMinCloture, xMaxCloture, yMinCloture, yMaxCloture,
                  zAvant, zArriere, zoomInMax, zoomOutMax,
                  incrementZoom,
                  xMinFenetre, xMaxFenetre, yMinFenetre, yMaxFenetre)
   {
	   appliquerProjection();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn const ProjectionOrtho& VueOrtho::obtenirProjection() const
   ///
   /// Retourne la projection orthogonale associée à cette vue.
   ///
   /// @return La projection orthogonale associée à cette vue.
   ///
   ////////////////////////////////////////////////////////////////////////
   const ProjectionOrtho& VueOrtho::obtenirProjection() const
   {
      return projection_;
   }

   ProjectionOrtho& VueOrtho::obtenirProjection()
   {
      return projection_;
   }



   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::appliquerProjection() const
   ///
   /// Applique la matrice de projection correspondant à cette vue.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::appliquerProjection() const
   {
      projection_.mettreAJourProjection();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::appliquerCamera() const
   ///
   /// Applique la matrice correspondant à cette vue selon la position de
   /// la caméra (eventuellement un gluLookAt()).
   /// 
   /// @param[in]	laquelle :	quelle camera
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::appliquerCamera(int laquelle/* = 1*/) const
   {
      obtenirCamera(laquelle).positionner();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::redimensionnerFenetre( const Vecteur2i& coinMin, const Vecteur2i& coinMax )
   ///
   /// Permet d'ajuster les coordonnées de la fenêtre virtuelle en fonction
   /// d'un redimensionnement de la fenêtre.
   ///
   /// @param[in]  coinMin : Coin contenant les coordonnées minimales de la
   ///                       nouvelle clôture.
   /// @param[in]  coinMax : Coin contenant les coordonnées maximales de la
   ///                       nouvelle clôture.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::redimensionnerFenetre( const Vecteur2i& coinMin,
                                         const Vecteur2i& coinMax )
   {
      projection_.redimensionnerFenetre(coinMin, coinMax);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::zoomerIn()
   ///
   /// Permet de faire un zoom in selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::zoomerIn()
   {
      projection_.zoomerIn(0.0);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::zoomerInSmooth()
   ///
   /// Permet de faire un zoom in smooth selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::zoomerInSmooth()
   {
	   projection_.zoomerIn(0.25);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::zoomerOut()
   ///
   /// Permet de faire un zoom out selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::zoomerOut()
   {
      projection_.zoomerOut(0.0);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::zoomerOutSmooth()
   ///
   /// Permet de faire un zoom out smooth selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::zoomerOutSmooth()
   {
	   projection_.zoomerOut(0.25);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::zoomerInElastique(const Vecteur2i& coin1, const Vecteur2i& coin2)
   ///
   /// Permet de faire un zoom in élastique.
   /// 
   /// @param[in]  coin1 : Coin contenant les coordonnées du premier coin du
   ///                     rectangle.
   /// @param[in]  coin2 : Coin contenant les coordonnées du second coin du
   ///                     rectangle.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::zoomerInElastique(const Vecteur2i& coin1,
                                    const Vecteur2i& coin2)
   {
      projection_.zoomerIn(coin1, coin2);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::zoomerOutElastique(const Vecteur2i& coin1, const Vecteur2i& coin2)
   ///
   /// Permet de faire un zoom out élastique.
   /// 
   /// @param[in]  coin1 : Coin contenant les coordonnées du premier coin du
   ///                     rectangle.
   /// @param[in]  coin2 : Coin contenant les coordonnées du second coin du
   ///                     rectangle.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::zoomerOutElastique(const Vecteur2i& coin1,
                                     const Vecteur2i& coin2)
   {
      projection_.zoomerOut(coin1, coin2);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::deplacerXYSouris(float deplacementX, float deplacementY)
   ///
   /// @param[in]  deplacementX : Déplacement en pourcentage de la largeur.
   /// @param[in]  deplacementY : Déplacement en pourcentage de la hauteur.
   ///
   /// Permet de faire un "pan" d'un certain pourcentage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::deplacerXYSouris(float deplacementX, float deplacementY)
   {
      projection_.translater(deplacementX, deplacementY);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::deplacerXYSouris(const Vecteur2i& deplacement)
   ///
   /// Permet de faire un "pan" équivalent à la distance du déplacement
   /// spécifié en coordonnées de clôture.
   ///
   /// @param[in]  deplacement : Déplacement en coordonnées de clôture
   ///                           (pixels).
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::deplacerXYSouris(const Vecteur2i& deplacement)
   {
      projection_.translater(deplacement);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::deplacerXYFleches(float deplacementX, float deplacementY)
   ///
   /// @param[in]  deplacementX : Déplacement en pourcentage de la largeur.
   /// @param[in]  deplacementY : Déplacement en pourcentage de la hauteur.
   ///
   /// Permet de faire un "pan" d'un certain pourcentage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::deplacerXYFleches(float deplacementX, float deplacementY)
   {
	   deplacerXYSouris(deplacementX, deplacementY);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::deplacerXYFleches(const Vecteur2i& deplacement)
   ///
   /// Permet de faire un "pan" équivalent à la distance du déplacement
   /// spécifié en coordonnées de clôture.
   ///
   /// @param[in]  deplacement : Déplacement en coordonnées de clôture
   ///                           (pixels).
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::deplacerXYFleches(const Vecteur2i& deplacement)
   {
	   deplacerXYSouris(deplacement);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::deplacerZ(float deplacement)
   ///
   /// Ne fait rien, car se déplacer dans l'axe de la profondeur n'a pas
   /// vraiment de signification avec une vue orthogonale.
   ///
   /// @param[in]  deplacement : Déplacement selon l'axe Z.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::deplacerZ(float deplacement)
   {
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::rotaterXY(float rotationX, float rotationY)
   ///
   /// Permet de faire une rotation de la caméra autour du point vers
   /// lequel elle regarde en modifiant l'angle de rotation et l'angle
   /// d'élévation.
   ///
   /// Une rotation de 100% correspondant à 360 degrés en X (angle de
   /// rotation) ou 180 degrés en Y (angle d'élévation).
   ///
   /// @param[in]  rotationX : Rotation en pourcentage de la largeur.
   /// @param[in]  rotationY : Rotation en pourcentage de la hauteur.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::rotaterXY(float rotationX, float rotationY)
   {
      //obtenirCamera().orbiterXY(rotationX * 360, rotationY * 180);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::rotaterXY(const Vecteur2i& rotation)
   ///
   /// Permet de faire une rotation de la caméra autour du point vers
   /// lequel elle regarde en modifiant l'angle de rotation et l'angle
   /// d'élévation.
   ///
   /// Un déplacement de la taille de la fenêtre correspond à 100%.
   ///
   /// @param[in]  rotation : Rotation en coordonnées de clotûre (pixels).
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::rotaterXY(const Vecteur2i& rotation)
   {
      /*const Vecteur2i dimensions = projection_.obtenirDimensionCloture();
      rotaterXY(rotation[0] / (float) dimensions[0],
                rotation[1] / (float) dimensions[1]);*/
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VueOrtho::rotaterZ(float rotation)
   ///
   /// Ne fait rien, car tourner autour de l'axe de la profondeur
   /// correspondrait à un rouli et n'est pas souhaitable pour cette vue.
   ///
   /// @param[in]  rotation : Rotation selon l'axe Z.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VueOrtho::rotaterZ(float rotation)
   {
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vecteur2 VueOrtho::convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const
   ///
   /// Conversion des coordonnées virtuelles.
   ///
   /// @param[in]  const Vecteur2i& v : Le premier vecteur.
   /// @param[in]  const Vecteur2i& v2 : Le deuxième vecteur.
   ///
   /// @return Le vectueur résultant.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur2 VueOrtho::convertirDeplacementClotureAVirtuelle( const Vecteur2i& v , const Vecteur2i& v2 ) const
   {
	   return projection_.convertirDistanceClotureAVirtuelle(v-v2);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vecteur3 VueOrtho::convertirClotureAVirtuelle( const Vecteur2i& v ) const
   ///
   /// Conversions des coordonnées de clotûre.
   ///
   /// @param[in]  const Vecteur2i& v : Le vecteur à convertir.
   ///
   /// @return Le vecteur résultant.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur3 VueOrtho::convertirClotureAVirtuelle( const Vecteur2i& v ) const
   {
	   Vecteur3 virt;
	   Vue::convertirClotureAVirtuelle(v[VX], v[VY], virt);
	   return virt;
   }


}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
