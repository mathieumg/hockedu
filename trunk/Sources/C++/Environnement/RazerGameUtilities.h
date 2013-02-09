///////////////////////////////////////////////////////////////////////////
/// @file RazerGameUtilities.h
/// @author Martin Bisson, Michael Ferris
/// @date 2013-02-09
/// @version 2.0
///
/// 
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__
#define __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__

#define BOX2D_INTEGRATED 1
#define BOX2D_DEBUG 1
#include <string>

class RazerGameUtilities {
public:
   /// Effectue une conversion entre une cha�ne Java et une cha�ne C++.
   static std::string obtenirChaineISO(void* env, void* chaine);
   
   /// La cha�ne repr�sentant un accelerateur.
   static const std::string NOM_ACCELERATEUR;

   /// La cha�ne repr�sentant le dossier.
   static const std::string NOM_DOSSIER;

   /// La cha�ne repr�sentant l'extension.
   static const std::string NOM_EXTENSION;

   /// La cha�ne repr�sentant le type de la piece.
   static const std::string NOM_PIECE;

   /// La cha�ne repr�sentant le type des buts.
   static const std::string NOM_BUT_MILIEU;

   /// La cha�ne repr�sentant le type des buts.
   static const std::string NOM_BUT_COTE;

   /// La cha�ne repr�sentant le type des murets.
   static const std::string NOM_MURET;

   /// La cha�ne repr�sentant le type des murets.
   static const std::string NOM_MURET_RELATIF;

   /// La cha�ne repr�sentant le type des murets de la zone d'edition.
   static const std::string NOM_MURET_ZONE_EDITION;

   /// La cha�ne repr�sentant le type de la table de jeu.
   static const std::string NOM_TABLE;

   /// La cha�ne repr�sentant le type des portails.
   static const std::string NOM_PORTAIL;

   /// La cha�ne repr�sentant le type de la rondelle.
   static const std::string NOM_RONDELLE;

   /// La cha�ne repr�sentant le type des maillets.
   static const std::string NOM_MAILLET;

   /// La cha�ne repr�sentant le type des points.
   static const std::string NOM_POINT;

   /// La cha�ne repr�sentant le type des Groupes.
   static const std::string NOM_GROUPE;
};


#endif // __ENVIRONNEMENT_UTILITAIREINF2990_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
