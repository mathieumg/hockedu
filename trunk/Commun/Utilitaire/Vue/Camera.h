////////////////////////////////////////////////////////////////////////////////////
/// @file Camera.h
/// @author DGI
/// @date 2006-12-15
/// @version 1.0 
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_CAMERA_H__
#define __UTILITAIRE_CAMERA_H__


#include "Vecteur.h"
#include "ObjetAnimable.h"


namespace vue {


   ///////////////////////////////////////////////////////////////////////////
   /// @class Camera
   /// @brief Classe représentant une caméra dans le monde en 3D.
   ///
   /// Cette camera encapsule les différentes opérations qu'il est possible 
   /// de faire pour déplacer le point de vue de l'observateur à l'intérieur
   /// de la scène en 3D.
   ///
   /// @author Martin Bisson
   /// @date 2006-12-15
   ///////////////////////////////////////////////////////////////////////////
   class Camera : public ObjetAnimable
   {
   public:
      /// Constructeur à partir des coordonnées cartésiennes.
      Camera( const Vecteur3& position,
              const Vecteur3& pointVise,
              const Vecteur3& directionHautCamera,
              const Vecteur3& directionHautMonde );

	  Camera() {position_ = Vecteur3(0, 0, 0);
				pointVise_ = Vecteur3(0, 0, 0);
				directionHaut_ = Vecteur3(0, 0, 0);
				directionHautMonde_ = Vecteur3(0, 0, 0);}

	  Camera& operator =(const Camera& autreCamera);

      /// Destructeur virtuel vide.
      virtual ~Camera() {/*delete pointVise_;*/}

      /// Assigner la position de la caméra.
      inline void assignerPosition(const Vecteur3& position);
      /// Assigner le point visé de la caméra.
      //inline void assignerPointVise(Vecteur3* pointVise);
	  /// Assigner le point visé de la caméra.
	  inline void assignerPointVise(const Vecteur3& pointVise);

      /// Assigner la direction du haut de la caméra.
      inline void assignerDirectionHaut(const Vecteur3& directionHaut);

	  /// Assigner la direction du haut du monde
	  inline void assignerDirectionHautMonde(const Vecteur3& directionHautMonde);


      /// Obtenir la position de la caméra.
      inline const Vecteur3& obtenirPosition() const;
      /// Obtenir le point visé de la caméra.
      inline const Vecteur3& obtenirPointVise() const;
      /// Obtenir la direction du haut de la caméra.
      inline const Vecteur3& obtenirDirectionHaut() const;
	  /// Obtenir la direction du haut de la caméra.
	  inline const Vecteur3& obtenirDirectionHautMonde() const;

      /// Déplacement dans le plan perpendiculaire à la direction visée.
      void deplacerXY(float deplacementX, float deplacementY, bool avecPointVise = false);
      /// Déplacement dans l'axe de la direction visée.
      void deplacerZ(float deplacement, bool bougePointVise, bool avecPointVise = false);
	  /// Deplacement en XYZ
	  void deplacerXYZ(Vecteur3 deplacement, bool avecPointVise = false);
      /// Rotation de la caméra autour de sa position.
      void tournerXY(float rotationX, float rotationY, bool empecheInversion = true);
      /// Rotation de la position de la caméra autour de son point de visé.
      void orbiterXY(float rotationX, float rotationY);



      /// Positionner la caméra (appel à gluLookAt).
      void positionner() const;

	  virtual void animerAnimation();

	  virtual std::string obtenirNom() const;

	  void recalculerHaut();

	  Vecteur3 obtenirPosition();

   private:
      /// La position de la caméra.
      Vecteur3 position_;
      /// La position du point visé par la caméra.
      Vecteur3 pointVise_;
      /// La direction du haut de la caméra.
      Vecteur3 directionHaut_;
      /// La direction du haut du monde de la caméra.
      Vecteur3 directionHautMonde_;

   };




   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void Camera::assignerPosition(const Vecteur3& position)
   ///
   /// Cette fonction permet d'assigner la position de la caméra.
   ///
   /// @param[in] position : La nouvelle position de la caméra.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void Camera::assignerPosition(const Vecteur3& position)
   {
      position_ = position;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void Camera::assignerPointVise(const Vecteur3& pointVise)
   ///
   /// Cette fonction permet d'assigner le point de visé de la caméra.
   ///
   /// @param[in] pointVise : Le nouveau point de visé de la caméra.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
//    inline void Camera::assignerPointVise(Vecteur3* pointVise)
//    {
//       pointVise_ = pointVise;
//    }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void Camera::assignerPointVise(const Vecteur3& pointVise)
   ///
   /// Cette fonction permet d'assigner le point de visé de la caméra.
   ///
   /// @param[in] pointVise : Le nouveau point de visé de la caméra.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void Camera::assignerPointVise(const Vecteur3& pointVise)
   {
	   //delete pointVise_;
	   pointVise_ = pointVise;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void Camera::assignerDirectionHaut(const Vecteur3& directionHaut)
   ///
   /// Cette fonction permet d'assigner la direction du haut de la caméra.
   ///
   /// @param[in] directionHaut : La nouvelle direction du haut de la caméra.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void Camera::assignerDirectionHaut(const Vecteur3& directionHaut)
   {
      directionHaut_ = directionHaut;
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void Camera::assignerDirectionHautMonde(const Vecteur3& directionHautMonde)
   ///
   /// Cette fonction permet d'assigner la direction du haut du monde
   ///
   /// @param[in] directionHautMonde : La nouvelle direction du haut du monde.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void Camera::assignerDirectionHautMonde(const Vecteur3& directionHautMonde)
   {
	   directionHautMonde_ = directionHautMonde;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Vecteur3& Camera::obtenirPosition() const
   ///
   /// Cette fonction permet d'obtenir la position de la caméra.
   ///
   /// @return La position de la caméra.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline const Vecteur3& Camera::obtenirPosition() const
   {
      return position_;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Vecteur3& Camera::obtenirPointVise() const
   ///
   /// Cette fonction permet d'obtenir le point de visé de la caméra.
   ///
   /// @return Le point de visé de la caméra.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline const Vecteur3& Camera::obtenirPointVise() const
   {
      return pointVise_;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Vecteur3& Camera::obtenirDirectionHaut() const
   ///
   /// Cette fonction permet d'obtenir la direction du haut de la caméra.
   ///
   /// @return La direction du haut de la caméra.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline const Vecteur3& Camera::obtenirDirectionHaut() const
   {
      return directionHaut_;
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline const Vecteur3& Camera::obtenirDirectionHautMonde() const
   ///
   /// Cette fonction permet d'obtenir la direction du haut du monde.
   ///
   /// @return La direction du haut du monde
   ///
   ////////////////////////////////////////////////////////////////////////
   inline const Vecteur3& Camera::obtenirDirectionHautMonde() const
   {
	   return directionHautMonde_;
   }


} // Fin de l'espace de nom vue.


#endif // __UTILITAIRE_CAMERA_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
