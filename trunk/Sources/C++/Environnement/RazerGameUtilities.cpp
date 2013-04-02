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
#if BOX2D_INTEGRATED  
#include <Box2D/Box2D.h>
#endif
#if WIN32
#include "GestionnaireModeles.h"
#include "GestionnaireEvenements.h"
#include "GestionnaireEtatAbstrait.h"
#elif __APPLE__
#endif

#include "Utilitaire.h"
#include "Terrain.h"

#if MIKE_DEBUG_
PRAGMA_DISABLE_OPTIMIZATION;
#endif

#if WITH_JAVA
#include <jni.h>
#endif
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
#if WITH_JAVA
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
#endif
    return "";
}

#ifndef __APPLE__
#include "Runnable.h"
#include "QueueThreadSafe.h"
/// File scope Variables for runnables///////////////////////////////////
QueueThreadSafe<Runnable*> mUIRunnables;
QueueThreadSafe<Runnable*> mUpdateRunnables;
bool mUpdating = false, mRendering=false;
//////////////////////////////////////////////////////////////////////////

void RazerGameUtilities::Rendering(bool isRendering)
{
    mRendering = isRendering;
}
void RazerGameUtilities::Updating(bool isUpdating)
{
    mUpdating = isUpdating;
}

void RazerGameUtilities::ExecuteRenderRunnables()
{
    // on obtient sa taille au depart pour s'assurer de parcourir
    // tous les éléments une seul fois, car il est possibled e les remettre sur la pile
    for(int i=mUIRunnables.size()-1; i>=0; --i)
    {
        Runnable* pRun = NULL;
        mUIRunnables.pop(pRun);
        if(pRun)
        {
            pRun->Run();
            if(pRun->KeepAlive())
            {
                mUIRunnables.push(pRun);
            }
            else
            {
                delete pRun;
            }
        }
    }
}

void RazerGameUtilities::ExecuteUpdateRunnables()
{
    // on obtient sa taille au depart pour s'assurer de parcourir
    // tous les éléments une seul fois, car il est possibled e les remettre sur la pile
    for(int i=mUpdateRunnables.size()-1; i>=0; --i)
    {
        Runnable* pRun = NULL;
        mUpdateRunnables.pop(pRun);
        if(pRun)
        {
            pRun->Run();
            if(pRun->KeepAlive())
            {
                mUIRunnables.push(pRun);
            }
            else
            {
                delete pRun;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::RunOnRenderThread( Runnable* run )
///
/// /*Description*/
///
/// @param[in] Runnable * run
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RazerGameUtilities::RunOnRenderThread( Runnable* run, bool pForceQueue /*= false*/ )
{
    if(mUpdating || pForceQueue)
    {
        mUIRunnables.push(run);
    }
    else
    {
        run->Run();
        if(run->KeepAlive())
        {
            mUIRunnables.push(run);
        }
        else
        {
            delete run;
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeModele::RunOnUpdateThread( Runnable* run )
///
/// /*Description*/
///
/// @param[in] Runnable * run
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RazerGameUtilities::RunOnUpdateThread( Runnable* run, bool pForceQueue /*= false*/ )
{
    if(mRendering || pForceQueue)
    {
        mUpdateRunnables.push(run);
    }
    else
    {
        run->Run();
        if(run->KeepAlive())
        {
            mUpdateRunnables.push(run);
        }
        else
        {
            delete run;
        }
    }
}
#endif

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
#if WIN32
    if(pModel)
    {
        float rayon,haut,bas;
        pModel->calculerCylindreEnglobant(rayon,bas,haut);
        float ratio = radius / rayon;
        pModel->assignerFacteurAgrandissement(Vecteur3(ratio,ratio,ratio));
        return GestionnaireModeles::CreerListe(pModel);
    }
#endif
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void RazerGameUtilities::LoadFieldFromFile( const std::string& pathFile, Terrain& pField )
///
/// Effectue le chargement XML d'un terrain
///
/// @param[in] const std::string & pathFile
/// @param[in] Terrain & pField
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RazerGameUtilities::LoadFieldFromFile( const std::string& pPathFile, Terrain& pField )
{
    std::string pathFile = pPathFile;
    static const char ext[] = ".xml";
    if(pathFile.size() < 4 || strcmp((pathFile.c_str() + pathFile.size()-4),ext) != 0)
    {
        pathFile.append(ext,4);
    }

    // Vérification de l'existence du ficher
    if ( !utilitaire::fichierExiste(pathFile) ) 
    {
        // Si on est en jeu on s'assure d'avoir une table valide
        pField.creerTerrainParDefaut(pathFile);
        // Si le fichier n'existe pas, on le crée.
        SaveFieldToFile(pathFile,pField);
    }
    // si le fichier existe on le lit
    else 
    {
        // Lire à partir du fichier de configuration
        XmlDocument document; 
        if(!XMLUtils::LoadDocument(document,pathFile.c_str()))
        {
            utilitaire::afficherErreur("Erreur : chargement XML : erreur de lecture du fichier");
            // Si on est en jeu on s'assure d'avoir une table valide
            pField.creerTerrainParDefaut(pathFile);
        }
        else
        {
            bool initField = true;
            const XmlElement* config = XMLUtils::FirstChildElement(document,"Hockedu");
            const XmlElement* root = document.GetElem();
            std::string version;
            if(config && XMLUtils::readAttribute(config,"Version",version) )
            {
                root = config; //backward compatibility
                if(version != XMLUtils::XmlFieldVersion)
                {
#if WIN32
                    char MessageStr[256];
                    sprintf_s(MessageStr,"%s\nMap version #%s do not match application's version #%s"\
                        "\n\nDo you want to try loading it anyway ?",pathFile.c_str(),version.c_str(),XMLUtils::XmlFieldVersion.c_str());
                    int Result = MessageBoxA( NULL, MessageStr, "Version mismatch", MB_ICONERROR | MB_YESNO | MB_TOPMOST );
                    if( Result == IDNO )
                    {
                        initField = false;
                    }
#else
                    initField = false;
#endif
                }
            }
            if(!initField || !pField.initialiserXml(root))
            {
                // Erreur dans l'initialisation avec le xml, donc on laisse un terrain vide
                pField.creerTerrainParDefaut(pathFile);
            }
            //pField.FixCollidingObjects();
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
    std::string pathFile = nomFichier;
    static const char ext[] = ".xml";
    if(pathFile.size() < 4 || strcmp((pathFile.c_str() + pathFile.size()-4),ext) != 0)
    {
        pathFile.append(ext,4);
    }

    XmlDocument document ;
    XMLUtils::CreateDocument(document);

    XmlElement* version = XMLUtils::createNode("Hockedu");
    XMLUtils::writeAttribute(version,"Version",XMLUtils::XmlFieldVersion);
    XMLUtils::LinkEndChild(document,version);

    // Creation du noeud du terrain
    XMLUtils::LinkEndChild(version,pField.creerNoeudXML());

    // Écrire dans le fichier
    XMLUtils::SaveDocument(document,pathFile.c_str());
}


#if BOX2D_INTEGRATED  
std::map<RazerKey,PhysicsFilter> RazerGameUtilities::EditionFilters;
std::map<RazerKey,PhysicsFilter> RazerGameUtilities::GameFilters;
////////////////////////////////////////////////////////////////////////
///
/// @fn void RazerGameUtilities::FillPhysicsFilters()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RazerGameUtilities::FillPhysicsFilters()
{
    EditionFilters.clear();
    EditionFilters[RAZER_KEY_RINK_BOARD] = PhysicsFilter(CATEGORY_BOUNDARY, 0xFFFF,-1);
    EditionFilters[RAZER_KEY_WALL] = PhysicsFilter(CATEGORY_WALL, 0xFFFF);
    EditionFilters[RAZER_KEY_PUCK] = PhysicsFilter(CATEGORY_PUCK, CATEGORY_MALLET|CATEGORY_WALL|CATEGORY_BOUNDARY|CATEGORY_SELECTION);
    EditionFilters[RAZER_KEY_MALLET] = PhysicsFilter(CATEGORY_MALLET, CATEGORY_PUCK|CATEGORY_WALL|CATEGORY_BOUNDARY|CATEGORY_SELECTION);
    EditionFilters[RAZER_KEY_PORTAL] = PhysicsFilter(CATEGORY_PORTAL, 0xFFFF);
    EditionFilters[RAZER_KEY_BOOST] = PhysicsFilter(CATEGORY_BOOST, 0xFFFF);
    EditionFilters[RAZER_KEY_GOAL] = PhysicsFilter(CATEGORY_BOUNDARY, 0xFFFF,-1);
    EditionFilters[RAZER_KEY_TABLE] = PhysicsFilter(CATEGORY_BOUNDARY, CATEGORY_MALLET,-1);
    EditionFilters[RAZER_KEY_PUCK_CATCHER] = PhysicsFilter(CATEGORY_BOUNDARY, CATEGORY_PUCK,-1);
    EditionFilters[RAZER_KEY_BONUS] = PhysicsFilter(CATEGORY_BONUS, 0xFFFF);
    EditionFilters[RAZER_KEY_CONTROL_POINT] = PhysicsFilter(CATEGORY_NONE, CATEGORY_SELECTION);
    EditionFilters[RAZER_KEY_ZONE_EDITION] = PhysicsFilter(CATEGORY_BOUNDARY,0xFFFF,-1);
    EditionFilters[RAZER_KEY_SELECTION_BODY] = PhysicsFilter(CATEGORY_SELECTION,0,-1);

    GameFilters.clear();
    GameFilters[RAZER_KEY_RINK_BOARD] = PhysicsFilter(CATEGORY_BOUNDARY, CATEGORY_PUCK|CATEGORY_MALLET);
    GameFilters[RAZER_KEY_WALL] = PhysicsFilter(CATEGORY_WALL, CATEGORY_PUCK|CATEGORY_MALLET);
    GameFilters[RAZER_KEY_PUCK] = PhysicsFilter(CATEGORY_PUCK, 0xFFFF);
    GameFilters[RAZER_KEY_MALLET] = PhysicsFilter(CATEGORY_MALLET, CATEGORY_PUCK|CATEGORY_WALL|CATEGORY_BOUNDARY|CATEGORY_MIDLANE);
    GameFilters[RAZER_KEY_PORTAL] = PhysicsFilter(CATEGORY_PORTAL, CATEGORY_PUCK,0,true);
    GameFilters[RAZER_KEY_BOOST] = PhysicsFilter(CATEGORY_BOOST, CATEGORY_PUCK,0,true);
    GameFilters[RAZER_KEY_GOAL] = PhysicsFilter(CATEGORY_BOUNDARY, CATEGORY_MALLET);
    GameFilters[RAZER_KEY_TABLE] = PhysicsFilter(CATEGORY_MIDLANE, CATEGORY_MALLET);  // the midlane preventing mallet go to through
    GameFilters[RAZER_KEY_PUCK_CATCHER] = PhysicsFilter(CATEGORY_BOUNDARY, CATEGORY_PUCK);
    GameFilters[RAZER_KEY_BONUS] = PhysicsFilter(CATEGORY_BONUS, CATEGORY_PUCK,0,true);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void RazerGameUtilities::ApplyFilters( b2FixtureDef& def,RazerKey key )
///
/// /*Description*/
///
/// @param[in] b2FixtureDef & def
/// @param[in] RazerKey key
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RazerGameUtilities::ApplyFilters( b2FixtureDef& def,RazerKey key, bool inGame )
{
    PhysicsFilter filter;
    std::map<RazerKey,PhysicsFilter>& filters = inGame?GameFilters:EditionFilters;
    auto it=filters.find(key);
    if(it != filters.end())
    {
        filter = it->second;
    }
    def.isSensor = filter.mIsSensor;
    def.filter.categoryBits = filter.mCategory;
    def.filter.maskBits = filter.mMaskbit;
    def.filter.groupIndex = filter.mGroup;
}
#endif //BOX2D_INTEGRATED



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

/// La chaîne représentant le type des Zamboni.
const std::string RazerGameUtilities::NAME_ZAMBONI = "zamboni";

/// La chaîne représentant le type des groupes
const std::string RazerGameUtilities::NOM_GROUPE = "Groupe";

const std::string RazerGameUtilities::NAME_POLYGONE = "polygone";
const std::string RazerGameUtilities::NAME_EMPTY_BONUS = "EmptyBonus";
const std::string RazerGameUtilities::NAME_BONUS = "bonus";
const std::string RazerGameUtilities::NAME_GOALER = "goaler";

const RazerGameUtilities::StringToKeyMap* RazerGameUtilities::StringTypeToEnum = NULL;

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

#if MIKE_DEBUG_
PRAGMA_ENABLE_OPTIMIZATION;
#endif
