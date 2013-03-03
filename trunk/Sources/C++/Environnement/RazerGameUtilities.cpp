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
   /// Cette fonction permet d'obtenir la valeur d'une cha�ne Java (String
   /// java ou jstring) sous la forme d'une cha�ne C++ (string STL) encod�
   /// ISO-8859-1.
   ///
   /// @param[in] env    : L'environnement Java.
   /// @param[in] chaine : La cha�ne � convertir.
   ///
   /// @return La cha�ne C++.
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

       // V�rification de l'existence du ficher
       if ( !utilitaire::fichierExiste(pFilePath) ) 
       {
           // Si on est en jeu on s'assure d'avoir une table valide
           if(pField.IsGameField())
               pField.creerTerrainParDefaut(pFilePath);
           else
               pField.initialiser(pFilePath);
           // Si le fichier n'existe pas, on le cr�e.
           SaveFieldToFile(pFilePath,pField);
       }
       // si le fichier existe on le lit
       else 
       {
           XmlDocument* document = XMLUtils::LoadDocument(pFilePath.c_str());

           // Lire � partir du fichier de configuration
           if( !document )
           {
               utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier");
               // Si on est en jeu on s'assure d'avoir une table valide
               if(pField.IsGameField())
                   pField.creerTerrainParDefaut(pFilePath);
               else
                   pField.initialiser(pFilePath);
               // Si le fichier n'existe pas, on le cr�e.
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

       // �crire dans le fichier
       XMLUtils::SaveDocument(document,nomFichier.c_str());
       XMLUtils::FreeDocument(document);
   }

   /// La cha�ne repr�sentant le dossier.
   const std::string RazerGameUtilities::NOM_DOSSIER_MEDIA = "../media/";

   /// La cha�ne repr�sentant l'extension.
   const std::string RazerGameUtilities::NOM_EXTENSION = ".obj";

   /// La cha�ne repr�sentant le type de la piece.
   const std::string RazerGameUtilities::NOM_HOUSE = "piece";

   /// La cha�ne repr�sentant le type des buts.
   const std::string RazerGameUtilities::NOM_BUT = "but_milieu";
   
   /// La cha�ne repr�sentant le type des murets.
   const std::string RazerGameUtilities::NOM_MURET = "muret";

   /// La cha�ne repr�sentant le type des murets.
   const std::string RazerGameUtilities::NAME_RINK_BOARD = "rink_board";
   
   /// La cha�ne repr�sentant le type de muret relatif pour l'�dition
   const std::string RazerGameUtilities::NAME_RELATIVE_WALL = "muret_relatif";

   /// La cha�ne repr�sentant le type de la table de jeu.
   const std::string RazerGameUtilities::NOM_TABLE = "table";

   /// La cha�ne repr�sentant le type des portails.
   const std::string RazerGameUtilities::NOM_PORTAIL = "portail";

   /// La cha�ne repr�sentant le type de la rondelle.
   const std::string RazerGameUtilities::NOM_RONDELLE = "rondelle";

   /// La cha�ne repr�sentant le type des maillets.
   const std::string RazerGameUtilities::NOM_MAILLET = "maillet";

   /// La cha�ne repr�sentant le type des accelerateurs.
   const std::string RazerGameUtilities::NOM_ACCELERATEUR = "accelerateur";

   /// La cha�ne repr�sentant le type des points pour le redimensionnement de la table.
   const std::string RazerGameUtilities::NAME_TABLE_CONTROL_POINT = "table_point";

   /// La cha�ne repr�sentant le type des points de controle d'objet.
   const std::string RazerGameUtilities::NAME_CONTROL_POINT = "control_point";

   /// La cha�ne repr�sentant le type des groupes
   const std::string RazerGameUtilities::NOM_GROUPE = "Groupe";

   const std::string RazerGameUtilities::NAME_POLYGONE = "polygone";


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
