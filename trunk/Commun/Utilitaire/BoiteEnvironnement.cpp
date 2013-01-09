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

// D�finition prise de l'en-t�te de la version 1.2 d'OpenGL.
#define GL_CLAMP_TO_EDGE 0x812F


namespace utilitaire {
   
   
   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn BoiteEnvironnement::BoiteEnvironnement(const std::string& fichierXpos, const std::string& fichierXneg, const std::string& fichierYpos, const std::string& fichierYneg, const std::string& fichierZpos, const std::string& fichierZneg)
   ///
   /// Ce constructeur charge les 6 textures correspondant � chacune des faces
   /// de la bo�te d'environnement.
   ///
   /// @param[in] fichierXpos : Le nom du fichier contenant l'image correspondant
   ///                          � l'axe des X positifs.
   /// @param[in] fichierXneg : Le nom du fichier contenant l'image correspondant
   ///                          � l'axe des X n�gatifs.
   /// @param[in] fichierYpos : Le nom du fichier contenant l'image correspondant
   ///                          � l'axe des X positifs.
   /// @param[in] fichierYneg : Le nom du fichier contenant l'image correspondant
   ///                          � l'axe des Y n�gatifs.
   /// @param[in] fichierZpos : Le nom du fichier contenant l'image correspondant
   ///                          � l'axe des Z positifs.
   /// @param[in] fichierZneg : Le nom du fichier contenant l'image correspondant
   ///                          � l'axe des Z n�gatifs.
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
   /// Ce destructeur lib�re l'espace allou�e � chacune des textures des faces
   /// de la bo�te d'environnement.
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
   /// Cette fonction affiche tout simplement la bo�te d'environnement.
   ///
   /// @param[in] centre      : La position du centre de la bo�te pour
   ///                          l'affichage.
   /// @param[in] demiLargeur : La largeur de la moiti� de la bo�te pour
   ///                          l'affichage.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void BoiteEnvironnement::afficher(const Vecteur3& centre,
                                     double demiLargeur) const
   {
      glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);

      // On d�sactive le test de profondeur car il n'est pas n�cessaire puisque
      // la bo�te est � l'infini.
      glDisable(GL_DEPTH_TEST);
      // On d�sactive l'�criture dans le tampon de profondeur pour s'assurer que
      // la bo�te est v�ritablement "� l'infini".
      glDepthMask(GL_FALSE);
      // On d�sactive l'�clairage, car si un effet d'�clairage �tait souhait�, il
      // devrait d�j� faire partie des textures de la bo�te.  Sinon, comme la
      // bo�te est � l'infini, la lumi�re "dynamique" ne devrait pas interagir
      // avec.
      glDisable(GL_LIGHTING);
      // On d�sactive le brouillard, car celui-ci permet de voir la nature
      // cubique de la bo�te d'environnement �tant donn� que les points ne sont
      // pas tous � la m�me distance de l'usager.
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

      // Face des X n�gatifs
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

      // Face des Y n�gatifs
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

      // LE SYST�ME D'AXE EST INVERS� POUR LES Z, ce n'est donc pas un syst�me
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

      // Face des Z n�gatifs
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
