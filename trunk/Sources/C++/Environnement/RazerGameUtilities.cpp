//////////////////////////////////////////////////////////////////////////////
/// @file RazerGameUtilities.cpp
/// @author Michael Ferris
/// @date 2007-05-22
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "RazerGameUtilities.h"

#include <jni.h>
#include "GestionnaireModeles.h"

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

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn unsigned int RazerGameUtilities::CreateListSphereDefault( class Modele3D*, float radius )
   ///
   /// /*Description*/
   ///
   /// @param[in] class Modele3D *
   /// @param[in] float radius
   ///
   /// @return unsigned int
   ///
   ////////////////////////////////////////////////////////////////////////
   unsigned int RazerGameUtilities::CreateListSphereDefault( Modele3D* pModel, float radius )
   {
       float rayon,haut,bas;
       pModel->calculerCylindreEnglobant(rayon,bas,haut);
       float ratio = radius / rayon;
       pModel->assignerFacteurAgrandissement(Vecteur3(ratio,ratio,ratio));
       return GestionnaireModeles::CreerListe(pModel);
   }

   /// La chaîne représentant le dossier.
   const std::string RazerGameUtilities::NOM_DOSSIER_MEDIA = "../media/";

   /// La chaîne représentant l'extension.
   const std::string RazerGameUtilities::NOM_EXTENSION = ".obj";

   /// La chaîne représentant le type de la piece.
   const std::string RazerGameUtilities::NOM_HOUSE = "piece";

   /// La chaîne représentant le type des buts.
   const std::string RazerGameUtilities::NOM_BUT = "but_milieu";
   
   /// La chaîne représentant le type des murets.
   const std::string RazerGameUtilities::NOM_MURET = "muret";

   /// La chaîne représentant le type des murets.
   const std::string RazerGameUtilities::NOM_MURET_RELATIF = "muret_relatif";
   
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
