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
#include "GestionnaireEvenements.h"
#include "GestionnaireEtatAbstrait.h"
#include "Utilitaire.h"
#include "Terrain.h"

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

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void RazerGameUtilities::LoadFieldFromFile( const std::string& pFilePath, Terrain& pField )
   ///
   /// Effectue le chargement XML d'un terrain
   ///
   /// @param[in] const std::string & pFilePath
   /// @param[in] Terrain & pField
   ///
   /// @return void
   ///
   ////////////////////////////////////////////////////////////////////////
   void RazerGameUtilities::LoadFieldFromFile( const std::string& pFilePath, Terrain& pField )
   {
       // Todo:: ajouter de la valider sur le path du fichier
//        if(pFilePath.size() < 4 )
//        {
// 
//        }
//        strcmp((pFilePath.c_str() + pFilePath.size()-4),".xml");
// 
       // En place pour eviter des cas speciaux avec la souris (soit des ajout d'objets liberer 2 fois)
       GestionnaireEvenements::modifierEtatSouris(ETAT_SOURIS_DEPLACER_FENETRE);

       // Vérification de l'existence du ficher
       if ( !utilitaire::fichierExiste(pFilePath) ) 
       {
           // Si on est en jeu on s'assure d'avoir une table valide
           if(pField.IsGameField())
               pField.creerTerrainParDefaut(pFilePath);
           else
               pField.initialiser(pFilePath);
           // Si le fichier n'existe pas, on le crée.
           SaveFieldToFile(pFilePath,pField);
       }
       // si le fichier existe on le lit
       else 
       {
           XmlDocument* document = XMLUtils::LoadDocument(pFilePath.c_str());

           // Lire à partir du fichier de configuration
           if( !document )
           {
               utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier");
               // Si on est en jeu on s'assure d'avoir une table valide
               if(pField.IsGameField())
                   pField.creerTerrainParDefaut(pFilePath);
               else
                   pField.initialiser(pFilePath);
               // Si le fichier n'existe pas, on le crée.
               SaveFieldToFile(pFilePath,pField);
           }
           else
           {
               if(!pField.initialiserXml((XmlElement*)document))
               {
                   // Erreur dans l'initialisation avec le xml, donc on laisse un terrain vide
                   pField.initialiser(pFilePath);
               }
               //pField.FixCollidingObjects();

               XMLUtils::FreeDocument(document);
           }
       }
   }

   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn void RazerGameUtilities::SaveFieldToFile( const std::string& nomFichier )
   ///
   /// /*Description*/
   ///
   /// @param[in] const std::string & nomFichier
   ///
   /// @return void
   ///
   ////////////////////////////////////////////////////////////////////////
   void RazerGameUtilities::SaveFieldToFile( const std::string& nomFichier, Terrain& pField  )
   {
       XmlDocument* document = XMLUtils::CreateDocument("1.0", "", "");

       // Creation du noeud du terrain
       XMLUtils::LinkEndChild((XmlElement*)document,pField.creerNoeudXML());

       // Écrire dans le fichier
       XMLUtils::SaveDocument(document,nomFichier.c_str());
       XMLUtils::FreeDocument(document);
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
   const std::string RazerGameUtilities::NAME_RINK_BOARD = "rink_board";
   
   /// La chaîne représentant le type de muret relatif pour l'édition
   const std::string RazerGameUtilities::NAME_RELATIVE_WALL = "muret_relatif";

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
   const std::string RazerGameUtilities::NAME_TABLE_CONTROL_POINT = "table_point";

   /// La chaîne représentant le type des points de controle d'objet.
   const std::string RazerGameUtilities::NAME_CONTROL_POINT = "control_point";

   /// La chaîne représentant le type des groupes
   const std::string RazerGameUtilities::NOM_GROUPE = "Groupe";

   const std::string RazerGameUtilities::NAME_POLYGONE = "polygone";


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
