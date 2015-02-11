////////////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireModeles.cpp
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "ConfigScene.h"
#include "GestionnaireModeles.h"
#include "Utilitaire.h"
#include "NoeudTable.h"
#include "FacadeModele.h"
#include "AideGL.h"
#include "Runnable.h"
#include "RenderOpenGL2.h"
#include <iostream>

// pour le warning sur getenv();
#pragma warning(disable:4996)
// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireModeles);


// Checks whether given thread is alive.
bool IsThreadAlive(const HANDLE hThread , DWORD& dwExitCode)
{
    // Read thread's exit code.
    dwExitCode = 0;
    GetExitCodeThread(hThread, &dwExitCode);
    return dwExitCode == STILL_ACTIVE;
}

struct ModelToLoad
{
    ModelToLoad(Modele3DKey key, std::string modelName, CreateListDelegate createListDelegate = NULL):
        mKey(key), mModelName(modelName), mCreateListDelegate(createListDelegate){}
    // these 2 are pretty much always the same
    // the key used in the maps to retrieve the list and model, must be unique
    Modele3DKey mKey;
    // the name of the .obj containing the model
    std::string mModelName;

    // delegate to create the openGL list, called after loading the model
    CreateListDelegate mCreateListDelegate;
};

struct TamponNomModele
{
//     struct NomModel 
//     {
//         std::string nom;
//         Modele3D* model;
//         GLuint liste;
//     };
    
    //int ip;
    std::vector<ModelToLoad> vec;
    //std::vector<NomModel> retour;
    // Remettre le mutex si on ajoute dynamiquement des modeles
    //HANDLE mutex,mutex2;
};

struct DataThreadModels
{
    TamponNomModele* tampon;
    HGLRC glrc;
    HDC dc;
};

DWORD WINAPI WorkerLoadModel(LPVOID arg);

HANDLE mutexCreationContexGl;
TamponNomModele tamponGlobal;

////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireModeles* GestionnaireModeles::obtenirInstance()
///
/// /*Description*/
///
///
/// @return GestionnaireModeles*
///
////////////////////////////////////////////////////////////////////////
GestionnaireModeles* GestionnaireModeles::obtenirInstance()
{
    if (instance_ == 0)
    {
        instance_ = new GestionnaireModeles();
    }

    return instance_;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireModeles::GestionnaireModeles()
///
/// Ne fait qu'initialiser les variables membres de la classe.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
GestionnaireModeles::GestionnaireModeles()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireModeles::~GestionnaireModeles()
///
/// Destructeur qui désalloue les ressources.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
GestionnaireModeles::~GestionnaireModeles()
{
    for_each(banqueModeles.begin(), banqueModeles.end(), utilitaire::LibererMappe());
    for (BanqueListes::iterator i = banqueListes.begin(); i!= banqueListes.end(); i++)
    {
        if(i->second != 0)
            glDeleteLists(i->second,1);
    }
    if(mHouseList && !ConfigScene::obtenirInstance()->GetIsHouseDisplay())
    {
        glDeleteLists(mHouseList,1);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Modele3D GestionnaireModeles::obtenirModele(std::string key) const
///
/// Cette fonction retourne le modele recherché à l'aide de sa clée.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
Modele3D* GestionnaireModeles::obtenirModele( Modele3DKey key )
{
    if(banqueModeles.find(key) != banqueModeles.end())
        return banqueModeles[key];
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::ajoutModele(std::string key, Modele3D modele3d)
///
/// Cette fonction est privé et n'est utilisé que par la méthode 
///	initialiseBanque, elle permet de charger 1 modèle dans la banque.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::ajoutModele( Modele3DKey key, Modele3D* modele3d )
{
    if(modele3d)
    {
        Modele3D* oldModele = obtenirModele(key);
        if(oldModele && oldModele != modele3d)
        {
            // Si le modele existe on le supprime
            delete oldModele;
        }
        banqueModeles[key] = modele3d;
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::AjouterListe( const std::string& key, GLuint liste )
///
/// /*Description*/
///
/// @param[in] const std::string & key
/// @param[in] GLuint liste
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::AjouterListe( Modele3DKey key, GLuint liste )
{
    if(liste != NULL)
    {
        // Si les listes existe deja on les supprimes
        GLuint liste1;
        obtenirListe(key,liste1);
        if(liste1 != NULL)
        {
            glDeleteLists(liste1,1);
        }
        banqueListes[key] = liste;
    }
}

void GestionnaireModeles::ModifierListe( Modele3DKey key, GLuint liste )
{
    banqueListes[key] = liste;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::CreerListe( std::string key, bool avecTexture )
///
/// Creation des liste d'affichage pour un modele donne
///
/// @param[in] std::string key : clé pour les banques
/// @param[in] bool avecTexture : indique si on veut crée la liste avec des textures
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
GLuint GestionnaireModeles::CreerListe( Modele3D* pModel, bool avecTexture /*= true*/ )
{
    GLuint liste = NULL;
    if(pModel)
    {
        liste = glGenLists(1);
        glNewList(liste, GL_COMPILE);
        pModel->dessiner(avecTexture);	
        glEndList();
    }

    return liste;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::obtenirListes( std::string key, GLuint& liste, GLuint& listeSelection )
///
/// accesseur des listes
///
/// @param[in] std::string key : clé de la banque
/// @param[out] GLuint & liste : variable de sortie pour la liste
/// @param[out] GLuint & listeSelection : variable de sortie pour la liste de selection
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::obtenirListe( Modele3DKey key, GLuint& liste )
{
    liste = NULL;
    if(banqueListes.find(key) != banqueListes.end())
        liste = banqueListes[key];
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool GestionnaireModeles::recharger( std::string type )
///
/// Permet de recharger un modele a partir du fichier
///
/// @param[in] std::string type : le nom du type de noeud a recharger
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::recharger( Modele3DKey type )
{
    checkf(0,"Do not use reload model until method has been updated");
    // Fonction pu a jour


// 	/// Instance temporaire qui permet d'ajouter tous les modèles à la banque de modèle.
// 	Modele3D *modele= new Modele3D();
// 
// 	/// Rechargement du modele
// 	modele->charger(RazerGameUtilities::NOM_DOSSIER_MEDIA+type+RazerGameUtilities::NOM_EXTENSION);
// 	ajoutModele(type,modele);
//     GLuint liste = CreerListe(modele);
//     AjouterListe(type, liste);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::initialiser()
///
/// Effectue l'initialisation du Gestionnaire de modèle
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::initialiser()
{
    // La piece en premier pour qu'elle soit loader en dernier
    tamponGlobal.vec.push_back( ModelToLoad( RAZER_KEY_HOUSE, "Piece", RazerGameUtilities::CreateListDelegateHouse ) );
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_ZAMBONI            , "zamboni"                    , RazerGameUtilities::CreateListDelegateZamboni          ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_EMPTY_BONUS        , "EmptyBonus"                 , RazerGameUtilities::CreateListDelegateEmptyBonus       ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_BONUS              , ""                           , RazerGameUtilities::CreateListDelegateBonus            ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_GOALER             , "Goaler/Goaler"              , RazerGameUtilities::CreateListDelegateGoaler           ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_FAN                , "Fan"                        , RazerGameUtilities::CreateListDelegateFan              ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_PAUSE              , "pause"                                                                               ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_MODEL_1            , "1"                                                                                   ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_MODEL_2            , "2"                                                                                   ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_MODEL_3            , "3"                                                                                   ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_GOAL               , "but_milieu"                 , RazerGameUtilities::CreateListDelegateGoal             ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_WALL               , "muret"                      , RazerGameUtilities::CreateListDelegateWall             ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_PORTAL             , "portail"                    , RazerGameUtilities::CreateListDelegatePortal           ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_PUCK               , "rondelle"                   , RazerGameUtilities::CreateListDelegatePuck             ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_PUCK_TROLL         , "rondelleTroll"              , RazerGameUtilities::CreateListDelegatePuckTroll        ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_BOOST              , "boost"                      , RazerGameUtilities::CreateListDelegateBoost            ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_SKIN_MALLET_RED    , "maillet_red"                , RazerGameUtilities::CreateListDelegateMallet           ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_MALLET             , "maillet"                    , RazerGameUtilities::CreateListDelegateMallet           ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_TABLE_CONTROL_POINT, "control_point/control_point", RazerGameUtilities::CreateListDelegateTableControlPoint));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_CONTROL_POINT      , "control_point/control_point", RazerGameUtilities::CreateListDelegateControlPoint     ));
    tamponGlobal.vec.push_back(ModelToLoad(RAZER_KEY_TABLE              , "table"                      , RazerGameUtilities::CreateListDelegateTable            ));

    DataThreadModels* dataWorkerModel = new DataThreadModels();
    dataWorkerModel->tampon = &tamponGlobal;
    dataWorkerModel->dc = FacadeModele::getInstance()->obtenirHDC();
    dataWorkerModel->glrc = wglCreateContext(dataWorkerModel->dc);
    HGLRC baseGlrc = FacadeModele::getInstance()->GetHGLRC();

    mLoadingThread = NULL;
    DWORD thId1;

    if(dataWorkerModel->glrc != NULL && wglShareLists(baseGlrc , dataWorkerModel->glrc))
    {
        mLoadingThread = CreateThread(NULL, 0, WorkerLoadModel, dataWorkerModel,NULL,&thId1);
        RazerGameUtilities::RunOnRenderThread(new Runnable([=](Runnable* pRun){
            DWORD exitCode;
            if(!IsThreadAlive(mLoadingThread,exitCode))
            {
                if(exitCode == FALSE)
                {
                    // pour ne pas recreer le context OpenGL
                    dataWorkerModel->glrc = NULL;
                    dataWorkerModel->dc = NULL;
                    WorkerLoadModel(dataWorkerModel);
                }
                else
                {
                    CloseHandle(mLoadingThread);
                }
                pRun->setKeepAlive(false);
            }
        },true));

    }
    if(mLoadingThread == NULL)
    {
        // pour ne pas recreer le context OpenGL
        dataWorkerModel->glrc = NULL;
        dataWorkerModel->dc = NULL;
        WorkerLoadModel(dataWorkerModel);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool GestionnaireModeles::isStillLoadingModel()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool GestionnaireModeles::isStillLoadingModel() const
{
    DWORD exitCode;
    if(!IsThreadAlive(mLoadingThread,exitCode))
    {
        return tamponGlobal.vec.size() != 0;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::ReloadModels()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::ReloadModels()
{
    initialiser();
}

IRenderComponent* GestionnaireModeles::createRenderComponent( NoeudAbstrait* node ) const
{
    IRenderComponent* component = new RenderOpenGL2();
    component->setNode( node );
    return component;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn DWORD WINAPI WorkerLoadModel( LPVOID arg )
///
/// Worker permettant de faire le chargement des différents modèles nécessaire pour le jeu
/// et les ajoute dans le conteneur de modèle
///
/// @param[in] LPVOID arg
///
/// @return DWORD WINAPI
///
////////////////////////////////////////////////////////////////////////
DWORD WINAPI WorkerLoadModel( LPVOID arg )
{
    DataThreadModels* data = (DataThreadModels*) arg;
    TamponNomModele* tampon = data->tampon;

    if(data->glrc)
    {
        if(!wglMakeCurrent(data->dc,data->glrc))
        {
            aidegl::verifierErreurOpenGL();
            //wglDeleteContext(data->glrc);
            delete data;
            return FALSE;
        }
    }

    while(tampon->vec.size())
    {
        ModelToLoad& modelInfo = tampon->vec.back();

        const string& modelName = modelInfo.mModelName;

        Modele3D* modele = NULL;
        if(modelName.size())
        {
            modele = new Modele3D();
            if( !modele->charger(RazerGameUtilities::NOM_DOSSIER_MEDIA + modelName + RazerGameUtilities::NOM_EXTENSION) )
            {
                delete modele;
                modele = NULL;
            }
#if _DEBUG
            printf( "Model %s[%d] loaded\n", modelName.c_str(), modelInfo.mKey );
#endif
        }
        
        GLuint liste = 0;
        if(modelInfo.mCreateListDelegate)
        {
            liste = modelInfo.mCreateListDelegate(modele);
        }
        else
        {
            liste = GestionnaireModeles::CreerListe(modele);
        }
#if _DEBUG
        if( liste )
        {
            printf( "Model %s[%d] list created\n", modelName.c_str(), modelInfo.mKey );
        }
#endif

        auto key = modelInfo.mKey;
        RazerGameUtilities::RunOnUpdateThread(new Runnable([key,modele,liste](Runnable*) -> void {
            // les ajouter direct dans ce thread n'est pas bon car ca peut crasher si un read&write ce fait en meme temps
            if(modele)
            {
                GestionnaireModeles::obtenirInstance()->ajoutModele(key,modele);
            }
            if(liste)
            {
                GestionnaireModeles::obtenirInstance()->AjouterListe(key, liste);
            }
        }));

        // pop plus tard car on garde une reference sur l'item
        tampon->vec.pop_back();
    }
    
    delete data;

    return TRUE;
}

CreateListDelegateImplementation(House)
{
    unsigned int liste = GestionnaireModeles::CreerListe(pModel);

    GestionnaireModeles::obtenirInstance()->mHouseList = liste;

    if(ConfigScene::obtenirInstance()->GetIsHouseDisplay())
    {
        return liste;
    }
    return 0;
}
