////////////////////////////////////////////////////////////////////////////////////
/// @file VuePerspectiveCiel.cpp
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES
#include <math.h>
#include "Utilitaire.h"
#include "VuePerspectiveCiel.h"
#include "Camera.h"
#include "Vue.h"
#include <iostream>




namespace vue {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn VuePerspectiveCiel::VuePerspectiveCiel(const Camera& camera, int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture, double zAvant, double zArriere, double zoomInMax, double zoomOutMax, double incrementZoom, double xMinFenetre, double xMaxFenetre, double yMinFenetre, double yMaxFenetre)
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
   /// @param[in] fovY			: angle d'ouverture de la camera
   /// 
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   VuePerspectiveCiel::VuePerspectiveCiel(const Camera& camera,
                      int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture,
                      double zAvant,  double zArriere,
                      double zoomInMax, double zoomOutMax,
                      double incrementZoom,
                      double xMinFenetre, double xMaxFenetre,
                      double yMinFenetre, double yMaxFenetre) :
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
   /// @fn const ProjectionPerspective& VuePerspectiveCiel::obtenirProjection() const
   ///
   /// Retourne la projection orthogonale associ�e � cette vue.
   ///
   /// @return La projection orthogonale associ�e � cette vue.
   ///
   ////////////////////////////////////////////////////////////////////////
   const ProjectionPerspective& VuePerspectiveCiel::obtenirProjection() const
   {
      return projection_;
   }

   ProjectionPerspective& VuePerspectiveCiel::obtenirProjection()
   {
      return projection_;
   }



   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::appliquerProjection() const
   ///
   /// Applique la matrice de projection correspondant � cette vue.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::appliquerProjection() const
   {
      projection_.mettreAJourProjection();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::appliquerCamera() const
   ///
   /// Applique la matrice correspondant � cette vue selon la position de
   /// la cam�ra (eventuellement un gluLookAt()).
   /// 
   /// @param[in]	laquelle :	quelle camera a utiliser
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::appliquerCamera(int laquelle/* = 1*/) const
   {
      obtenirCamera().positionner();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::redimensionnerFenetre( const Vecteur2i& coinMin, const Vecteur2i& coinMax )
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
   void VuePerspectiveCiel::redimensionnerFenetre( const Vecteur2i& coinMin,
                                         const Vecteur2i& coinMax )
   {
      projection_.redimensionnerFenetre(coinMin, coinMax);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::zoomerIn()
   ///
   /// Permet de faire un zoom in selon l'incr�ment de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::zoomerIn()
   {
	   Vecteur3 deplacement = obtenirCamera().obtenirPointVise()-obtenirCamera().obtenirPosition();
	   deplacement*=0.1;
	   obtenirCamera().deplacerXYZ(deplacement);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::zoomerInSmooth()
   ///
   /// Permet de faire un zoom in smooth selon l'incr�ment de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::zoomerInSmooth()
   {
	   Vecteur3 deplacement = obtenirCamera().obtenirPointVise()-obtenirCamera().obtenirPosition();
	   deplacement*=0.005;
	   obtenirCamera().deplacerXYZ(deplacement);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::zoomerOut()
   ///
   /// Permet de faire un zoom out selon l'incr�ment de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::zoomerOut()
   {
	   Vecteur3 deplacement = obtenirCamera().obtenirPosition()-obtenirCamera().obtenirPointVise();
	   deplacement*=0.1;
	   obtenirCamera().deplacerXYZ(deplacement);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::zoomerOutSmooth()
   ///
   /// Permet de faire un zoom out smooth selon l'incr�ment de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::zoomerOutSmooth()
   {
	   Vecteur3 deplacement = obtenirCamera().obtenirPosition()-obtenirCamera().obtenirPointVise();
	   deplacement*=0.005;
	   obtenirCamera().deplacerXYZ(deplacement);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::zoomerInElastique(const Vecteur2i& coin1, const Vecteur2i& coin2)
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
   void VuePerspectiveCiel::zoomerInElastique(const Vecteur2i& coin1,
                                    const Vecteur2i& coin2)
   {
      //projection_.zoomerIn(coin1, coin2);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::zoomerOutElastique(const Vecteur2i& coin1, const Vecteur2i& coin2)
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
   void VuePerspectiveCiel::zoomerOutElastique(const Vecteur2i& coin1,
                                     const Vecteur2i& coin2)
   {
      //projection_.zoomerOut(coin1, coin2);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::deplacerXYSouris(double deplacementX, double deplacementY)
   ///
   /// @param[in]  deplacementX : D�placement en pourcentage de la largeur.
   /// @param[in]  deplacementY : D�placement en pourcentage de la hauteur.
   ///
   /// Permet de faire un "pan" d'un certain pourcentage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::deplacerXYSouris(double deplacementX, double deplacementY)
   {
      //projection_.translater(deplacementX, deplacementY);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::deplacerXYSouris(const Vecteur2i& deplacement)
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
   void VuePerspectiveCiel::deplacerXYSouris(const Vecteur2i& deplacement)
   {
      
// 	  double distance = (obtenirCamera().obtenirPointVise()-obtenirCamera().obtenirPosition()).norme();
// 	  double angle = projection_.obtenirAngleOuverture()*M_PI/360.0;
// 	  double facteur = tan(angle)*distance;
// 	  double pourcentageX = (double)deplacement[VX]/(double)projection_.obtenirDimensionCloture()[VX];
// 	  double pourcentageY = (double)deplacement[VY]/(double)projection_.obtenirDimensionCloture()[VY];
// 	  double facteurX = facteur*pourcentageX*2;
// 	  double facteurY = facteur*pourcentageY*2;
// 
// 	  obtenirCamera().deplacerXY(-facteurX, facteurY, true);

   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::deplacerXYFleches(double deplacementX, double deplacementY)
   ///
   /// @param[in]  deplacementX : D�placement en pourcentage de la largeur.
   /// @param[in]  deplacementY : D�placement en pourcentage de la hauteur.
   ///
   /// Permet de faire un "pan" d'un certain pourcentage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::deplacerXYFleches(double deplacementX, double deplacementY)
   {
	   //projection_.translater(deplacementX, deplacementY);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::deplacerXYFleches(const Vecteur2i& deplacement)
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
   void VuePerspectiveCiel::deplacerXYFleches(const Vecteur2i& deplacement)
   {

	   //deplacerXYSouris(deplacement);

   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::deplacerZ(double deplacement)
   ///
   /// Ne fait rien, car se d�placer dans l'axe de la profondeur n'a pas
   /// vraiment de signification avec une vue orthogonale.
   ///
   /// @param[in]  deplacement : D�placement selon l'axe Z.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::deplacerZ(double deplacement)
   {
	   Vecteur2 deplacementVirtuel = projection_.convertirDistanceClotureAVirtuelle(Vecteur2(deplacement, 0));
	   obtenirCamera().deplacerZ(deplacementVirtuel[0], 0);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::rotaterXY(double rotationX, double rotationY)
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
   void VuePerspectiveCiel::rotaterXY(double rotationX, double rotationY)
   {
      //obtenirCamera().orbiterXY(rotationX * 2 * M_PI, rotationY * M_PI);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::rotaterXY(const Vecteur2i& rotation)
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
   void VuePerspectiveCiel::rotaterXY(const Vecteur2i& rotation)
   {
      //const Vecteur2i dimensions = projection_.obtenirDimensionCloture();
      //rotaterXY(rotation[0] / (double) dimensions[0],
                //rotation[1] / (double) dimensions[1]);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::rotaterZ(double rotation)
   ///
   /// Ne fait rien, car tourner autour de l'axe de la profondeur
   /// correspondrait � un rouli et n'est pas souhaitable pour cette vue.
   ///
   /// @param[in]  rotation : Rotation selon l'axe Z.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveCiel::rotaterZ(double rotation)
   {
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vecteur2 VuePerspectiveCiel::convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const
   ///
   /// Conversion des coordonn�es virtuelles.
   ///
   /// @param[in]  const Vecteur2i& v : Le premier vecteur.
   /// @param[in]  const Vecteur2i& v2 : Le deuxi�me vecteur.
   ///
   /// @return Le vectueur r�sultant.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur2 VuePerspectiveCiel::convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const
   {
	   Vecteur3 virt1;
	   Vue::convertirClotureAVirtuelle(v[VX], v[VY], virt1);
	   Vecteur3 virt2;
	   Vue::convertirClotureAVirtuelle(v2[VX], v2[VY], virt2);
	   Vecteur3 diff = virt1-virt2;
	   diff[VY]*=-1;
	   return diff.convertir<2>();
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vecteur3 VuePerspectiveCiel::convertirClotureAVirtuelle( const Vecteur2i& v ) const
   ///
   /// Conversions des coordonn�es de clot�re.
   ///
   /// @param[in]  const Vecteur2i& v : Le vecteur � convertir.
   ///
   /// @return Le vecteur r�sultant.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur3 VuePerspectiveCiel::convertirClotureAVirtuelle( const Vecteur2i& v ) const
   {
	   Vecteur3 virt;
	   Vue::convertirClotureAVirtuelle(v[VX], v[VY], virt);
	   return virt;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveCiel::obtenirAngleOuvertureProjection()
   ///
   /// Retourne l<angle d<ouverture de la projection
   ///
   /// 
   /// @return double :	angle d<ouverture en degres
   ///
   ////////////////////////////////////////////////////////////////////////
   double VuePerspectiveCiel::obtenirAngleOuvertureProjection()
   {
	   return projection_.obtenirAngleOuverture();
   }





}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
