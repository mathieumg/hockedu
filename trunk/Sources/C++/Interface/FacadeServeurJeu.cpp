
///////////////////////////////////////////////////////////////////////////////
/// @file FacadeServeurJeu.cpp
/// @author Mathieu Parent
/// @date 2013-03-18
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "FacadeServeurJeu.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\ControllerServeurJeu.h"
#include "..\Reseau\PaquetHandlers\PacketHandler.h"
#include <stdexcept>
#include <time.h>
#include "GameManager.h"
#include "RazerGameUtilities.h"
#include "..\Reseau\PaquetRunnableServeurJeu.h"
#include "UsinePaquet.h"

void InitDLLServeurJeu()
{
    NETWORK_LOG_FILE_NAME =         "SVRGAME_LOG_";
    NETWORK_PACKET_SENT_FILE_NAME = "SVRGAME_PACKET_SENT_";
    NETWORK_PACKET_RECV_FILE_NAME = "SVRGAME_PACKET_RECV_";

    // Initialisation du GestionnaireReseau
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();

    wGestionnaireReseau->setController(new ControllerServeurJeu);
    wGestionnaireReseau->initGameServer();

    // Initialise la Facade Serveur Jeu (demarre la boucle de tick)
    FacadeServeurJeu::getInstance();

}

void ConnectMasterServer(const std::string& wMasterServerIP)
{
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
    wGestionnaireReseau->demarrerNouvelleConnection("MasterServer", wMasterServerIP, TCP);

    Paquet* wPaquet = new Paquet();
    PacketDataEvent *data = (PacketDataEvent*)CreatePacketData(PT_PACKETDATAEVENT);
    data->mEventCode = GAME_SERVER_AUTHENTICATION_REQUEST;
    data->mMessage = "";
    wPaquet->setData(data);

    SPSocket wSocketMasterServer = wGestionnaireReseau->getSocket("MasterServer", TCP);
    wSocketMasterServer->setOnConnectionCallback([wGestionnaireReseau, wPaquet]()->void {
        wGestionnaireReseau->envoyerPaquet("MasterServer", wPaquet, TCP);
    });
}
void CleanUp()
{
    FacadeServeurJeu::libererInstance();
    GestionnaireReseau::libererInstance();
}


DLLEXPORT_SERVEUR_JEU const char* ObtenirAdresseIpLocaleAssociee( unsigned int minAdress, unsigned int maxAdress )
{
    std::string wTemp;
    try 
    {
        wTemp = GestionnaireReseau::obtenirInstance()->getAdresseIPLocaleAssociee(minAdress,maxAdress);
    }
    catch(ExceptionReseau&)
    {
        wTemp = "";
    }
    const char* wIp = wTemp.c_str();

    char *ret = new char[17];
    memcpy(ret,wIp, strlen(wIp)+1);
    return ret;
}


/// Pointeur vers l'instance unique de la classe.
FacadeServeurJeu* FacadeServeurJeu::instance_ = 0;



////////////////////////////////////////////////////////////////////////
///
/// @fn void* DeamonTick( void *arg )
///
/// Thread qui s'assure de faire un tick sur la liste des parties
///
/// @param[in] void *arg  : Pointeur sur FacadeServeurJeu
///
/// @return void*
///
////////////////////////////////////////////////////////////////////////
bool tickDaemon = true;
void* DeamonTick( void *arg )
{
    FacadeServeurJeu* wFacade = (FacadeServeurJeu*) arg;
    // Interval en ms, ne changera jamais
    int wTickInterval = wFacade->getTickInterval();


    // Not Accurate
    clock_t wLastTick = clock();
    while(tickDaemon)
    {
        FacadePortability::takeMutex(GameManager::mMutexTickRemove);
        clock_t wElapsed = clock()-wLastTick;
        if(wElapsed > wTickInterval)
        {
            wFacade->animer((float)wElapsed/1000.0f);
            wLastTick = clock();
        }
        FacadePortability::sleep(1); // Enlever cette ligne pour etre plus precis, mais va bouffer le CPU
        FacadePortability::releaseMutex(GameManager::mMutexTickRemove);
    }
    return 0;
}

// Callback a appeler pour demarrer le download de la map
ManagedStartMapDownload mCallbackManagedStartDownload = NULL;

void SetStartMapDownloadCallback( ManagedStartMapDownload pCallback )
{
    mCallbackManagedStartDownload = pCallback;
}






////////////////////////////////////////////////////////////////////////
///
/// @fn FacadeServeurJeu* FacadeServeurJeu::getInstance()
///
/// Cette fonction retourne un pointeur vers l'instance unique de la
/// classe.  Si cette instance n'a pas été créée, elle la crée.  Cette
/// création n'est toutefois pas nécessairement "thread-safe", car
/// aucun verrou n'est pris entre le test pour savoir si l'instance
/// existe et le moment de sa création.
///
/// @return Un pointeur vers l'instance unique de cette classe.
///
////////////////////////////////////////////////////////////////////////
FacadeServeurJeu* FacadeServeurJeu::getInstance()
{
    if (instance_ == 0)
        instance_ = new FacadeServeurJeu;

    return instance_;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeServeurJeu::libererInstance()
///
/// Cette fonction libère l'instance unique de cette classe.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeServeurJeu::libererInstance()
{
    if(instance_)
    {
        delete instance_;
        instance_ = 0;
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn FacadeServeurJeu::FacadeServeurJeu()
///
/// Ce constructeur par défaut ne fait qu'initialiser toutes les
/// variables à une valeur nulle.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
FacadeServeurJeu::FacadeServeurJeu()
    :mTickInterval(10)
{

    // Initialisation de la randomisation
    srand( (unsigned int) clock()+(unsigned int) time);

    // Demarrage du thread de tick
    FacadePortability::createThread(mDaemon, DeamonTick, this);
    if(mDaemon==NULL)
    {
        throw std::runtime_error("Erreur lors de la creation du thread de reception");
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn FacadeServeurJeu::~FacadeServeurJeu()
///
/// Ce destructeur libère les objets du modèle.
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
FacadeServeurJeu::~FacadeServeurJeu()
{
    tickDaemon = false;
    FacadePortability::waitForThreadExit(mDaemon);
    FacadePortability::terminateThread(mDaemon);

    GameManager::libererInstance();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void FacadeServeurJeu::animer( const float& temps)
///
/// Cette fonction effectue les différents calculs d'animations
/// nécessaires pour le mode jeu, tel que les différents calculs de
/// physique du jeu.
///
/// @param[in] temps : Intervalle de temps en sec sur lequel effectuer le calcul.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void FacadeServeurJeu::animer( const float& temps)
{
    // Start
    RazerGameUtilities::Updating(true);

    RazerGameUtilities::ExecuteUpdateRunnables();
    GameManager::obtenirInstance()->animer(temps);

    // End
    RazerGameUtilities::Updating(false);
}




////////////////////////////////////////////////////////////////////////
///
/// @fn int FacadeServeurJeu::transmitEvent( EventCodes pCode )
///
/// Send event to the controller
///
/// @param[in] EventCodes pCode
///
/// @return int
///
////////////////////////////////////////////////////////////////////////
void FacadeServeurJeu::transmitEvent( EventCodes pCode, ... )
{
    va_list args;
    va_start(args, pCode);
    GestionnaireReseau::obtenirInstance()->transmitEvent(pCode,args);
    va_end(args);
}



bool FacadeServeurJeu::downloadMap(int pUserId, int pMapId, CallbackDone pCallbackDone) const
{
    if(mCallbackManagedStartDownload)
    {
        // Si callback, appeler pour demarrer le download
        mCallbackManagedStartDownload(pUserId, pMapId, pCallbackDone);
        return true;
    }
    else
    {
        return false;
    }
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////