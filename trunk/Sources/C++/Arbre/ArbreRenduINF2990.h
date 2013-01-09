///////////////////////////////////////////////////////////////////////////
/// @file ArbreRenduINF2990.h
/// @author Martin Bisson
/// @date 2007-03-23
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ARBRE_ARBRERENDUINF2990_H__
#define __ARBRE_ARBRERENDUINF2990_H__


#include "ArbreRendu.h"

#include <map>

class NoeudTable;

///////////////////////////////////////////////////////////////////////////
/// @class ArbreRenduINF2990
/// @brief Classe qui représente l'arbre de rendu spécifique au projet de
///        INF2990.
///
///        Cette classe s'occupe de configurer les usines des noeuds qui
///        seront utilisés par le projet.
///
/// @author Martin Bisson
/// @date 2007-03-23
///////////////////////////////////////////////////////////////////////////
class ArbreRenduINF2990 : public ArbreRendu
{
public:
   /// Constructeur par défaut.
   ArbreRenduINF2990();
   /// Destructeur.
   virtual ~ArbreRenduINF2990();

   /// Initialise l'arbre de rendu à son état initial.
   void initialiser();

   /// Retourne le noeud représentant la table elle-même
   NoeudTable* obtenirTable();


   /// La chaîne représentant un accelerateur.
   static const std::string NOM_ACCELERATEUR;

   /// La chaîne représentant le dossier.
   static const std::string NOM_DOSSIER;

   /// La chaîne représentant l'extension.
   static const std::string NOM_EXTENSION;

   /// La chaîne représentant le type de la piece.
   static const std::string NOM_PIECE;
   
   /// La chaîne représentant le type des buts.
   static const std::string NOM_BUT_MILIEU;

   /// La chaîne représentant le type des buts.
   static const std::string NOM_BUT_COTE;

   /// La chaîne représentant le type des murets.
   static const std::string NOM_MURET;

   /// La chaîne représentant le type des murets.
   static const std::string NOM_MURET_RELATIF;

   /// La chaîne représentant le type des murets de la zone d'edition.
   static const std::string NOM_MURET_ZONE_EDITION;

   /// La chaîne représentant le type de la table de jeu.
   static const std::string NOM_TABLE;

   /// La chaîne représentant le type des portails.
   static const std::string NOM_PORTAIL;
   
   /// La chaîne représentant le type de la rondelle.
   static const std::string NOM_RONDELLE;

   /// La chaîne représentant le type des maillets.
   static const std::string NOM_MAILLET;

   /// La chaîne représentant le type des points.
   static const std::string NOM_POINT;

   /// La chaîne représentant le type des Groupes.
   static const std::string NOM_GROUPE;


};



#endif // __ARBRE_ARBRERENDUINF2990_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
