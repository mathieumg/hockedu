///////////////////////////////////////////////////////////////////////////////
/// @file LignePointillee.h
/// @author Martin Bisson
/// @date 2007-01-28
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_LIGNEPOINTILLEE_H__
#define __UTILITAIRE_LIGNEPOINTILLEE_H__


#include "glew.h"
#include "Vecteur.h"


namespace aidegl {


   ///////////////////////////////////////////////////////////////////////////
   /// @class LignePointillee
   /// @brief Classe qui gère l'affichage de lignes pointillées.
   ///
   /// @author Martin Bisson
   /// @date 2007-01-28
   ///////////////////////////////////////////////////////////////////////////
   class LignePointillee
   {
   public:
      /// Constructeur par defaut.
      LignePointillee();

      /// Début du rendu de lignes.
      void commencer() const;

      /// Fin du rendu de lignes.
      void finir() const;

      /// Assigne la couleur des lignes.
      inline void assignerCouleur(const Vecteur4f& couleur);
      /// Assigne le facteur multiplicatif du pointillé.
      inline void assignerFacteur(int facteur);
      /// Assigne le patron de pointillé des lignes.
      inline void assignerPatron(unsigned short patron);


   private:
      /// Couleur des lignes.
      Vecteur4f      couleur_;
      /// Facteur multiplicatif du pointillé.
      int            facteur_;
      /// Patron de pointillé des lignes.
      unsigned short patron_;

   };



   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline LignePointillee::LignePointillee()
   ///
   /// Constructeur par défaut qui se contente d'initialiser les paramètres
   /// du rendu de la ligne à des valeurs par défaut.
   ///
   /// @return Aucune (constructeur).
   ///
   ////////////////////////////////////////////////////////////////////////
   inline LignePointillee::LignePointillee()
      : couleur_(1.0f, 1.0f, 1.0f, 1.0f) , facteur_(1) , patron_(0x3333)
   {
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void LignePointillee::commencer() const
   ///
   /// Cette fonction assigne les paramètres nécessaires pour le début du
   /// rendu de la ligne dans la machine à états OpenGL.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void LignePointillee::commencer() const
   {
      // On sauvegarde les attributs de traçage
      const GLbitfield masque = GL_CURRENT_BIT | GL_LIGHTING_BIT;
      if (patron_ == 0xFFFF) {
         // Pas de pointillé.
         glPushAttrib( masque );
      }
      else {
         // Pointillé.
         glPushAttrib( masque | GL_LINE_BIT );
         glEnable( GL_LINE_STIPPLE );
         glLineStipple( facteur_, patron_ );
      }

      glDisable(GL_LIGHTING);
      glColor4f( couleur_[0], couleur_[1], couleur_[2], couleur_[3]);
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void LignePointillee::finir() const
   ///
   /// Cette fonction restaure les paramètres d'avant le début du tracé
   /// des lignes.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void LignePointillee::finir() const
   {
      // On rétablit les attributs initiaux.
      glPopAttrib();
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void LignePointillee::assignerCouleur(const Vecteur4f& couleur)
   ///
   /// Cette fonction permet d'assigner la couleur utilisée pour dessiner
   /// les lignes.
   ///
   /// @param[in] couleur : La couleur des lignes.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void LignePointillee::assignerCouleur(const Vecteur4f& couleur)
   {
      couleur_ = couleur;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void LignePointillee::assignerFacteur(int facteur)
   ///
   /// Cette fonction permet d'assigner le facteur multiplicatif du
   /// pointillé utilisé pour dessiner les lignes.
   ///
   /// @param[in] facteur : Le facteur multiplicatif.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void LignePointillee::assignerFacteur(int facteur)
   {
      facteur_ = facteur;
   }


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn inline void LignePointillee::assignerPatron(unsigned short patron)
   ///
   /// Cette fonction permet d'assigner le patron du pointillé utilisé pour
   /// dessiner les lignes.
   ///
   /// @param[in] patron : Le patron du pointillé.
   ///
   /// @return Aucune.
   ///
   ////////////////////////////////////////////////////////////////////////
   inline void LignePointillee::assignerPatron(unsigned short patron)
   {
      patron_ = patron;
   }


} // Fin de l'espace de nom aidegl.


#endif // __UTILITAIRE_LIGNEPOINTILLEE_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
