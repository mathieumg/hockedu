////////////////////////////////////////////////////////////////////////////////////
/// @file Projection.cpp
/// @author DGI
/// @date 2006-12-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
////////////////////////////////////////////////////////////////////////////////////
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include "Projection.h"


namespace vue {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn Projection::Projection(int xMinCloture, int xMaxCloture, int yMinCloture, int yMaxCloture, float zAvant,  float zArriere, float zoomInMax, float zoomOutMax, float incrementZoom, bool estPerspective)
   ///
   /// Constructeur d'une projection.  Ne fait qu'assigner les variables
   /// membres.
   ///
   /// @param[in] xMinCloture    : coordonn�e minimale en @a x de la cl�ture.
   /// @param[in] xMaxCloture    : coordonn�e maximale en @a x de la cl�ture.
   /// @param[in] yMinCloture    : coordonn�e minimale en @a y de la cl�ture.
   /// @param[in] yMaxCloture    : coordonn�e maximale en @a y de la cl�ture.
   /// @param[in] zAvant         : distance du plan avant (en @a z).
   /// @param[in] zArriere       : distance du plan arri�re (en @a z).
   /// @param[in] zoomInMax      : facteur de zoom in maximal.
   /// @param[in] zoomOutMax     : facteur de zoom out maximal.
   /// @param[in] incrementZoom  : distance du plan arri�re (en @a z).
   /// @param[in] estPerspective : vrai si la projection est perspective.
   /// 
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   Projection::Projection(int xMinCloture, int xMaxCloture,
                          int yMinCloture, int yMaxCloture,
                          float zAvant,  float zArriere,
                          float zoomInMax, float zoomOutMax,
                          float incrementZoom, bool estPerspective) :
      xMinCloture_(xMinCloture),
      xMaxCloture_(xMaxCloture),
      yMinCloture_(yMinCloture),
      yMaxCloture_(yMaxCloture),
      zAvant_(zAvant),
      zArriere_(zArriere),
      zoomInMax_(zoomInMax),
      zoomOutMax_(zoomOutMax),
      incrementZoom_(incrementZoom),
      estPerspective_(estPerspective)
   {
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Projection::mettreAJourCloture() const
   ///
   /// Sp�cifie la cl�ture de la fen�tre � l'aide de la fonction glViewport()
   /// dans la machine � �tats d'OpenGL.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void Projection::mettreAJourCloture() const
   {
      glViewport( xMinCloture_, 
                  yMinCloture_,
                  GLint( xMaxCloture_ - xMinCloture_ ), 
                  GLint( yMaxCloture_ - yMinCloture_ ) );
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Projection::mettreAJourCloture( Vecteur2i coinMin, Vecteur2i coinMax )
   ///
   /// Sp�cifie la cl�ture de la fen�tre � l'aide de la fonction glViewport()
   /// dans la machine � �tats d'OpenGL.
   ///
   /// @param[in]	quelViewPort	:	index du viewport
   /// @param[in]	nbTotalViewport :	nb de viewports
   /// 
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void Projection::mettreAJourCloture( int quelViewPort, int nbTotalViewport/* = 2*/ )
   {
	   
	   int modificateur = quelViewPort-1;
	   int largeurPartie = (xMaxCloture_-xMinCloture_)/nbTotalViewport;
	   int hauteurPartie = (yMaxCloture_-yMinCloture_);

	   glViewport( xMinCloture_+modificateur*largeurPartie, 
		   yMinCloture_,
		   GLint( largeurPartie ), 
		   GLint( hauteurPartie ) );

	   
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void Projection::mettreAJourProjection() const
   ///
   /// Sp�cifie la matrice de projection dans la machine � �tats d'OpenGL.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   void Projection::mettreAJourProjection() const
   {
      // Sauvegarde du mode courant de matrice.
      //GLint mode;
      //glGetIntegerv(GL_MATRIX_MODE, &mode);

      //glMatrixMode(GL_PROJECTION);
      //glLoadIdentity();

      // Application de la projection.
      appliquer();

      //glMatrixMode(mode);
   }






}; // Fin du namespace vue.


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
