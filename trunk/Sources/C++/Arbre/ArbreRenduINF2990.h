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
/// @brief Classe qui repr�sente l'arbre de rendu sp�cifique au projet de
///        INF2990.
///
///        Cette classe s'occupe de configurer les usines des noeuds qui
///        seront utilis�s par le projet.
///
/// @author Martin Bisson
/// @date 2007-03-23
///////////////////////////////////////////////////////////////////////////
class ArbreRenduINF2990 : public ArbreRendu
{
public:
   /// Constructeur par d�faut.
   ArbreRenduINF2990();
   /// Destructeur.
   virtual ~ArbreRenduINF2990();

   /// Initialise l'arbre de rendu � son �tat initial.
   void initialiser();

   /// Retourne le noeud repr�sentant la table elle-m�me
   NoeudTable* obtenirTable();


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



#endif // __ARBRE_ARBRERENDUINF2990_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
