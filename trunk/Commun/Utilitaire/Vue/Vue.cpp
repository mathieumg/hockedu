/////////////////////////////////////////////////////////////////////////////////
/// @file Vue.cpp
/// @author DGI
/// @date 2006-12-18
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
/////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <GL/glu.h>

#include "Vue.h"
#include "Plan3D.h"
#include "Droite3D.h"
#define _USE_MATH_DEFINES
#include <math.h>


namespace vue {


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vue::Vue(const Camera& camera)
   ///
   /// Ce constructeur permet d'assigner la caméra de cette vue.
   ///
   /// @param[in] camera : La caméra associée à cette vue.
   /// @param[in] camera2 : La caméra2 associée à cette vue.
   /// @param[in] nbViewports : Le nombre de viewports
   ///
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////////
   Vue::Vue(const Camera& camera, const Camera& camera2 /*= Camera()*/, int nbViewports/* = 1*/)
   {
	   camera_[0] = camera;
	   nbViewports_ = nbViewports;
	   if(nbViewports>1)
		   camera_[1] = camera2;
   }


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool Vue::convertirClotureAVirtuelle(int x, int y, const math::Plan3D& plan, Vecteur3& point) const
   ///
   /// Cette fonction permet de transformer un point (donné en coordonnées
   /// d'affichage) en coordonnées virtuelles étant donné un certain plan sur
   /// lequel doit se trouver le point.  Elle utilise les fonction d'OpenGL
   /// donc cette fonction s'applique peu importe la position de la caméra.
   ///
   /// @param[in]      x     : La position @a X du point en coordonnée
   ///                         d'affichage.
   /// @param[in]      y     : La position @a Y du point en coordonnée
   ///                         d'affichage.
   /// @param[in]      plan  : Le plan sur lequel on veut trouver la position
   ///                         correspondante en virtuel.
   /// @param[in, out] point : point transformé @a (x,y) le @a z est le même
   ///                         que le plan
   ///
   /// @return Faux s'il est impossible de convertir le point en virtuel
   ///         puisque le plan est parallèle à la direction de la caméra, vrai
   ///         autrement.
   ///
   ////////////////////////////////////////////////////////////////////////////
   bool Vue::convertirClotureAVirtuelle(int x, int y, const math::Plan3D& plan, Vecteur3& point) const
   {
      //Initialisation de variables.
      const GLdouble MinZ = 0.0;
      const GLdouble MaxZ = 1.0;

      // Lire la matrice de modélisation et de visualisation.
      GLdouble matriceModelisation[16];
      glGetDoublev ( GL_MODELVIEW_MATRIX, matriceModelisation );

      // Lire la matrice de projection.
      GLdouble matriceProjection[16];
      glGetDoublev ( GL_PROJECTION_MATRIX, matriceProjection );

      // Lire la clôture.
      GLint cloture[4];
      glGetIntegerv ( GL_VIEWPORT, cloture );

      // Premier point.
      Vecteur3 point1;
      gluUnProject(
         x, cloture[3] - y, MinZ,
         matriceModelisation, matriceProjection, cloture,
         &point1[0], &point1[1], &point1[2]
      );

      // Deuxième point.
      Vecteur3 point2;
      gluUnProject(
         x, cloture[3] - y, MaxZ,
         matriceModelisation, matriceProjection, cloture,
         &point2[0], &point2[1], &point2[2]
      );

      // On construit la droite.
	   math::Droite3D* DroiteCoupe;
	   try
	   {
		   DroiteCoupe = new math::Droite3D( point1, point2 );
	   }
	   catch(...)
	   {
		   return false;
	   }
    
      return DroiteCoupe->intersection( plan, point );
   }


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool Vue::convertirClotureAVirtuelle(int x, int y, Vecteur3& point) const
   ///
   /// Cette fonction permet de transformer un point (donné en coordonnées
   /// d'affichage) en coordonnées virtuelles dans le plan XY.
   ///
   /// @param[in]      x     : La position @a X du point en coordonnée
   ///                         d'affichage.
   /// @param[in]      y     : La position @a Y du point en coordonnée
   ///                         d'affichage.
   /// @param[in, out] point : point transformé @a (x,y) le @a z est le même
   ///                         que le plan
   ///
   /// @return Faux s'il est impossible de convertir le point en virtuel
   ///         puisque le plan est parallèle à la direction de la caméra.
   ///
   ////////////////////////////////////////////////////////////////////////////
   bool Vue::convertirClotureAVirtuelle(int x, int y, Vecteur3& point) const
   {
      const math::Plan3D plan(Vecteur3(0,0,1), Vecteur3(0,0,0));
      return convertirClotureAVirtuelle(x, y, plan, point);
   }



   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Vue::animer(double temps)
   ///
   /// Cette fonction permet de faire évoluer la vue en fonction du temps, par
   /// exemple lorsque cette dernière se déplace par rapport à un objet en
   /// fonction du temps.
   ///
   /// @param[in]  temps double : L'intervalle de temps pour lequel réaliser
   ///                            l'animation.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Vue::animer(double temps)
   {
   }


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Vue::modifierCamera( Camera camera )
   ///
   /// Modification de la camera
   ///
   /// @param[in]  camera Camera	: camera possedant les attributs a copier
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Vue::modifierCamera( Camera camera, int laquelle/* = 1*/ )
   {
	   obtenirCamera(laquelle) = camera;


   }



   void Vue::centrerCamera(double largeurVue)
   {
	   GestionnaireAnimations::obtenirInstance()->viderAnimationCamera();
	   double angleOuverture = obtenirAngleOuvertureProjection();
	   Vecteur3 positionOptimale = Vecteur3(0, -0.0001, 300);
	   if(angleOuverture!=0.0 && largeurVue!=0.0)
	   {
		   // On modifie la position selon l'angle pour voir toute la zone d'edition
		   double angle = angleOuverture*M_PI/360.0;
		   double distance = largeurVue/(2.0*tan(angle));
		   positionOptimale[VZ] = distance;
	   }

	   vue::Camera* cameraCourante = &obtenirCamera();

	   AnimationFrame* frame[4];
	   frame[0] = new AnimationFrame(0, cameraCourante->obtenirPosition(), cameraCourante->obtenirPointVise(), cameraCourante->obtenirDirectionHaut());
	   frame[1] = new AnimationFrame(800, positionOptimale, Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));
	   frame[2] = new AnimationFrame(1000, positionOptimale, Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));
	   frame[3] = new AnimationFrame(1100, positionOptimale, Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));


	   Animation* animation = new Animation(LINEAIRE, true, true, true);
	   for(int i=0; i<4; i++)
		   animation->ajouterFrame(frame[i]);

	   animation->ajouterObjet(cameraCourante);
	   GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
   }



}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
