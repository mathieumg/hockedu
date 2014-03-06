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
#include "EventManager.h"
#include "GestionnaireEtatAbstrait.h"
#elif __APPLE__
#endif

#include "Utilitaire.h"
#include "Terrain.h"

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
    pField.setAbsolutePath(pPathFile);
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

	int wTest  = (int)pathFile.find_last_of("\\");
	int wTest2 = (int)pathFile.find_last_of("/");
    int wIndexSeparateur = utilitaire::borneSuperieure(wTest2, wTest);

    int wIndexPoint = (int)pathFile.find_last_of(".");
    std::string wMapName;
    if(wIndexPoint == std::string::npos)
    {
        wMapName = pathFile.substr(wIndexSeparateur+1, pathFile.length()-wIndexSeparateur);
    }
    else
    {
        wMapName = pathFile.substr(wIndexSeparateur+1, wIndexPoint-wIndexSeparateur-1);
    }

    pField.modifierNom(wMapName);

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
    /// utilisation d'une category de gameplay pour pouvoir simplement faire la selection avec celui-ci
    EditionFilters[RAZER_KEY_CONTROL_POINT] = PhysicsFilter(CATEGORY_FORCE_FIELD, CATEGORY_SELECTION);
    EditionFilters[RAZER_KEY_ZONE_EDITION] = PhysicsFilter(CATEGORY_BOUNDARY,0xFFFF,-1);
    EditionFilters[RAZER_KEY_SELECTION_BODY] = PhysicsFilter(CATEGORY_SELECTION,0xFFFF,-1);

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

const char* RazerGameUtilities::RazerKeyToString[NB_RAZER_KEYS] =
{
    "None"                //RAZER_KEY_NONE
    , "Boost"               //RAZER_KEY_BOOST
    , "House"               //RAZER_KEY_HOUSE
    , "Goal"                //RAZER_KEY_GOAL
    , "Wall"                //RAZER_KEY_WALL
    , "Rink_board"          //RAZER_KEY_RINK_BOARD
    , "Table"               //RAZER_KEY_TABLE
    , "Portal"              //RAZER_KEY_PORTAL
    , "Puck"                //RAZER_KEY_PUCK
    , "Puck_catcher"        //RAZER_KEY_PUCK_CATCHER
    , "Puck_troll"          //RAZER_KEY_PUCK_TROLL
    , "Mallet"              //RAZER_KEY_MALLET
    , "Table_control_point" //RAZER_KEY_TABLE_CONTROL_POINT
    , "Control_point"       //RAZER_KEY_CONTROL_POINT
    , "Group"               //RAZER_KEY_GROUP
    , "Polygon"             //RAZER_KEY_POLYGON
    , "Zamboni"             //RAZER_KEY_ZAMBONI
    , "Empty_bonus"         //RAZER_KEY_EMPTY_BONUS
    , "Bonus"               //RAZER_KEY_BONUS
    , "Goaler"              //RAZER_KEY_GOALER
    , "Fan"                 //RAZER_KEY_FAN
    , "Pause"               //RAZER_KEY_PAUSE
    , "Model_1"             //RAZER_KEY_MODEL_1
    , "Model_2"             //RAZER_KEY_MODEL_2
    , "Model_3"             //RAZER_KEY_MODEL_3
    , "Root_tree"           //RAZER_KEY_ROOT_TREE
    , "Zone_edition"        //RAZER_KEY_ZONE_EDITION
    , "Selection_body"      //RAZER_KEY_SELECTION_BODY
    , "Force_field"         //RAZER_KEY_FORCE_FIELD
    , "Skin_mallet_red"     //RAZER_KEY_SKIN_MALLET_RED
    , "Bonus_properties"    //RAZER_KEY_BONUS_PROPERTIES
};

/// La chaîne représentant le dossier.
const std::string RazerGameUtilities::NOM_DOSSIER_MEDIA = "../media/";

/// La chaîne représentant l'extension.
const std::string RazerGameUtilities::NOM_EXTENSION = ".obj";

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
