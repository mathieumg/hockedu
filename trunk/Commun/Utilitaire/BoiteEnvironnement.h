///////////////////////////////////////////////////////////////////////////
/// @file BoiteEnvironnement.h
/// @author Martin Bisson
/// @date 2007-05-28
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_BOITEENVIRONNEMENT_H__
#define __UTILITAIRE_BOITEENVIRONNEMENT_H__


#include "Vecteur.h"
#include <string>


namespace utilitaire {
   
   
   ///////////////////////////////////////////////////////////////////////////
   /// @class BoiteEnvironnement
   /// @brief Classe repr�sentant une bo�te d'environnement ("skybox").
   ///
   ///        Elle s'occupe de charger 6 images du cube formant la bo�te.  Elle
   ///        utilise la convention de sens de CubeMapGen (de ATI), lorsque les
   ///        images sont export�es avec le mapping OpenGL (plut�t que DirectX).
   ///
   /// @author Martin Bisson
   /// @date 2007-05-28
   ///////////////////////////////////////////////////////////////////////////
   class BoiteEnvironnement
   {
   public:
      /// Constructeur � partir des noms des fichiers d'images de la bo�te.
      BoiteEnvironnement(
         const std::string& fichierXpos, const std::string& fichierXneg,
         const std::string& fichierYpos, const std::string& fichierYneg,
         const std::string& fichierZpos, const std::string& fichierZneg
         );
      /// Destructeur.
      ~BoiteEnvironnement();

      /// Affiche la bo�te d'environnement.
      void afficher(const Vecteur3& centre, float demiLargeur) const;


   private:
      /// Identificateurs des textures de la bo�te d'environnement.
      unsigned int texturesEnvironnement_[6];

   };


} // Fin de l'espace de nom utilitaire.


#endif // __UTILITAIRE_BOITEENVIRONNEMENT_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
