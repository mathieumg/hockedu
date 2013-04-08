/////////////////////////////////////////////////////////////////////////////////
/// @file Vue.cpp
/// @author DGI
/// @date 2006-12-18
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
/////////////////////////////////////////////////////////////////////////////////
#if WIN32
#define _WINSOCKAPI_
#include <windows.h>
#include <GL/glu.h>

#elif __APPLE__
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "glu.h"
#endif

#include "Vue.h"
#include "Plan3D.h"
#include "Droite3D.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Projection.h"


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
      const GLfloat MinZ = 0.0;
      const GLfloat MaxZ = 1.0;

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
#if !WIN32
      Vecteur3 point1;
      Vecteur3 point2;
#else
      Vecteur3D<double> point1;
      Vecteur3D<double> point2;
#endif
      gluUnProject(
         x, cloture[3] - y, MinZ,
         matriceModelisation, matriceProjection, cloture,
         &point1[0], &point1[1], &point1[2]
      );

      // Deuxième point.
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
   /// @fn void Vue::animer(float temps)
   ///
   /// Cette fonction permet de faire évoluer la vue en fonction du temps, par
   /// exemple lorsque cette dernière se déplace par rapport à un objet en
   /// fonction du temps.
   ///
   /// @param[in]  temps float : L'intervalle de temps pour lequel réaliser
   ///                            l'animation.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Vue::animer(float temps)
   {
   }




   void Vue::centrerCamera(float largeurVue,int pViewportNumber, AnimationTerminatedCallback pCallback)
   {
	   GestionnaireAnimations::obtenirInstance()->viderAnimationCamera();
	   float angleOuverture = obtenirAngleOuvertureProjection();
	   Vecteur3 positionOptimale = Vecteur3(0, -0.0001f, 300);
	   if(angleOuverture!=0.0 && largeurVue!=0.0)
	   {
		   // On modifie la position selon l'angle pour voir toute la zone d'edition
		   float angle = angleOuverture*((float)M_PI)/360.0f;
		   float distance = largeurVue/(2.0f*tan(angle));
		   positionOptimale[VZ] = distance;
	   }

	   vue::Camera* cameraCourante = &obtenirCamera(pViewportNumber);
       Vecteur3 cameraPos = cameraCourante->obtenirPosition();

       auto delta = cameraPos-positionOptimale;
       float temps = delta.norme();
       
       if(temps>2000)
       {
           temps = 2000;
       }
       
	   AnimationFrame* frame[4];
	   frame[0] = new AnimationFrame(0, cameraPos, cameraCourante->obtenirPointVise(), cameraCourante->obtenirDirectionHaut());
	   frame[1] = new AnimationFrame(temps, positionOptimale, Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));
	   frame[2] = new AnimationFrame(temps+200, positionOptimale, Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));
	   frame[3] = new AnimationFrame(temps+300, positionOptimale, Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));


	   Animation* animation = new Animation(LINEAIRE, true, true, true);
       animation->mAnimationTerminatedCallback = pCallback;
	   for(int i=0; i<4; i++)
		   animation->ajouterFrame(frame[i]);

	   animation->ajouterObjet(cameraCourante);
	   GestionnaireAnimations::obtenirInstance()->ajouterAnimation(animation);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Vue::appliquerVue( int pViewPortNumber )
   ///
   /// /*Description*/
   ///
   /// @param[in] int pViewPortNumber
   ///
   /// @return void
   ///
   ////////////////////////////////////////////////////////////////////////
   void Vue::appliquerVue( int pViewPortNumber )
   {
       checkf((unsigned int)(pViewPortNumber-1) < (unsigned int)nbViewports_);
       // Positionne la caméra
       glMatrixMode( GL_MODELVIEW );
       appliquerViewport(pViewPortNumber);
       glLoadIdentity();
       appliquerCamera(pViewPortNumber); // gluLookAt

       glMatrixMode( GL_PROJECTION );
       glLoadIdentity();
       GLint viewport[4];
       glGetIntegerv(GL_VIEWPORT, viewport);
       Vecteur2i dimCloture = Vecteur2i(viewport[2], viewport[3]);
       Vecteur2 dimFenetre = obtenirProjection().obtenirDimensionFenetre();
       obtenirProjection().ajusterRapportAspect(dimCloture, dimFenetre); // APPLIQUE AUSSI LA PERSPECTIVE
       glMatrixMode (GL_MODELVIEW);   
   }



}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
