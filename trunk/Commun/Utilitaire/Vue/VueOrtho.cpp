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
   /// Constructeur d'une vue orthogonale.  Ne fait que cr�er les objets
   /// Projection et Camera correspondant.
   ///
   /// @param[in] camera        : Cam�ra � utiliser au d�part pour cette vue.
   /// @param[in] xMinCloture   : coordonn�e minimale en @a x de la cl�ture.
   /// @param[in] xMaxCloture   : coordonn�e maximale en @a x de la cl�ture.
   /// @param[in] yMinCloture   : coordonn�e minimale en @a y de la cl�ture.
   /// @param[in] yMaxCloture   : coordonn�e maximale en @a y de la cl�ture.
   /// @param[in] zAvant        : distance du plan avant (en @a z).
   /// @param[in] zArriere      : distance du plan arri�re (en @a z).
   /// @param[in] zoomInMax     : facteur de zoom in maximal.
   /// @param[in] zoomOutMax    : facteur de zoom out maximal.
   /// @param[in] incrementZoom : distance du plan arri�re (en @a z).
   /// @param[in] xMinFenetre   : coordonn�e minimale en @a x de la fen�tre
   ///                            virtuelle.
   /// @param[in] xMaxFenetre   : coordonn�e maximale en @a x de la fen�tre
   ///                            virtuelle.
   /// @param[in] yMinFenetre   : coordonn�e minimale en @a y de la fen�tre
   ///                            virtuelle.
   /// @param[in] yMaxFenetre   : coordonn�e maximale en @a y de la fen�tre
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
   /// Retourne la projection orthogonale associ�e � cette vue.
   ///
   /// @return La projection orthogonale associ�e � cette vue.
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
   /// Applique la matrice de projection correspondant � cette vue.
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
   /// Applique la matrice correspondant � cette vue selon la position de
   /// la cam�ra (eventuellement un gluLookAt()).
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
   /// Permet d'ajuster les coordonn�es de la fen�tre virtuelle en fonction
   /// d'un redimensionnement de la fen�tre.
   ///
   /// @param[in]  coinMin : Coin contenant les coordonn�es minimales de la
   ///                       nouvelle cl�ture.
   /// @param[in]  coinMax : Coin contenant les coordonn�es maximales de la
   ///                       nouvelle cl�ture.
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
   /// Permet de faire un zoom in selon l'incr�ment de zoom.
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
   /// Permet de faire un zoom in smooth selon l'incr�ment de zoom.
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
   /// Permet de faire un zoom out selon l'incr�ment de zoom.
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
   /// Permet de faire un zoom out smooth selon l'incr�ment de zoom.
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
   /// Permet de faire un zoom in �lastique.
   /// 
   /// @param[in]  coin1 : Coin contenant les coordonn�es du premier coin du
   ///                     rectangle.
   /// @param[in]  coin2 : Coin contenant les coordonn�es du second coin du
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
   /// Permet de faire un zoom out �lastique.
   /// 
   /// @param[in]  coin1 : Coin contenant les coordonn�es du premier coin du
   ///                     rectangle.
   /// @param[in]  coin2 : Coin contenant les coordonn�es du second coin du
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
   /// @param[in]  deplacementX : D�placement en pourcentage de la largeur.
   /// @param[in]  deplacementY : D�placement en pourcentage de la hauteur.
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
   /// Permet de faire un "pan" �quivalent � la distance du d�placement
   /// sp�cifi� en coordonn�es de cl�ture.
   ///
   /// @param[in]  deplacement : D�placement en coordonn�es de cl�ture
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
   /// @param[in]  deplacementX : D�placement en pourcentage de la largeur.
   /// @param[in]  deplacementY : D�placement en pourcentage de la hauteur.
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
   /// Permet de faire un "pan" �quivalent � la distance du d�placement
   /// sp�cifi� en coordonn�es de cl�ture.
   ///
   /// @param[in]  deplacement : D�placement en coordonn�es de cl�ture
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
   /// Ne fait rien, car se d�placer dans l'axe de la profondeur n'a pas
   /// vraiment de signification avec une vue orthogonale.
   ///
   /// @param[in]  deplacement : D�placement selon l'axe Z.
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
   /// Permet de faire une rotation de la cam�ra autour du point vers
   /// lequel elle regarde en modifiant l'angle de rotation et l'angle
   /// d'�l�vation.
   ///
   /// Une rotation de 100% correspondant � 360 degr�s en X (angle de
   /// rotation) ou 180 degr�s en Y (angle d'�l�vation).
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
   /// Permet de faire une rotation de la cam�ra autour du point vers
   /// lequel elle regarde en modifiant l'angle de rotation et l'angle
   /// d'�l�vation.
   ///
   /// Un d�placement de la taille de la fen�tre correspond � 100%.
   ///
   /// @param[in]  rotation : Rotation en coordonn�es de clot�re (pixels).
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
   /// correspondrait � un rouli et n'est pas souhaitable pour cette vue.
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
   /// Conversion des coordonn�es virtuelles.
   ///
   /// @param[in]  const Vecteur2i& v : Le premier vecteur.
   /// @param[in]  const Vecteur2i& v2 : Le deuxi�me vecteur.
   ///
   /// @return Le vectueur r�sultant.
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
   /// Conversions des coordonn�es de clot�re.
   ///
   /// @param[in]  const Vecteur2i& v : Le vecteur � convertir.
   ///
   /// @return Le vecteur r�sultant.
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
