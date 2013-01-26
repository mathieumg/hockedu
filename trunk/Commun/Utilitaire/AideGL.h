///////////////////////////////////////////////////////////////////////////////
/// @file AideGL.h
/// @brief Ce fichier contient la déclaration de l'espace de nom aidegl.
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
#ifndef __UTILITAIRE_AIDEGL_H__
#define __UTILITAIRE_AIDEGL_H__

#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include <string>

#include "Vecteur.h"


/// Espace de nom contenant des fonctions utiles pour effectuer certaines
/// tâches OpenGL qui reviennent d'un projet à l'autre.
namespace aidegl {


   /// Crée un contexte OpenGL dans la fenêtre donnée.
   bool creerContexteGL(HWND hwnd, HDC& hDC, HGLRC& hRC);
   /// Détruit un contexte OpenGL.
   bool detruireContexteGL(HWND hwnd, HDC hDC, HGLRC hRC);

   /// Initialise le mode de rendu du rectangle élastique.
   void initialiserRectangleElastique(
      const Vecteur2i& point,
      unsigned short   patron = 0x3333,
      int              facteur = 1
      );
   /// Met à jour le rectangle élastique (suite à un déplacement).
   void mettreAJourRectangleElastique(
      const Vecteur2i& pointAncrage,
      const Vecteur2i& pointAvant,
      const Vecteur2i& pointApres
      );

   /// Dessin direct d'un rectangle elastique
   void dessinerRectangleElastique(const Vecteur2i& point1, const Vecteur2i& point2);

   /// Termine le mode de rendu du rectangle élastique.
   void terminerRectangleElastique(
      const Vecteur2i& pointAncrage,
      const Vecteur2i& pointFinal
      );


   /// Affiche un rectangle subdivisé par un nombre de subdivision.
   void afficherRectangle(
      const Vecteur3& point1, const Vecteur3& point2,
      const Vecteur3& point3, const Vecteur3& point4,
      int divisionS, int divisionT
      );

   /// Affiche un rectangle subdivisé d'un intervalle.
   void afficherRectangleIntervalle(
      const Vecteur3& point1, const Vecteur3& point2,
      const Vecteur3& point3, const Vecteur3& point4,
      float deltaS, float deltaT
      );


   /// Drapeau pour afficher le nom du code d'erreur.
   static const int AFFICHE_ERREUR_CODE           = (1 << 0);
   /// Drapeau pour afficher la chaîne de gluErrorString.
   static const int AFFICHE_ERREUR_GLUERRORSTRING = (1 << 1);
   /// Drapeau pour afficher la description du code d'erreur.
   static const int AFFICHE_ERREUR_DESCRIPTION    = (1 << 2);
   /// Drapeau pour afficher toutes les informations.
   static const int AFFICHE_ERREUR_TOUT           =
      AFFICHE_ERREUR_CODE           |
      AFFICHE_ERREUR_GLUERRORSTRING |
      AFFICHE_ERREUR_DESCRIPTION;
   /// Drapeau pour afficher même s'il n'y a pas d'erreur.
   static const int AFFICHE_ERREUR_AUCUNE         = (1 << 3);

   /// Retourne une chaîne correspondant au code d'erreur OpenGL.
   std::string obtenirMessageErreur(
      GLenum codeErreur, int drapeau = AFFICHE_ERREUR_TOUT
      );

   /// Vérifie s'il y a une erreur OpenGL.
   void verifierErreurOpenGL(int drapeau = AFFICHE_ERREUR_TOUT);


   /// Charge une texture OpenGL à partir d'un fichier image.
   bool glLoadTexture(const std::string& nomFichier, unsigned int& idTexture, bool genererTexture=true);

   /// Crée une texture à partir des pixels en mémoire.
   void glCreateTexture(
      unsigned char* data, int x, int y, int bpp, int pitch, unsigned int& ID, bool genererTexture=true
      );


} // Fin de l'espace de nom aidegl


#endif // __UTILITAIRE_AIDEGL_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
