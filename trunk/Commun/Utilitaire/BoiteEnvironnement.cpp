///////////////////////////////////////////////////////////////////////////
/// @file BoiteEnvironnement.cpp
/// @author Martin Bisson
/// @date 2007-05-28
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////
#include "BoiteEnvironnement.h"
#include "AideGL.h"

#include <windows.h>
#include "glew.h"

// Définition prise de l'en-tête de la version 1.2 d'OpenGL.
#define GL_CLAMP_TO_EDGE 0x812F


namespace utilitaire {
   
   
   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn BoiteEnvironnement::BoiteEnvironnement(const std::string& fichierXpos, const std::string& fichierXneg, const std::string& fichierYpos, const std::string& fichierYneg, const std::string& fichierZpos, const std::string& fichierZneg)
   ///
   /// Ce constructeur charge les 6 textures correspondant à chacune des faces
   /// de la boîte d'environnement.
   ///
   /// @param[in] fichierXpos : Le nom du fichier contenant l'image correspondant
   ///                          à l'axe des X positifs.
   /// @param[in] fichierXneg : Le nom du fichier contenant l'image correspondant
   ///                          à l'axe des X négatifs.
   /// @param[in] fichierYpos : Le nom du fichier contenant l'image correspondant
   ///                          à l'axe des X positifs.
   /// @param[in] fichierYneg : Le nom du fichier contenant l'image correspondant
   ///                          à l'axe des Y négatifs.
   /// @param[in] fichierZpos : Le nom du fichier contenant l'image correspondant
   ///                          à l'axe des Z positifs.
   /// @param[in] fichierZneg : Le nom du fichier contenant l'image correspondant
   ///                          à l'axe des Z négatifs.
   ///
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   BoiteEnvironnement::BoiteEnvironnement(
         const std::string& fichierXpos, const std::string& fichierXneg,
         const std::string& fichierYpos, const std::string& fichierYneg,
         const std::string& fichierZpos, const std::string& fichierZneg
         )
   {
      aidegl::glLoadTexture(fichierXpos, texturesEnvironnement_[0]);
      aidegl::glLoadTexture(fichierXneg, texturesEnvironnement_[1]);
      aidegl::glLoadTexture(fichierYpos, texturesEnvironnement_[2]);
      aidegl::glLoadTexture(fichierYneg, texturesEnvironnement_[3]);
      aidegl::glLoadTexture(fichierZpos, texturesEnvironnement_[4]);
      aidegl::glLoadTexture(fichierZneg, texturesEnvironnement_[5]);

      for (int i = 0; i < 6; ++i) {
         glBindTexture(GL_TEXTURE_2D, texturesEnvironnement_[i]);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }   
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn BoiteEnvironnement::~BoiteEnvironnement()
   ///
   /// Ce destructeur libère l'espace allouée à chacune des textures des faces
   /// de la boîte d'environnement.
   ///
   /// @return Aucune (destructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   BoiteEnvironnement::~BoiteEnvironnement()
   {
      glDeleteTextures(6, texturesEnvironnement_);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void BoiteEnvironnement::afficher(const Vecteur3& centre, double demiLargeur) const
   ///
   /// Cette fonction affiche tout simplement la boîte d'environnement.
   ///
   /// @param[in] centre      : La position du centre de la boîte pour
   ///                          l'affichage.
   /// @param[in] demiLargeur : La largeur de la moitié de la boîte pour
   ///                          l'affichage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void BoiteEnvironnement::afficher(const Vecteur3& centre,
                                     double demiLargeur) const
   {
      glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);

      // On désactive le test de profondeur car il n'est pas nécessaire puisque
      // la boîte est à l'infini.
      glDisable(GL_DEPTH_TEST);
      // On désactive l'écriture dans le tampon de profondeur pour s'assurer que
      // la boîte est véritablement "à l'infini".
      glDepthMask(GL_FALSE);
      // On désactive l'éclairage, car si un effet d'éclairage était souhaité, il
      // devrait déjà faire partie des textures de la boîte.  Sinon, comme la
      // boîte est à l'infini, la lumière "dynamique" ne devrait pas interagir
      // avec.
      glDisable(GL_LIGHTING);
      // On désactive le brouillard, car celui-ci permet de voir la nature
      // cubique de la boîte d'environnement étant donné que les points ne sont
      // pas tous à la même distance de l'usager.
      glDisable(GL_FOG);
      // On active les textures.
      glEnable(GL_TEXTURE_2D);

      glPushMatrix();
      glTranslated(centre[0], centre[1], centre[2]);
      glScaled(demiLargeur, demiLargeur, demiLargeur);

      // L'orientation des images est celle de CubeMapGen, de ATI.

      // Face des X positifs
      glBindTexture(GL_TEXTURE_2D, texturesEnvironnement_[0]);
      glBegin(GL_QUADS);
      {
         //glTexCoord2i(0, 0);
		  glTexCoord2i(1, 0);
         glVertex3i( 1,  1, -1);
         //glTexCoord2i(0, 1);
		 glTexCoord2i(0, 0);
         glVertex3i( 1, -1, -1); 
         //glTexCoord2i(1, 1);
		 glTexCoord2i(0, 1);
         glVertex3i( 1, -1,  1);
         //glTexCoord2i(1, 0);
		 glTexCoord2i(1, 1);
         glVertex3i( 1,  1,  1);
      }
      glEnd();

      // Face des X négatifs
      glBindTexture(GL_TEXTURE_2D, texturesEnvironnement_[1]);
      glBegin(GL_QUADS);
      {
         //glTexCoord2i(0, 0);
		 glTexCoord2i(0, 1);
         glVertex3i(-1,  1,  1);
         //glTexCoord2i(0, 1);
		 glTexCoord2i(1, 1);
         glVertex3i(-1, -1,  1);
         //glTexCoord2i(1, 1);
		 glTexCoord2i(1, 0);
         glVertex3i(-1, -1, -1); 
         //glTexCoord2i(1, 0);
		 glTexCoord2i(0, 0);
         glVertex3i(-1,  1, -1);
      }
      glEnd();

      // Face des Y positifs
      glBindTexture(GL_TEXTURE_2D, texturesEnvironnement_[2]);
      glBegin(GL_QUADS);
      {
         //glTexCoord2i(0, 0);
		  glTexCoord2i(1, 1);
         glVertex3i(-1,  1,  1);
         //glTexCoord2i(0, 1);
		 glTexCoord2i(1, 0);
         glVertex3i(-1,  1, -1); 
         //glTexCoord2i(1, 1);
		 glTexCoord2i(0, 0);
         glVertex3i( 1,  1, -1);
         //glTexCoord2i(1, 0);
		 glTexCoord2i(0, 1);
         glVertex3i( 1,  1,  1);
      }
      glEnd();

      // Face des Y négatifs
      glBindTexture(GL_TEXTURE_2D, texturesEnvironnement_[3]);
      glBegin(GL_QUADS);
      {
         glTexCoord2i(0, 0);
         glVertex3i(-1, -1, -1); 
         glTexCoord2i(0, 1);
         glVertex3i(-1, -1,  1);
         glTexCoord2i(1, 1);
         glVertex3i( 1, -1,  1);
         glTexCoord2i(1, 0);
         glVertex3i( 1, -1, -1);
      }
      glEnd();

      // LE SYSTÈME D'AXE EST INVERSÉ POUR LES Z, ce n'est donc pas un système
      // direct.

      // Face des Z positifs
      glBindTexture(GL_TEXTURE_2D, texturesEnvironnement_[5]);
      glBegin(GL_QUADS);
      {
         glTexCoord2i(0, 0);
         glVertex3i( 1,  1,  1);
         glTexCoord2i(0, 1);
         glVertex3i( 1, -1,  1);
         glTexCoord2i(1, 1);
         glVertex3i(-1, -1,  1);
         glTexCoord2i(1, 0);
         glVertex3i(-1,  1,  1);
      }
      glEnd();

      // Face des Z négatifs
      glBindTexture(GL_TEXTURE_2D, texturesEnvironnement_[4]);
      glBegin(GL_QUADS);
      {
         glTexCoord2i(0, 0);
         glVertex3i(-1,  1, -1); 
         glTexCoord2i(0, 1);
         glVertex3i(-1, -1, -1);
         glTexCoord2i(1, 1);
         glVertex3i( 1, -1, -1);
         glTexCoord2i(1, 0);
         glVertex3i( 1,  1, -1);
      }
      glEnd();

      glPopMatrix();
      glPopAttrib();
   }


} // Fin de l'espace de nom utilitaire.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
