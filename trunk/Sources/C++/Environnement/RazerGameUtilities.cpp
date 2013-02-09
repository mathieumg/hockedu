//////////////////////////////////////////////////////////////////////////////
/// @file UtilitaireINF2990.cpp
/// @author Martin Bisson
/// @date 2007-05-22
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "RazerGameUtilities.h"

#include <jni.h>

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn string obtenirChaineISO(JNIEnv* env, jstring chaine)
   ///
   /// Cette fonction permet d'obtenir la valeur d'une chaîne Java (String
   /// java ou jstring) sous la forme d'une chaîne C++ (string STL) encodé
   /// ISO-8859-1.
   ///
   /// @param[in] env    : L'environnement Java.
   /// @param[in] chaine : La chaîne à convertir.
   ///
   /// @return La chaîne C++.
   ///
   ////////////////////////////////////////////////////////////////////////
   std::string RazerGameUtilities::obtenirChaineISO(void* envVoid, void* chaineVoid)
   {
       JNIEnv* env = (JNIEnv*)envVoid;
       jstring chaine = *(jstring*)chaineVoid;
      jclass classeString = env->FindClass("java/lang/String");

      jmethodID getBytes = env->GetMethodID(
         classeString, "getBytes", "(Ljava/lang/String;)[B"
         );
      jbyteArray byteArray = (jbyteArray)env->CallObjectMethod(
         chaine, getBytes, env->NewStringUTF("ISO-8859-1")
         );

      return std::string((const char*)env->GetByteArrayElements(
         byteArray, 0), env->GetArrayLength(byteArray)
         );
   }


   /// La chaîne représentant le dossier.
   const std::string RazerGameUtilities::NOM_DOSSIER = "media/";

   /// La chaîne représentant l'extension.
   const std::string RazerGameUtilities::NOM_EXTENSION = ".obj";

   /// La chaîne représentant le type de la piece.
   const std::string RazerGameUtilities::NOM_PIECE = "piece";

   /// La chaîne représentant le type des buts.
   const std::string RazerGameUtilities::NOM_BUT_MILIEU = "but_milieu";

   /// La chaîne représentant le type des buts.
   const std::string RazerGameUtilities::NOM_BUT_COTE = "but_cote";

   /// La chaîne représentant le type des murets.
   const std::string RazerGameUtilities::NOM_MURET = "muret";

   /// La chaîne représentant le type des murets.
   const std::string RazerGameUtilities::NOM_MURET_RELATIF = "muret_relatif";

   /// La chaîne représentant le type des murets de la zone d'edition.
   const std::string RazerGameUtilities::NOM_MURET_ZONE_EDITION = "muret_zone_edition";

   /// La chaîne représentant le type de la table de jeu.
   const std::string RazerGameUtilities::NOM_TABLE = "table";

   /// La chaîne représentant le type des portails.
   const std::string RazerGameUtilities::NOM_PORTAIL = "portail";

   /// La chaîne représentant le type de la rondelle.
   const std::string RazerGameUtilities::NOM_RONDELLE = "rondelle";

   /// La chaîne représentant le type des maillets.
   const std::string RazerGameUtilities::NOM_MAILLET = "maillet";

   /// La chaîne représentant le type des accelerateurs.
   const std::string RazerGameUtilities::NOM_ACCELERATEUR = "accelerateur";

   /// La chaîne représentant le type des points pour le redimensionnement de la table.
   const std::string RazerGameUtilities::NOM_POINT = "point";

   /// La chaîne représentant le type des groupes
   const std::string RazerGameUtilities::NOM_GROUPE = "Groupe";


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
