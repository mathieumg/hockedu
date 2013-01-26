///////////////////////////////////////////////////////////////////////////////
/// @file AideGL.cpp
/// @brief Ce fichier contient l'implantation de l'espace de nom aidegl.
///
/// Il contient les déclarations de fonctions utiles pour effectuer certaines
/// tâches OpenGL qui reviennent d'un projet à l'autre.
///
/// @author Martin Bisson
/// @date 2007-01-20
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "AideGL.h"
#include "Utilitaire.h"

#ifdef WIN32
#define _WINSOCKAPI_
#include <windows.h>
// Nous pouvons charger la librairie OpenGL (GLU) en indiquant cette directive
// au précompilateur
#pragma comment ( lib, "glu32.lib" )
#endif

#include "FreeImage.h"
#include "glew.h"
#include <GL/glu.h>
#include <iostream>
#include <cassert>


namespace aidegl {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool creerContexteGL(HWND hWnd, HDC& hDC, HGLRC& hRC)
   ///
   /// Cette fonction crée un contexte OpenGL pour la fenêtre donnée.
   ///
   /// @param[in] hWnd : Poignée ("handle") vers la fenêtre dans laquelle on veut créer un contexte.
   /// @param[out] hDC : Poignée ("handle") vers le "Device context" de la fenêtre.
   /// @param[out] hRC : Poignée ("handle") vers le contexte OpenGL créé.
   ///
   /// @return Vrai en cas de succès.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool creerContexteGL(HWND hWnd, HDC& hDC, HGLRC& hRC)
   {
      // Obtention du "Device context"
      hDC = ::GetDC(hWnd);

      if (hDC == 0) {
         return false;
      }

      // On choisit le format pour le "Device context"
      PIXELFORMATDESCRIPTOR pfd;
      ::ZeroMemory( &pfd, sizeof( pfd ) );
      pfd.nSize = sizeof( pfd );
      pfd.nVersion = 1;
      pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
                    PFD_SUPPORT_OPENGL | 
                    PFD_DOUBLEBUFFER;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 24;
      pfd.cDepthBits = 16;
      pfd.cStencilBits = 1;
      pfd.iLayerType = PFD_MAIN_PLANE;

      int format = ::ChoosePixelFormat( hDC, &pfd );
      if (format == 0) {
         ::ReleaseDC(hWnd, hDC);
         hDC = 0;
         return false;
      }

      if (!::SetPixelFormat( hDC, format, &pfd )) {
         ::ReleaseDC(hWnd, hDC);
         hDC = 0;
         return false;
      }


      // Création du contexte
      hRC = ::wglCreateContext( hDC );
      if (hRC == 0) {
         ::ReleaseDC(hWnd, hDC);
         hDC = 0;
         return false;
      }

      // Activation du contexte
      if (!::wglMakeCurrent( hDC, hRC )) {
         ::wglDeleteContext( hRC );
         hRC = 0;
         ::ReleaseDC(hWnd, hDC);
         hDC = 0;
         return false;
      }

      // Tout s'est bien passé
      return true;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool detruireContexteGL(HWND hWnd, HDC hDC, HGLRC hRC)
   ///
   /// Cette fonction détruit proprement un contexte OpenGL.
   ///
   /// @param[in] hWnd : Poignée ("handle") vers la fenêtre du contexte à détruire.
   /// @param[in] hDC  : Poignée ("handle") vers le "Device context" de la fenêtre.
   /// @param[in] hRC  : Poignée ("handle") vers le contexte OpenGL à détruire.
   ///
   /// @return Vrai en cas de succès.
   ///
   ////////////////////////////////////////////////////////////////////////
   bool detruireContexteGL(HWND hWnd, HDC hDC, HGLRC hRC)
   {
      bool succes = true;

      if (!::wglMakeCurrent( 0, 0 ))
         succes = false;

      if (!::wglDeleteContext( hRC ))
         succes = false;
      
      if (::ReleaseDC( hWnd, hDC ) == 0)
         succes = false;

      return succes;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void initialiserRectangleElastique(const Vecteur2i& point, unsigned short patron, int facteur)
   ///
   /// Cette fonction initialise le mode de rendu du rectangle élastique en
   /// mode XOR en sauvegardant l'état de la machine OpenGL et en la modifiant
   /// pour que le rendu soit correct.
   ///
   /// @param[in] point   : Point initial de localisation du rectangle.
   /// @param[in] patron  : Patron de pointillé pour les lignes du rectangle.
   /// @param[in] facteur : Facteur du pointillé pour les lignes du rectangle.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void initialiserRectangleElastique(
      const Vecteur2i& point,
      unsigned short   patron, //= 0x3333
      int              facteur //= 1
      )
   {
      // On sauvegarde les attributs de traçage.
      glPushAttrib( GL_LINE_BIT |
                    GL_CURRENT_BIT |
                    GL_COLOR_BUFFER_BIT |
                    GL_DEPTH_BUFFER_BIT |
                    GL_LIGHTING_BIT );

      // On dessine uniquement dans ce qui est déjà à l'écran.
      glDrawBuffer( GL_FRONT );

      // On désactive l'éclairage et le test de profondeur.
      glDisable( GL_DEPTH_TEST );
      glDisable( GL_LIGHTING );
	  glDisable( GL_TEXTURE_2D );

      // On sauvegarde les matrices de transformation.
      glMatrixMode( GL_MODELVIEW );
      glPushMatrix();
      glLoadIdentity();
      glMatrixMode( GL_PROJECTION );
      glPushMatrix();

      // On initialise la matrice de projection: 1 pixel = 1 unité virtuelle.
      GLint Cloture[4];
      glGetIntegerv( GL_VIEWPORT, Cloture);
      glLoadIdentity() ;
      gluOrtho2D( 0.0, (GLfloat) Cloture[2], (GLfloat) Cloture[3], 0.0 );

      // On trace le rectangle en pointillés.
      glEnable( GL_LINE_STIPPLE );
      glLineStipple( 1, 0x3333 );

      // On choisit la couleur blanche pour que le XOR soit une inversion des
      // bits de la couleur déjà présente dans la mémoire de trame.
      glColor3f( 1.0, 1.0, 1.0 );

      // On active le mode XOR.
      glEnable( GL_COLOR_LOGIC_OP );
      glLogicOp( GL_XOR );

      // On trace le rectangle en un seul point.
      glBegin(GL_LINE_LOOP);
      {
         glVertex2i(point[0], point[1]);
         glVertex2i(point[0], point[1]);
         glVertex2i(point[0], point[1]);
         glVertex2i(point[0], point[1]);
      }
      glEnd();

      // On veut que le rectangle soit immédiatement visible.
      glFlush();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void mettreAJourRectangleElastique(const Vecteur2i& pointAncrage, const Vecteur2i& pointAvant, const Vecteur2i& pointApres)
   ///
   /// Cette fonction met à jour la position du rectangle élastique en
   /// effaçant le rectangle précédent pour le remplacer par un nouveau.
   ///
   /// @param[in] pointAncrage : Point initial de localisation du rectangle.
   /// @param[in] pointAvant   : Autre coin du rectangle avant la mise à jour.
   /// @param[in] pointApres   : Autre coin du rectangle après la mise à jour.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void mettreAJourRectangleElastique(
      const Vecteur2i& pointAncrage,
      const Vecteur2i& pointAvant,
      const Vecteur2i& pointApres
      )
   {
      // On trace l'ancien rectangle pour l'effacer et restaurer la couleur qui
      // était là avant (grâce au XOR).
      glBegin(GL_LINE_LOOP);
      {
         glVertex2i(pointAncrage[0] , pointAncrage[1]);
         glVertex2i(pointAncrage[0] , pointAvant[1]);
         glVertex2i(pointAvant[0]   , pointAvant[1]);
         glVertex2i(pointAvant[0]   , pointAncrage[1]);
      }
      glEnd();

      // On trace le nouveau rectangle.
      glBegin(GL_LINE_LOOP);
      {
         glVertex2i(pointAncrage[0] , pointAncrage[1]);
         glVertex2i(pointAncrage[0] , pointApres[1]);
         glVertex2i(pointApres[0]   , pointApres[1]);
         glVertex2i(pointApres[0]   , pointAncrage[1]);
      }
      glEnd();

      // On s'arrange pour que le nouveau rectangle soit immédiatement visible.
      glFlush();
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void aidegl::dessinerRectangleElastique( const Vecteur2i& point1, const Vecteur2i& point2 )
   ///
   /// Dessin immédiat d'un rectangle elastique
   ///
   /// @param[in] const Vecteur2i & point1 : premier point du rectangle elastique
   /// @param[in] const Vecteur2i & point2 : deuxième point du rectangle elastique
   ///
   /// @return void
   ///
   ////////////////////////////////////////////////////////////////////////
   void dessinerRectangleElastique(const Vecteur2i& point1, const Vecteur2i& point2)
   {
	   // On sauvegarde les attributs de traçage.
	   glPushAttrib( GL_LINE_BIT |
		   GL_CURRENT_BIT |
		   GL_COLOR_BUFFER_BIT |
		   GL_DEPTH_BUFFER_BIT |
		   GL_LIGHTING_BIT );

	   // On dessine uniquement dans ce qui est déjà à l'écran.
	   //glDrawBuffer( GL_FRONT );

	   // On désactive l'éclairage et le test de profondeur.
	   glDisable( GL_DEPTH_TEST );
	   glDisable( GL_LIGHTING );
	   glDisable( GL_TEXTURE_2D );

	   // On sauvegarde les matrices de transformation.
	   glMatrixMode( GL_MODELVIEW );
	   glPushMatrix();
	   glLoadIdentity();
	   glMatrixMode( GL_PROJECTION );
	   glPushMatrix();

	   // On initialise la matrice de projection: 1 pixel = 1 unité virtuelle.
	   GLint Cloture[4];
	   glGetIntegerv( GL_VIEWPORT, Cloture);
	   glLoadIdentity() ;
	   gluOrtho2D( 0.0, (GLfloat) Cloture[2], (GLfloat) Cloture[3], 0.0 );

	   // On trace le rectangle en pointillés.
	   glEnable( GL_LINE_STIPPLE );
	   glLineStipple( 1, 0x3333 );

	   // On choisit la couleur blanche pour que le XOR soit une inversion des
	   // bits de la couleur déjà présente dans la mémoire de trame.
	   glColor3f( 1.0, 1.0, 1.0 );

	   // On active le mode XOR.
	   glEnable( GL_COLOR_LOGIC_OP );
	   glLogicOp( GL_XOR );

	   // On trace l'ancien rectangle pour l'effacer et restaurer la couleur qui
	   // était là avant (grâce au XOR).
	   glBegin(GL_LINE_LOOP);
	   {
		   glVertex2i(point1[0] , point1[1]);
		   glVertex2i(point1[0] , point2[1]);
		   glVertex2i(point2[0]   , point2[1]);
		   glVertex2i(point2[0]   , point1[1]);
	   }
	   glEnd();

	   // On rétablit les attributs initiaux.
	   glPopAttrib();

	   // On rétablit la transformation initiale.
	   glMatrixMode( GL_PROJECTION );
	   glPopMatrix();
	   glMatrixMode( GL_MODELVIEW );
	   glPopMatrix();
	   glDisable(GL_COLOR_LOGIC_OP);
	   // On s'arrange pour que le nouveau rectangle soit immédiatement visible.
	   glFlush();

	   // On se remet à dessiner dans le tampon arrière.
	   //glDrawBuffer(GL_BACK);
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void terminerRectangleElastique(const Vecteur2i& pointAncrage, const Vecteur2i& pointFinal)
   ///
   /// Cette fonction termine le mode de rendu du rectangle élastique en
   /// restaurant l'état de la machine OpenGL à ce qu'il était.
   ///
   /// @param[in] pointAncrage : Point initial de localisation du rectangle.
   /// @param[in] pointFinal   : Point final de localisation du rectangle.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void terminerRectangleElastique(
      const Vecteur2i& pointAncrage,
      const Vecteur2i& pointFinal
      )
   {
      // On trace l'ancien rectangle pour l'effacer et restaurer la couleur qui
      // était là avant (grâce au XOR).
      glBegin(GL_LINE_LOOP);
      {
         glVertex2i(pointAncrage[0] , pointAncrage[1]);
         glVertex2i(pointAncrage[0] , pointFinal[1]);
         glVertex2i(pointFinal[0]   , pointFinal[1]);
         glVertex2i(pointFinal[0]   , pointAncrage[1]);
      }
      glEnd();

      // On rétablit les attributs initiaux.
      glPopAttrib();

      // On rétablit la transformation initiale.
      glMatrixMode( GL_PROJECTION );
      glPopMatrix();
      glMatrixMode( GL_MODELVIEW );
      glPopMatrix();
	  glDisable(GL_COLOR_LOGIC_OP);
      // On s'arrange pour que le nouveau rectangle soit immédiatement visible.
      glFlush();

      // On se remet à dessiner dans le tampon arrière.
      glDrawBuffer(GL_BACK);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void afficherRectangle(const Vecteur3& point1, const Vecteur3& point2, const Vecteur3& point3, const Vecteur3& point4, int divisionS, int divisionT)
   ///
   /// Cette fonction affiche un rectangle subdivisé en grille d'un certain
   /// nombre de points.
   ///
   /// @param[in] point1    : Premier point du rectangle.
   /// @param[in] point2    : Second point du rectangle.
   /// @param[in] point3    : Troisième point du rectangle.
   /// @param[in] point4    : Quatrième point du rectangle.
   /// @param[in] divisionS : Nombre de divisions dans une dimension.
   /// @param[in] divisionT : Nombre de divisions dans l'autre dimension.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void afficherRectangle(
      const Vecteur3& point1, const Vecteur3& point2,
      const Vecteur3& point3, const Vecteur3& point4,
      int divisionS, int divisionT
      )
   {
      const Vecteur3 dirS1 = point2 - point1;
      const Vecteur3 dirS2 = point3 - point4;

      Vecteur3 ancienInter1 = point1;
      Vecteur3 ancienDirT   = point4 - point1;
      for (int i = 1; i < divisionS; ++i) {
         const float factorS = i / (float) (divisionS - 1);
         const Vecteur3 inter1 = point1 + dirS1 * factorS;
         const Vecteur3 inter2 = point4 + dirS2 * factorS;
         const Vecteur3 dirT   = inter2 - inter1;

         glBegin(GL_QUAD_STRIP);
         for (int j = 0; j < divisionT; ++j) {
            const float factorT = j / (float) (divisionT - 1);
            const Vecteur3 vertex1 = ancienInter1 + ancienDirT * factorT;
            const Vecteur3 vertex2 = inter1 + dirT * factorT;

            glVertex3d(vertex1[0], vertex1[1], vertex1[2]);
            glVertex3d(vertex2[0], vertex2[1], vertex2[2]);
         }
         glEnd();

         ancienInter1 = inter1;
         ancienDirT   = dirT;
      }
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void afficherRectangle(const Vecteur3& point1, const Vecteur3& point2, const Vecteur3& point3, const Vecteur3& point4, double deltaS, double deltaT)
   ///
   /// Cette fonction affiche un rectangle subdivisé en grille d'une certaine
   /// distance maximal entre chacun des points successifs.
   ///
   /// @param[in] point1 : Premier point du rectangle.
   /// @param[in] point2 : Second point du rectangle.
   /// @param[in] point3 : Troisième point du rectangle.
   /// @param[in] point4 : Quatrième point du rectangle.
   /// @param[in] deltaS : Distance maximale dans une dimension.
   /// @param[in] deltaT : Distance maximale dans l'autre dimension.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void afficherRectangleIntervalle(
      const Vecteur3& point1, const Vecteur3& point2,
      const Vecteur3& point3, const Vecteur3& point4,
      double deltaS, double deltaT
      )
   {
      int nS1 = (int) (0.5f + (point2-point1).norme() / deltaS);
      int nS2 = (int) (0.5f + (point3-point4).norme() / deltaS);
      int nT1 = (int) (0.5f + (point4-point1).norme() / deltaT);
      int nT2 = (int) (0.5f + (point2-point3).norme() / deltaT);

      int S = nS1 > nS2 ? nS1 : nS2;
      if (S < 2)
         S = 2;
      int T = nT1 > nT2 ? nT1 : nT2;
      if (T < 2)
         T = 2;

      afficherRectangle(point1, point2, point3, point4, S, T);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn std::string obtenirMessageErreur(GLenum codeErreur, int drapeau)
   ///
   /// Cette fonction retourne une chaîne détaillant l'erreur identifiée par
   /// le code d'erreur passé en paramètre.  La chaîne retournée contient
   /// d'abord le nom de la constante identifiant le code d'erreur, suivi
   /// du message de la fonction gluErrorString(), et une explication
   /// provenant du "man page" de glGetError().
   ///
   /// @param[in] codeErreur : Code d'erreur.
   /// @param[in] drapeau    : Information désirée dans le message d'erreur.
   ///
   /// @return Chaîne contenant le message d'erreur.
   ///
   ////////////////////////////////////////////////////////////////////////
   std::string obtenirMessageErreur(
      GLenum codeErreur, int drapeau //= AFFICHE_ERREUR_TOUT
      )
   {
      std::string chaineCode;
      std::string message;

      // Il sera possible d'avoir un "map" au lieu de ce switch, qui n'est pas
      // très performant pour les cas où on ne veut pas le message par exemple.
      switch (codeErreur) {
         case GL_NO_ERROR:
            chaineCode = "GL_NO_ERROR";
            message = "No error has been recorded.  The value of this "
                      "symbolic constant is guaranteed to be 0.";
            break;

         case GL_INVALID_ENUM:
            chaineCode = "GL_INVALID_ENUM";
            message = "An unacceptable value is specified for an enumerated "
                      "argument.  The offending command is ignored, and has "
                      "no other side effect than to set the error flag.";
            break;

         case GL_INVALID_VALUE:
            chaineCode = "GL_INVALID_VALUE";
            message = "A numeric argument is out of range.  The offending "
                      "command is ignored, and has no other side effect than "
                      "to set the error flag.";
            break;

         case GL_INVALID_OPERATION:
            chaineCode = "GL_INVALID_OPERATION";
            message = "The specified operation is not allowed in the current "
                      "state.  The offending command is ignored, and has no "
                      "other side effect than to set the error flag.";
            break;

         case GL_STACK_OVERFLOW:
            chaineCode = "GL_STACK_OVERFLOW";
            message = "This command would cause a stack overflow.  The "
                      "offending command is ignored, and has no other side "
                      "effect than to set the error flag.";
            break;

         case GL_STACK_UNDERFLOW:
            chaineCode = "GL_STACK_UNDERFLOW";
            message = "This command would cause a stack underflow.  The "
                      "offending command is ignored, and has no other side "
                      "effect than to set the error flag.";
            break;

         case GL_OUT_OF_MEMORY:
            chaineCode = "GL_OUT_OF_MEMORY";
            message = "There is not enough memory left to execute the "
                      "command.  The state of the GL is undefined, except "
                      "for the state of the error flags, after this error is "
                      "recorded.";
            break;

         default:
            chaineCode = "GL_????????";
            message = "Code d'erreur non identifié.";
            break;
      };

      // Concatenation des messages
      bool aPrecedent = false;
      std::string retour;

      if (drapeau & AFFICHE_ERREUR_CODE) {
         retour = chaineCode;
         aPrecedent = true;
      }

      if (drapeau & AFFICHE_ERREUR_GLUERRORSTRING) {
         if (aPrecedent)
            retour += " , ";
         retour += reinterpret_cast<const char*> (
            gluErrorString(codeErreur)
            );
         aPrecedent = true;
      }

      if (drapeau & AFFICHE_ERREUR_DESCRIPTION) {
         if (aPrecedent)
            retour += " , ";
         retour += message;
      }

      return retour;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void verifierErreurOpenGL(int drapeau)
   ///
   /// Cette fonction vérifie s'il y a une erreur OpenGL, et si c'est le cas
   /// affiche un message d'erreur correspondant à l'erreur.
   ///
   /// @param[in] drapeau : Information désirée dans le message d'erreur.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void verifierErreurOpenGL(
      int drapeau //= AFFICHE_ERREUR_TOUT
      )
   {
      GLenum codeErreur = glGetError();

      if ( codeErreur || (drapeau & AFFICHE_ERREUR_AUCUNE) )
         std::cerr << obtenirMessageErreur(codeErreur, drapeau) << std::endl;
   }


   ///////////////////////////////////////////////////////////////////////////
   ///
   /// @fn bool glLoadTexture(const std::string& nomFichier, unsigned int& idTexture,
	///                        bool genererTexture)
   ///
   /// Cette fonction crée une texture OpenGL à partir d'une image contenu
   /// dans un fichier.  FreeImage est utilisée pour lire l'image, donc tous
   /// les formats reconnues par cette librairie devraient être supportés.
   ///
   /// @param[in]  nomFichier     : Le nom du fichier image à charger.
   /// @param[out] idTexture      : L'identificateur de la texture créée.
	/// @param[in]  genererTexture : Doit-on demander à OpenGL de générer un numéro
	///										de texture au préalable?
   ///
   /// @return Vrai si le chargement a réussi, faux autrement.
   ///
   ///////////////////////////////////////////////////////////////////////////
   bool glLoadTexture(const std::string& nomFichier, unsigned int& idTexture, bool genererTexture)
   {
      // Ce code de lecture générique d'un fichier provient de la
      // documentation de FreeImage
      FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
      // check the file signature and deduce its format
      // (the second argument is currently not used by FreeImage)
      format = FreeImage_GetFileType(nomFichier.c_str(), 0);
      if(format == FIF_UNKNOWN) {
         // no signature ?
         // try to guess the file format from the file extension
         format = FreeImage_GetFIFFromFilename(nomFichier.c_str());
      }
      // check that the plugin has reading capabilities ...
      if((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format)) {
         utilitaire::afficherErreur(
            std::string("Format du fichier image \"") +
            std::string(nomFichier.c_str()) + std::string("\" non supporté")
            );
         return false;
      }
      // ok, let's load the file
      FIBITMAP* dib = FreeImage_Load(format, nomFichier.c_str(), 0);

      if (dib == 0) {
         utilitaire::afficherErreur(
            std::string("Erreur à la lecture du fichier \"") +
            std::string(nomFichier.c_str()) + std::string("\"")
            );
         return false;
      }

      FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);
      if (dib32 == 0) {
         utilitaire::afficherErreur(
            std::string("Incapable de convertir le fichier \"") +
            std::string(nomFichier.c_str()) + std::string("\" en 32 bpp.")
            );
         FreeImage_Unload(dib);
         return false;
      }

		int pitch = FreeImage_GetPitch(dib32);

      glCreateTexture(
         FreeImage_GetBits(dib32),
         FreeImage_GetWidth(dib32),
         FreeImage_GetHeight(dib32),
         FreeImage_GetBPP(dib32),
         FreeImage_GetPitch(dib32),
         idTexture,
			genererTexture
         );

      FreeImage_Unload(dib32);
      FreeImage_Unload(dib);

      return true;
   }


   ///////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void glCreateTexture(unsigned char* data, int x, int y, int bpp, int pitch,
	///                          unsigned int& ID, bool genererTexture)
   ///
   /// Cette fonction crée une texture à partir des pixels en mémoire.
   ///
   /// @param[in]  data           : Le tableau des valeurs des pixels.
   /// @param[in]  x              : La largeur de l'image.
   /// @param[in]  y              : La hauteur de l'image.
   /// @param[in]  bpp            : Le nombre de bits par pixels.
   /// @param[in]  pitch          : La largeur en mémoire d'une rangée de l'image.
   /// @param[out] ID             : L'identificateur de la texture créée.
	/// @param[in]  genererTexture : Doit-on demander à OpenGL de générer un numéro
	///										de texture au préalable?
   ///
   /// @return Aucune.
   ///
   ///////////////////////////////////////////////////////////////////////////
   void glCreateTexture(
      unsigned char* data, int x, int y, int bpp, int pitch, unsigned int& ID, bool genererTexture
      )
   {
		if (genererTexture) {
			glGenTextures( 1, &ID );
		}
      glBindTexture( GL_TEXTURE_2D, ID );

      // Le pitch est ignoré pour le moment, car on le suppose toujours égal à
      // la largeur, mais il pourrait être utilisé pour produire une
      // implantation plus robuste.
      if (pitch != x*bpp/8) {
         utilitaire::afficherErreur(
            "L'implantation ne supporte pas un \"pitch\" différent de la largeur."
         );
         return;
      }

      // Tel que spécifié dans la documentation de FreeImage (voir Pixel Access 
      // Functions, page 29 du PDF FreeImage 3.9.0 documentation), la 
      // disposition des composantes des couleurs est dépendante de 
      // l'architecture.  Sur little endian, le format BGR (plutôt que RGB) est 
      // utilisé.  On utilise donc l'extension GL_EXT_bgra (on pourrait vérifie 
      // à l'exécution que l'extension est présente)
      if ( bpp == 24 )
         gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
      else if ( bpp == 32 )
         gluBuild2DMipmaps(GL_TEXTURE_2D, 4, x, y, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
      else
         utilitaire::afficherErreur(
            "Incapable de lire une image qui n'a pas 24 ou 32 bits par pixels."
         );

      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   }


} // Fin de l'espace de nom aidegl.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
