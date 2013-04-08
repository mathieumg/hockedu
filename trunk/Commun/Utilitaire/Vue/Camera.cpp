///////////////////////////////////////////////////////////////////////////////
/// @file Camera.cpp
/// @author DGI
/// @date 2006-12-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#if WIN32
#define _WINSOCKAPI_
#include <windows.h>
#include <GL/glu.h>

#elif __APPLE__
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "glu.h"
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "Utilitaire.h"
#include "Camera.h"
#define DEG2RAD(a) ((a) * M_PI / 180.0)
#define CLIP(a,min,max) (a < min) ? min : ((a > max) ? max : a)

const int LimitZoomIn = 100;
const int LimitZoomOut = 2000;

namespace vue {

    bool isDistanceValid(float length)
    {
        unsigned int lengthCrop = length - LimitZoomIn;
        return lengthCrop < LimitZoomOut;
    }

   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Camera::Camera(const Vecteur3& position, const Vecteur3& pointVise, const Vecteur3& directionHautCamera, const Vecteur3& directionHautMonde)
   ///
   /// Constructeur de la cam�ra � partir des coordonn�es cart�siennes.
   ///
   /// @param[in]  position            : position de la cam�ra.
   /// @param[in]  pointVise           : point vis�.
   /// @param[in]  directionHautCamera : direction du haut de la cam�ra.
   /// @param[in]  directionHautMonde  : direction du haut du monde de la
   ///                                   cam�ra.
   ///
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////////
   Camera::Camera( const Vecteur3& position,
                   const Vecteur3& pointVise,
                   const Vecteur3& directionHautCamera,
                   const Vecteur3& directionHautMonde
                   )
                   : position_(position),
					 pointVise_(pointVise),
                     directionHaut_(directionHautCamera),
                     directionHautMonde_(directionHautMonde)
   {
	 //pointVise_ = new Vecteur3(pointVise);
   }


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::deplacerXY( float deplacementX, float deplacementY )
   ///
   /// D�place la cam�ra dans le plan perpendiculaire � la direction vis�e
   ///
   /// @param[in]  deplacementX : D�placement sur l'axe horizontal du plan de
   ///                            la cam�ra.
   /// @param[in]  deplacementY : D�placement sur l'axe vertical du plan de la
   ///                            cam�ra.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Camera::deplacerXY(float deplacementX, float deplacementY, bool avecPointVise)
   {
	   /*position_[VX]+=deplacementX;
	   position_[VY]+=deplacementY;
	   if(avecPointVise)
	   {
		   pointVise_[VX]+=deplacementX;
		   pointVise_[VY]+=deplacementY;
	   }
	   */

	   Vecteur3 haut = directionHaut_;
	   Vecteur3 vise = pointVise_-position_;
	   haut.normaliser();
	   vise.normaliser();
	   Vecteur3 direction = produitVectoriel(haut, vise); // Devrait etre normalise

	   position_+=(-deplacementX*direction+deplacementY*haut);
	   if(avecPointVise)
			pointVise_+=(-deplacementX*direction+deplacementY*haut);

   }


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::deplacerZ( float deplacement, bool bougePointVise )
   ///
   /// D�place la cam�ra dans l'axe de la direction vis�e.
   ///
   /// @param[in]  deplacement    : D�placement sur l'axe de la direction vis�e
   /// @param[in]  bougePointVise : Si vrai, le point de vis� est �galement
   ///                              d�plac�.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Camera::deplacerZ(float deplacement, bool bougePointVise, bool avecPointVise)
   {
	   Vecteur3 nouvellePosition = position_+Vecteur3(0,0,deplacement);
	   float distanceCentre = (nouvellePosition-pointVise_).norme();
	   if(nouvellePosition[VZ]>-20.0 && isDistanceValid(distanceCentre))
	   {
		   position_ = nouvellePosition;
		   if(avecPointVise)
			   (pointVise_)[VZ]+=deplacement;
	   }
	   
   }

   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::deplacerXYZ( Vecteur3 deplacement )
   ///
   /// D�place la cam�ra dans les 3 axes
   ///
   /// @param[in]  deplacement    : D�placement a effectuer
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Camera::deplacerXYZ( Vecteur3 deplacement, bool avecPointVise )
   {
	   Vecteur3 nouvellePosition = position_+deplacement;
	   float distanceCentre = (nouvellePosition-pointVise_).norme();
	   if(nouvellePosition[VZ]>-20.0f && isDistanceValid(distanceCentre))
	   {
		   position_ = nouvellePosition;
		   if(avecPointVise)
			   pointVise_+=deplacement;
	   }
   }



   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::tournerXY( float rotationX, float rotationY, bool empecheInversion )
   ///
   /// Rotation de la cam�ra autour de sa position (et donc d�placement du
   /// point vis� en gardant la position fixe.
   ///
   /// @param[in] rotationX        : Modification de l'angle de rotation du
   ///                               point vis� par rapport � la position.
   /// @param[in] rotationY        : Modification de l'angle d'�l�vation du
   ///                               point vis� par rapport � la position.
   /// @param[in] empecheInversion : Si vrai, la rotation n'est pas effectu�e
   ///                               si elle am�nerait une inversion de la
   ///                               cam�ra.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Camera::tournerXY(float rotationX,
                          float rotationY,
                          bool   empecheInversion //=true
                          )
   {
	   // Deplacer le point viser pour donner l'illusion que la camera tourne sur elle meme

	   // On calcule le rayon courant
	   float r = (pointVise_-position_).norme();
	   // On calcule les angles courants
	   float theta = atan2(position_[VY]-(pointVise_)[VY],position_[VX]-(pointVise_)[VX]);
	   float phi = acos((position_[VZ]-(pointVise_)[VZ])/r);
	   // On effectue la modification des angles
	   theta-=rotationX;
	   float phiTemp = phi-rotationY;
	   phi = CLIP(phiTemp, 0.00001f, (float)M_PI); // On limite la rotation sur cet axe
	   
	   // On calcule la nouvelle position et on l'applique
	   float dx = r*cos(theta)*sin(phi);
	   float dy = r*sin(theta)*sin(phi);
	   float dz = r*cos(phi);
	   Vecteur3 nouvellePosition(dx, dy, dz);
	   pointVise_ = position_-nouvellePosition;


	   recalculerHaut();
   }


   ////////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::orbiterXY( float rotationX, float rotationY, bool empecheInversion )
   ///
   /// Rotation de la cam�ra autour de son point de vis� (et donc d�placement
   /// de la position en gardant le point de vis� fixe.
   ///
   /// @param[in]  rotationX        : Modification de l'angle de rotation de la
   ///                                position par rapport au point de vis�.
   /// @param[in]  rotationY        : Modification de l'angle d'�l�vation de la
   ///                                position par rapport au point de vis�.
   ///
   ///  @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////////
   void Camera::orbiterXY(float rotationX,
                          float rotationY
                          )
   {
	   // On calcule le rayon courant
	   float r = (pointVise_-position_).norme();
	   // On calcule les angles courants
	   float theta = atan2(position_[VY]-(pointVise_)[VY],position_[VX]-(pointVise_)[VX]);
	   float phi = acos((position_[VZ]-(pointVise_)[VZ])/r);
	   // On effectue la modification des angles
	   theta-=rotationX;
	   float phiTemp = phi-rotationY;
	   phi = CLIP(phiTemp, 0.000001f, (float)M_PI/2.f); // On limite la rotation sur cet axe
	   // On calcule la nouvelle position et on l'applique
	   float dx = r*cos(theta)*sin(phi);
	   float dy = r*sin(theta)*sin(phi);
	   float dz = r*cos(phi);
	   Vecteur3 nouvellePosition(dx, dy, dz);
	   float distanceCentre = (nouvellePosition-pointVise_).norme();
	   if(nouvellePosition[VZ]>-20.0f && isDistanceValid(distanceCentre))
	   {
		   position_ = nouvellePosition+pointVise_;

		   // On calcule maintenant le veteur "Haut" de la camera
		   recalculerHaut();
	   }
	   

   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::positionner() const
   ///
   /// Positionne la cam�ra dans la sc�ne � l'aide de gluLookAt().
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void Camera::positionner() const
   {
      gluLookAt( position_[0]      , position_[1]      , position_[2],
                 (pointVise_)[0]     , (pointVise_)[1]     , (pointVise_)[2],
                 directionHaut_[0] , directionHaut_[1] , directionHaut_[2] );
       
   }

   void Camera::appliquerAnimation( const ObjectAnimationParameters& pAnimationResult )
   {
	   if(pAnimationResult.CanUpdatedPosition())
			position_ = pAnimationResult.mPosition;
	   if(pAnimationResult.CanUpdatedAngle())
			pointVise_ = pAnimationResult.mAngle;
	   if(pAnimationResult.CanUpdatedScale() &&  !pAnimationResult.mScale.estNul())
			directionHaut_ = pAnimationResult.mScale;
	   positionner();
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::recalculerHaut()
   ///
   /// Recalcule le vecteur haut de la camera
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void Camera::recalculerHaut() 
   {
	   Vecteur3 vise = pointVise_-position_;
	   vise.normaliser();
	   Vecteur3 proj0xy = vise;
	   proj0xy[VZ] = 0.0; // Projection sur 0xy
	   Vecteur3 cote = produitVectoriel(proj0xy, Vecteur3(0, 0, 1));

	   Vecteur3 nouveauHaut = produitVectoriel(cote, vise); // De cette facon, le vecteur pointe toujours vers le point du centre et vers le haut
	   nouveauHaut.normaliser();
	   directionHaut_ = nouveauHaut;
   }

   Vecteur3 Camera::obtenirPosition()
   {
	   return position_;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Camera::operator=( const Camera& autreCamera )
   ///
   /// Operateur de copie
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   Camera& Camera::operator=( const Camera& autreCamera )
   {
	   this->assignerPosition(autreCamera.obtenirPosition());
	   this->assignerPointVise(autreCamera.obtenirPointVise());
	   this->assignerDirectionHaut(autreCamera.obtenirDirectionHaut());
	   this->assignerDirectionHautMonde(autreCamera.obtenirDirectionHautMonde());
	   return *this;
   }

  

}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
