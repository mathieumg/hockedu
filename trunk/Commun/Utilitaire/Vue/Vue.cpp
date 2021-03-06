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
#include "AnimationCamera.h"
#include "ManagerAnimations.h"


namespace vue {


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Vue::Vue(const Camera& camera)
   ///
   /// Ce constructeur permet d'assigner la cam�ra de cette vue.
   ///
   /// @param[in] camera : La cam�ra associ�e � cette vue.
   /// @param[in] camera2 : La cam�ra2 associ�e � cette vue.
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
   /// Cette fonction permet de transformer un point (donn� en coordonn�es
   /// d'affichage) en coordonn�es virtuelles �tant donn� un certain plan sur
   /// lequel doit se trouver le point.  Elle utilise les fonction d'OpenGL
   /// donc cette fonction s'applique peu importe la position de la cam�ra.
   ///
   /// @param[in]      x     : La position @a X du point en coordonn�e
   ///                         d'affichage.
   /// @param[in]      y     : La position @a Y du point en coordonn�e
   ///                         d'affichage.
   /// @param[in]      plan  : Le plan sur lequel on veut trouver la position
   ///                         correspondante en virtuel.
   /// @param[in, out] point : point transform� @a (x,y) le @a z est le m�me
   ///                         que le plan
   ///
   /// @return Faux s'il est impossible de convertir le point en virtuel
   ///         puisque le plan est parall�le � la direction de la cam�ra, vrai
   ///         autrement.
   ///
   ////////////////////////////////////////////////////////////////////////////
   bool Vue::convertirClotureAVirtuelle(int x, int y, const math::Plan3D& plan, Vecteur3& point) const
   {
      //Initialisation de variables.
      const GLfloat MinZ = 0.0;
      const GLfloat MaxZ = 1.0;

      // Lire la matrice de mod�lisation et de visualisation.
      GLdouble matriceModelisation[16];
      glGetDoublev ( GL_MODELVIEW_MATRIX, matriceModelisation );

      // Lire la matrice de projection.
      GLdouble matriceProjection[16];
      glGetDoublev ( GL_PROJECTION_MATRIX, matriceProjection );

      // Lire la cl�ture.
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

      // Deuxi�me point.
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
   /// Cette fonction permet de transformer un point (donn� en coordonn�es
   /// d'affichage) en coordonn�es virtuelles dans le plan XY.
   ///
   /// @param[in]      x     : La position @a X du point en coordonn�e
   ///                         d'affichage.
   /// @param[in]      y     : La position @a Y du point en coordonn�e
   ///                         d'affichage.
   /// @param[in, out] point : point transform� @a (x,y) le @a z est le m�me
   ///                         que le plan
   ///
   /// @return Faux s'il est impossible de convertir le point en virtuel
   ///         puisque le plan est parall�le � la direction de la cam�ra.
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
   /// Cette fonction permet de faire �voluer la vue en fonction du temps, par
   /// exemple lorsque cette derni�re se d�place par rapport � un objet en
   /// fonction du temps.
   ///
   /// @param[in]  temps float : L'intervalle de temps pour lequel r�aliser
   ///                            l'animation.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Vue::animer(float temps)
   {
   }




   void Vue::centrerCamera(float largeurVue,int pViewportNumber)
   {
       vue::Camera* cameraCourante = &obtenirCamera(pViewportNumber);
       cameraCourante->AnimationSubject::signalObservers();

       Vecteur3 positionOptimale = getOptimalPosition(largeurVue);
       Vecteur3 cameraPos = cameraCourante->obtenirPosition();

       auto delta = cameraPos-positionOptimale;
       // time in sec
       float temps = delta.norme()/1000.f;
       
       if(temps>2.f)
       {
           temps = 2.f;
       }
       else if(temps < 0.5f)
       {
           temps = 0.5f;
       }
       AnimationCamera* animation = ManagerAnimations::obtenirInstance()->CreateAnimation<AnimationCamera>().second;
       animation->AddFrame(0, cameraPos, cameraCourante->obtenirPointVise(), cameraCourante->obtenirDirectionHaut());
       animation->AddFrame(temps, positionOptimale, Vecteur3(0, 0, 0), Vecteur3(0, 1, 0));

       animation->SetCamera(cameraCourante);
       animation->SetAlgo(ANIMATION_LINEAR);
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
       // Positionne la cam�ra
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

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Vue::getOptimalPosition( float largeurVue )
   ///
   /// /*Description*/
   ///
   /// @param[in] float largeurVue
   ///
   /// @return void
   ///
   ////////////////////////////////////////////////////////////////////////
   Vecteur3 Vue::getOptimalPosition( float largeurVue )
   {
       float angleOuverture = obtenirAngleOuvertureProjection();
       Vecteur3 positionOptimale = Vecteur3(0, -0.0001f, 300);
       if(angleOuverture!=0.0 && largeurVue!=0.0)
       {
           // On modifie la position selon l'angle pour voir toute la zone d'edition
           float angle = angleOuverture*((float)M_PI)/360.0f;
           float distance = largeurVue/(2.0f*tan(angle));
           positionOptimale[VZ] = distance;
       }
       return positionOptimale;
   }



}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
