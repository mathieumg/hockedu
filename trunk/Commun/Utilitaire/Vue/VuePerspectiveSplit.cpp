////////////////////////////////////////////////////////////////////////////////////
/// @file VuePerspectiveSplit.cpp
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
#include "VuePerspectiveSplit.h"
#include "Camera.h"
#include "Vue.h"
#include <iostream>



namespace vue {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn VuePerspectiveSplit::VuePerspectiveSplit(const Camera& camera, int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture, float zAvant, float zArriere, float zoomInMax, float zoomOutMax, float incrementZoom, float xMinFenetre, float xMaxFenetre, float yMinFenetre, float yMaxFenetre)
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
   /// @param[in] fovY			: angle d'ouverture de la camera
   /// 
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   VuePerspectiveSplit::VuePerspectiveSplit(const Camera& camera,
                      int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture,
                      float zAvant,  float zArriere,
                      float zoomInMax, float zoomOutMax,
                      float incrementZoom,
                      float xMinFenetre, float xMaxFenetre,
                      float yMinFenetre, float yMaxFenetre) :
      Vue(camera, camera, 2),
      projection_(xMinCloture, xMaxCloture, yMinCloture, yMaxCloture,
                  zAvant, zArriere, zoomInMax, zoomOutMax,
                  incrementZoom,
                  xMinFenetre, xMaxFenetre, yMinFenetre, yMaxFenetre)
   {
	   nbViewports_ = 2;

	   Vecteur3 positionCourante = camera_[1].obtenirPosition();
	   positionCourante[VX]*=-1;
	   camera_[1].assignerPosition(positionCourante);


	   appliquerProjection();
	   
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn const ProjectionPerspective& VuePerspectiveSplit::obtenirProjection() const
   ///
   /// Retourne la projection orthogonale associée à cette vue.
   ///
   /// @return La projection orthogonale associée à cette vue.
   ///
   ////////////////////////////////////////////////////////////////////////
   const ProjectionPerspective& VuePerspectiveSplit::obtenirProjection() const
   {
      return projection_;
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn    ProjectionPerspective& VuePerspectiveSplit::obtenirProjection()
   ///
   /// Retourne la projection orthogonale associée à cette vue.
   ///
   /// @return La projection orthogonale associée à cette vue.
   ///
   ////////////////////////////////////////////////////////////////////////
   ProjectionPerspective& VuePerspectiveSplit::obtenirProjection()
   {
      return projection_;
   }



   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::appliquerProjection() const
   ///
   /// Applique la matrice de projection correspondant à cette vue.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::appliquerProjection() const
   {
      projection_.mettreAJourProjection();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::appliquerCamera() const
   ///
   /// Applique la matrice correspondant à cette vue selon la position de
   /// la caméra (eventuellement un gluLookAt()).
   ///
   /// @param[in]	laquelle : quelle camera a utiliser
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::appliquerCamera(int laquelle/* = 1*/) const
   {
	   
	   obtenirCamera(laquelle).positionner();
	   
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::redimensionnerFenetre( const Vecteur2i& coinMin, const Vecteur2i& coinMax )
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
   void VuePerspectiveSplit::redimensionnerFenetre( const Vecteur2i& coinMin,
                                         const Vecteur2i& coinMax )
   {
      projection_.redimensionnerFenetre(coinMin, coinMax);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::zoomerIn()
   ///
   /// Permet de faire un zoom in selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::zoomerIn()
   {
	   Vecteur3 deplacement = obtenirCamera(1).obtenirPointVise()-obtenirCamera(1).obtenirPosition();
	   deplacement*=0.1f;
	   obtenirCamera(1).deplacerXYZ(deplacement);

	   deplacement = obtenirCamera(2).obtenirPointVise()-obtenirCamera(2).obtenirPosition();
	   deplacement*=0.1f;
	   obtenirCamera(2).deplacerXYZ(deplacement);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::zoomerInSmooth()
   ///
   /// Permet de faire un zoom in smooth selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::zoomerInSmooth()
   {
	   Vecteur3 deplacement = obtenirCamera().obtenirPointVise()-obtenirCamera().obtenirPosition();
	   deplacement*=0.005f;
	   obtenirCamera().deplacerXYZ(deplacement);

	   deplacement = obtenirCamera(2).obtenirPointVise()-obtenirCamera(2).obtenirPosition();
	   deplacement*=0.005f;
	   obtenirCamera(2).deplacerXYZ(deplacement);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::zoomerOut()
   ///
   /// Permet de faire un zoom out selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::zoomerOut()
   {
	   Vecteur3 deplacement = obtenirCamera().obtenirPosition()-obtenirCamera().obtenirPointVise();
	   deplacement*=0.1f;
	   obtenirCamera().deplacerXYZ(deplacement);

	   deplacement = obtenirCamera(2).obtenirPosition()-obtenirCamera(2).obtenirPointVise();
	   deplacement*=0.1f;
	   obtenirCamera(2).deplacerXYZ(deplacement);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::zoomerOutSmooth()
   ///
   /// Permet de faire un zoom out smooth selon l'incrément de zoom.
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::zoomerOutSmooth()
   {
	   Vecteur3 deplacement = obtenirCamera().obtenirPosition()-obtenirCamera().obtenirPointVise();
	   deplacement*=0.005f;
	   obtenirCamera().deplacerXYZ(deplacement);

	   deplacement = obtenirCamera(2).obtenirPosition()-obtenirCamera(2).obtenirPointVise();
	   deplacement*=0.005f;
	   obtenirCamera(2).deplacerXYZ(deplacement);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::zoomerInElastique(const Vecteur2i& coin1, const Vecteur2i& coin2)
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
   void VuePerspectiveSplit::zoomerInElastique(const Vecteur2i& coin1,
                                    const Vecteur2i& coin2)
   {
      //projection_.zoomerIn(coin1, coin2);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::zoomerOutElastique(const Vecteur2i& coin1, const Vecteur2i& coin2)
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
   void VuePerspectiveSplit::zoomerOutElastique(const Vecteur2i& coin1,
                                     const Vecteur2i& coin2)
   {
      //projection_.zoomerOut(coin1, coin2);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::deplacerXYSouris(float deplacementX, float deplacementY)
   ///
   /// @param[in]  deplacementX : Déplacement en pourcentage de la largeur.
   /// @param[in]  deplacementY : Déplacement en pourcentage de la hauteur.
   ///
   /// Permet de faire un "pan" d'un certain pourcentage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::deplacerXYSouris(float deplacementX, float deplacementY)
   {
      //projection_.translater(deplacementX, deplacementY);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::deplacerXYSouris(const Vecteur2i& deplacement)
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
   void VuePerspectiveSplit::deplacerXYSouris(const Vecteur2i& deplacement)
   {
      
	  //float distance = (obtenirCamera().obtenirPointVise()-obtenirCamera().obtenirPosition()).norme();
	  float angle = projection_.obtenirAngleOuverture()*(float)M_PI/180.0f;
	  float facteur = tan(angle)*100.0f;
	  float pourcentageX = (float)deplacement[VX]/(float)projection_.obtenirDimensionCloture()[VX];
	  float pourcentageY = (float)deplacement[VY]/(float)projection_.obtenirDimensionCloture()[VY];
	  float facteurX = facteur*pourcentageX*2;
	  float facteurY = facteur*pourcentageY*2;

	  obtenirCamera(1).deplacerXY(-facteurX, facteurY, true);
	  obtenirCamera(2).deplacerXY(-facteurX, facteurY, true);

   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::deplacerXYFleches(float deplacementX, float deplacementY)
   ///
   /// @param[in]  deplacementX : Déplacement en pourcentage de la largeur.
   /// @param[in]  deplacementY : Déplacement en pourcentage de la hauteur.
   ///
   /// Permet de faire un "pan" d'un certain pourcentage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::deplacerXYFleches(float deplacementX, float deplacementY)
   {
	   //projection_.translater(deplacementX, deplacementY);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::deplacerXYFleches(const Vecteur2i& deplacement)
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
   void VuePerspectiveSplit::deplacerXYFleches(const Vecteur2i& deplacement)
   {

	   deplacerXYSouris(-deplacement);

   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::deplacerZ(float deplacement)
   ///
   /// Ne fait rien, car se déplacer dans l'axe de la profondeur n'a pas
   /// vraiment de signification avec une vue orthogonale.
   ///
   /// @param[in]  deplacement : Déplacement selon l'axe Z.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::deplacerZ(float deplacement)
   {
	   Vecteur2 deplacementVirtuel = projection_.convertirDistanceClotureAVirtuelle(Vecteur2(deplacement, 0));
	   obtenirCamera(1).deplacerZ(deplacementVirtuel[0], 0);
	   obtenirCamera(2).deplacerZ(deplacementVirtuel[0], 0);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::rotaterXY(float rotationX, float rotationY)
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
   void VuePerspectiveSplit::rotaterXY(float rotationX, float rotationY)
   {
      obtenirCamera(1).orbiterXY(rotationX * 2 * (float)M_PI, rotationY * (float)M_PI);
	  obtenirCamera(2).orbiterXY(rotationX * 2 * (float)M_PI, rotationY * (float)M_PI);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::rotaterXY(const Vecteur2i& rotation)
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
   void VuePerspectiveSplit::rotaterXY(const Vecteur2i& rotation)
   {
      const Vecteur2i dimensions = projection_.obtenirDimensionCloture();
      rotaterXY(rotation[0] / (float) dimensions[0],
                rotation[1] / (float) dimensions[1]);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::rotaterZ(float rotation)
   ///
   /// Ne fait rien, car tourner autour de l'axe de la profondeur
   /// correspondrait à un rouli et n'est pas souhaitable pour cette vue.
   ///
   /// @param[in]  rotation : Rotation selon l'axe Z.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::rotaterZ(float rotation)
   {
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vecteur2 VuePerspectiveSplit::convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const
   ///
   /// Conversion des coordonnées virtuelles.
   ///
   /// @param[in]  const Vecteur2i& v : Le premier vecteur.
   /// @param[in]  const Vecteur2i& v2 : Le deuxième vecteur.
   ///
   /// @return Le vectueur résultant.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur2 VuePerspectiveSplit::convertirDeplacementClotureAVirtuelle( const Vecteur2i& v, const Vecteur2i& v2  ) const
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
   /// @fn Vecteur3 VuePerspectiveSplit::convertirClotureAVirtuelle( const Vecteur2i& v ) const
   ///
   /// Conversions des coordonnées de clotûre.
   ///
   /// @param[in]  const Vecteur2i& v : Le vecteur à convertir.
   ///
   /// @return Le vecteur résultant.
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur3 VuePerspectiveSplit::convertirClotureAVirtuelle( const Vecteur2i& v ) const
   {
	   Vecteur3 virt;
	   Vue::convertirClotureAVirtuelle(v[VX], v[VY], virt);
	   return virt;
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::obtenirAngleOuvertureProjection()
   ///
   /// Retourne l<angle d<ouverture de la projection
   ///
   /// 
   /// @return float :	angle d<ouverture en degres
   ///
   ////////////////////////////////////////////////////////////////////////
   float VuePerspectiveSplit::obtenirAngleOuvertureProjection()
   {
	   return projection_.obtenirAngleOuverture();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void VuePerspectiveSplit::obtenirAngleOuvertureProjection()
   ///
   /// Applique le viewport correspondant
   ///
   /// @param[in]	lequel :	quelViewport
   /// 
   /// @return float :	angle d<ouverture en degres
   ///
   ////////////////////////////////////////////////////////////////////////
   void VuePerspectiveSplit::appliquerViewport( int lequel )
   {
	   projection_.mettreAJourCloture(lequel);
   }


}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
