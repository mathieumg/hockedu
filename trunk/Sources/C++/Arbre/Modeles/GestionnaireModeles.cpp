////////////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireModeles.cpp
/// @author Samuel Ledoux
/// @date 2012-01-27
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "GestionnaireModeles.h"
#include "ArbreRenduINF2990.h"
#include "Utilitaire.h"
#include "NoeudTable.h"
#include "FacadeModele.h"
#include "AideGL.h"

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

typedef void (*ModelPostLoad)(const std::string&, Modele3D*, GLuint);

struct ModelToLoad
{
    ModelToLoad(std::string key, ModelPostLoad callback = NULL, bool createList = true):
    mKey(key), mModelName(mKey), mbCreateList(createList), mCallBack(callback)
    {

    }
    ModelToLoad(std::string key, std::string modelName, ModelPostLoad callback = NULL, bool createList = true):
    mKey(key), mModelName(modelName), mbCreateList(createList), mCallBack(callback)
    {

    }
    // these 2 are pretty much always the same
    std::string mKey;
    std::string mModelName;
    bool mbCreateList;
    ModelPostLoad mCallBack;
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
    int compteurTypeID = 0;
    nameToTypeId_[ArbreRenduINF2990::NOM_MURET] = ++compteurTypeID;
    typeIdToName_[compteurTypeID] = ArbreRenduINF2990::NOM_MURET;

    nameToTypeId_[ArbreRenduINF2990::NOM_PORTAIL] = ++compteurTypeID;
    typeIdToName_[compteurTypeID] = ArbreRenduINF2990::NOM_PORTAIL;

    nameToTypeId_[ArbreRenduINF2990::NOM_ACCELERATEUR] = ++compteurTypeID;
    typeIdToName_[compteurTypeID] = ArbreRenduINF2990::NOM_ACCELERATEUR;

    nameToTypeId_[ArbreRenduINF2990::NOM_MAILLET] = ++compteurTypeID;
    typeIdToName_[compteurTypeID] = ArbreRenduINF2990::NOM_MAILLET;

    nameToTypeId_[ArbreRenduINF2990::NOM_POINT] = ++compteurTypeID;
    typeIdToName_[compteurTypeID] = ArbreRenduINF2990::NOM_POINT;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireModeles::~GestionnaireModeles()
///
/// Destructeur qui d�salloue les ressources.
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
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Modele3D GestionnaireModeles::obtenirModele(std::string key) const
///
/// Cette fonction retourne le modele recherch� � l'aide de sa cl�e.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
Modele3D* GestionnaireModeles::obtenirModele( const std::string& key )
{
	if(banqueModeles.find(key) != banqueModeles.end())
		return banqueModeles[key];
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::ajoutModele(std::string key, Modele3D modele3d)
///
/// Cette fonction est priv� et n'est utilis� que par la m�thode 
///	initialiseBanque, elle permet de charger 1 mod�le dans la banque.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::ajoutModele( const std::string& key, Modele3D* modele3d )
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
void GestionnaireModeles::AjouterListe( const std::string& key, GLuint liste )
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

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::CreerListe( std::string key, bool avecTexture )
///
/// Creation des liste d'affichage pour un modele donne
///
/// @param[in] std::string key : cl� pour les banques
/// @param[in] bool avecTexture : indique si on veut cr�e la liste avec des textures
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
/// @param[in] std::string key : cl� de la banque
/// @param[out] GLuint & liste : variable de sortie pour la liste
/// @param[out] GLuint & listeSelection : variable de sortie pour la liste de selection
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::obtenirListe( const std::string& key, GLuint& liste )
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
void GestionnaireModeles::recharger( const std::string& type )
{
	/// Instance temporaire qui permet d'ajouter tous les mod�les � la banque de mod�le.
	Modele3D *modele= new Modele3D();

	/// Rechargement du modele
	modele->charger(ArbreRenduINF2990::NOM_DOSSIER+type+ArbreRenduINF2990::NOM_EXTENSION);
	ajoutModele(type,modele);
    GLuint liste = CreerListe(modele);
    AjouterListe(type, liste);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::string GestionnaireModeles::obtenirNameFromTypeId( const GLuint& typeId ) const
///
/// permet de conna�tre le nom du type a partir de son ID
///
/// @param[in] const GLuint & typeId
///
/// @return std::string : le nom du type concret si trouve ou une chaine vide s'il n'existe pas
///
////////////////////////////////////////////////////////////////////////
std::string GestionnaireModeles::obtenirNameFromTypeId( const GLuint& typeId ) const
{
	TypeIdToName::const_iterator iter = typeIdToName_.find(typeId);
	if(iter != typeIdToName_.end())
		return iter->second;
	return "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GLuint GestionnaireModeles::obtenirTypeIdFromName( const std::string name ) const
///
/// permet de conna�tre le ID d'un type partir de son nom
///
/// @param[in] const std::string name
///
/// @return GLuint : le ID du type ayant ce nom, ou 0 s'il ne le trouve pas
///
////////////////////////////////////////////////////////////////////////
GLuint GestionnaireModeles::obtenirTypeIdFromName( const std::string& name ) const
{
	NameToTypeId::const_iterator iter = nameToTypeId_.find(name);
	if(iter != nameToTypeId_.end())
		return iter->second;
	return 0;
}

void TablePostLoad(const std::string& key, Modele3D* model, GLuint list)
{
    NoeudTable::initialiserListeIndexPoints(model);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModeles::initialiser()
///
/// Effectue l'initialisation du Gestionnaire de mod�le
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModeles::initialiser()
{
    // La piece en premier pour qu'elle soit loader en dernier
    tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_PIECE));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_TABLE,TablePostLoad,false));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_BUT_MILIEU));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_BUT_COTE));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_MURET));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_PORTAIL));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_RONDELLE));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_ACCELERATEUR));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_MAILLET));
	tamponGlobal.vec.push_back(ModelToLoad(ArbreRenduINF2990::NOM_POINT));
	tamponGlobal.vec.push_back(ModelToLoad("pause"));
	tamponGlobal.vec.push_back(ModelToLoad("1"));
	tamponGlobal.vec.push_back(ModelToLoad("2"));
	tamponGlobal.vec.push_back(ModelToLoad("3"));

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
        FacadeModele::getInstance()->RunOnUIThread(new Runnable([=](Runnable* pRun){
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
/// @fn DWORD WINAPI WorkerLoadModel( LPVOID arg )
///
/// Worker permettant de faire le chargement des diff�rents mod�les n�cessaire pour le jeu
/// et les ajoute dans le conteneur de mod�le
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
            wglDeleteContext(data->glrc);
            delete data;
            return FALSE;
        }
    }

	while(tampon->vec.size())
	{
        ModelToLoad& modelInfo = tampon->vec.back();
		const string& modelName = modelInfo.mModelName;

		Modele3D* modele = new Modele3D();
		if(!modele->charger(ArbreRenduINF2990::NOM_DOSSIER+modelName+ArbreRenduINF2990::NOM_EXTENSION))
        {
            tampon->vec.pop_back();
            continue;
        }
        GLuint liste = GestionnaireModeles::CreerListe(modele);

        const string& key = modelInfo.mKey;
        FacadeModele::getInstance()->RunOnUIThread(new Runnable([=](Runnable*) -> void {
            // les ajouter direct dans ce thread n'est pas bon car ca peut crasher si un read&write ce fait en meme temps
            GestionnaireModeles::obtenirInstance()->ajoutModele(key,modele);
            if(modelInfo.mbCreateList)
            {
                GestionnaireModeles::obtenirInstance()->AjouterListe(key, liste);
            }
        }));

        if(modelInfo.mCallBack)
        {
            modelInfo.mCallBack(key,modele,liste);
        }

        tampon->vec.pop_back();
	}
	
    wglDeleteContext(data->glrc);
    delete data;

	return TRUE;
}
