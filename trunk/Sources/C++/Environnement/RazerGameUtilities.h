///////////////////////////////////////////////////////////////////////////
/// @file RazerGameUtilities.h
/// @author Martin Bisson, Michael Ferris
/// @date 2013-02-09
/// @version 2.0
///
/// 
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__
#define __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__

#include <string>

// Macro permettant de localiser la signature des fonctions et éviter des gros replace all en cas de changement
#define CreateListDelegateSignature(name) static unsigned int CreateListDelegate##name##(class Modele3D* pModel)
#define CreateListDelegateImplementation(name) unsigned int RazerGameUtilities::CreateListDelegate##name##(Modele3D* pModel)
typedef unsigned int (*CreateListDelegate)(class Modele3D*);

class RazerGameUtilities {
public:
    static unsigned int CreateListSphereDefault(class Modele3D*, float radius);


   /// Effectue une conversion entre une chaîne Java et une chaîne C++.
   static std::string obtenirChaineISO(void* env, void* chaine);
   
   /// La chaîne représentant un accelerateur.
   static const std::string NOM_ACCELERATEUR;
   CreateListDelegateSignature(Boost);

   /// La chaîne représentant le dossier.
   static const std::string NOM_DOSSIER_MEDIA;

   /// La chaîne représentant l'extension.
   static const std::string NOM_EXTENSION;

   /// La chaîne représentant le type de la piece.
   static const std::string NOM_HOUSE;
   CreateListDelegateSignature(House);

   /// La chaîne représentant le type des buts.
   static const std::string NOM_BUT;
   CreateListDelegateSignature(Goal);

   /// La chaîne représentant le type des murets.
   static const std::string NOM_MURET;
   CreateListDelegateSignature(Wall);

   /// La chaîne représentant le type des murets.
   static const std::string NOM_MURET_RELATIF;

   /// La chaîne représentant le type de la table de jeu.
   static const std::string NOM_TABLE;
   CreateListDelegateSignature(Table);

   /// La chaîne représentant le type des portails.
   static const std::string NOM_PORTAIL;
   CreateListDelegateSignature(Portal);

   /// La chaîne représentant le type de la rondelle.
   static const std::string NOM_RONDELLE;
   CreateListDelegateSignature(Puck);

   /// La chaîne représentant le type des maillets.
   static const std::string NOM_MAILLET;
   CreateListDelegateSignature(Mallet);

   /// La chaîne représentant le type des points.
   static const std::string NOM_POINT;
   CreateListDelegateSignature(ControlPoint);

   /// La chaîne représentant le type des Groupes.
   static const std::string NOM_GROUPE;

};


#endif // __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
