
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
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetUserStatus.h"
#include "..\Reseau\PaquetHandlers\PacketHandler.h"
#include "..\Reseau\Paquets\PaquetEvent.h"
#include "..\Reseau\UsinePaquets\UsinePaquetGameStatus.h"
#include "..\reseau\UsinePaquets\UsinePaquetMaillet.h"
#include "..\Reseau\UsinePaquets\UsinePaquetGameCreation.h"
#include "..\reseau\UsinePaquets\UsinePaquetGameConnection.h"
#include <stdexcept>
#include <time.h>
#include "GameManager.h"
#include "RazerGameUtilities.h"
#include "..\reseau\UsinePaquets\UsinePaquetRondelle.h"
#include "..\Reseau\UsinePaquets\UsinePaquetGameEvent.h"
#include "..\Reseau\UsinePaquets\UsinePaquetBonus.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerBonus.h"
#include "..\Reseau\PaquetRunnableServeurJeu.h"

void InitDLLServeurJeu()
{
    // Initialisation du GestionnaireReseau
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();

    wGestionnaireReseau->setController(new ControllerServeurJeu);
    if(!ControllerServeurJeu::isLocalServer())
    {
        wGestionnaireReseau->initGameServer();
    }
    else
    {
        wGestionnaireReseau->initServer();
    }
    

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau(CHAT_MESSAGE, new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    wGestionnaireReseau->ajouterOperationReseau(USER_STATUS, new PacketHandlerUserStatus, new UsinePaquetUserStatus);
    wGestionnaireReseau->ajouterOperationReseau(GAME_STATUS, new PacketHandlerGameStatus, new UsinePaquetGameStatus);
    //wGestionnaireReseau->ajouterOperationReseau(AUTHENTIFICATION_SERVEUR_JEU, new PacketHandlerAuthentificationServeurJeu, new UsinePaquetAuthentificationServeurJeu);
    wGestionnaireReseau->ajouterOperationReseau(MAILLET, new PacketHandlerMaillet, new UsinePaquetMaillet);
    wGestionnaireReseau->ajouterOperationReseau(RONDELLE, new PacketHandlerRondelle, new UsinePaquetRondelle);
    wGestionnaireReseau->ajouterOperationReseau(GAME_CREATION_REQUEST, new PacketHandlerGameCreation, new UsinePaquetGameCreation);
    wGestionnaireReseau->ajouterOperationReseau(GAME_CONNECTION, new PacketHandlerGameConnection, new UsinePaquetGameConnection);
    wGestionnaireReseau->ajouterOperationReseau(GAME_EVENT, new PacketHandlerGameEvent, new UsinePaquetGameEvent);
    wGestionnaireReseau->ajouterOperationReseau(BONUS, new PacketHandlerBonus, new UsinePaquetBonus);

    // Initialise la Facade Serveur Jeu (demarre la boucle de tick)
    FacadeServeurJeu::getInstance();

}

void ConnectMasterServer(const std::string& wMasterServerIP)
{
    if(!ControllerServeurJeu::isLocalServer())
    {
        GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
        wGestionnaireReseau->demarrerNouvelleConnection("MasterServer", wMasterServerIP, TCP);

        PaquetEvent* wPaquet = (PaquetEvent*)wGestionnaireReseau->creerPaquet(EVENT);
        wPaquet->setEventCode(GAME_SERVER_AUTHENTICATION_REQUEST);
        wPaquet->setMessage("");

        SPSocket wSocketMasterServer = wGestionnaireReseau->getSocket("MasterServer", TCP);
        wSocketMasterServer->setOnConnectionCallback([wGestionnaireReseau, wPaquet]()->void {
            wGestionnaireReseau->envoyerPaquet("MasterServer", wPaquet, TCP);
        });
    }
}


char* ObtenirAdresseIpLocaleAssociee(const std::string& pIpAssociee)
{
    std::string wTemp = GestionnaireReseau::obtenirInstance()->getAdresseIPLocaleAssociee(pIpAssociee);
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
void* DeamonTick( void *arg )
{
    FacadeServeurJeu* wFacade = (FacadeServeurJeu*) arg;
    // Interval en ms, ne changera jamais
    int wTickInterval = wFacade->getTickInterval();


    // Not Accurate
    clock_t wLastTick = clock();
    while(true)
    {
        clock_t wElapsed = clock()-wLastTick;
        if(wElapsed > wTickInterval)
        {
            wFacade->animer((float)wElapsed/1000.0f);
            wLastTick = clock();
        }
        FacadePortability::sleep(1); // Enlever cette ligne pour etre plus precis, mais va bouffer le CPU
    }
}



void SetStartMapDownloadCallback( ManagedStartMapDownload pCallback )
{
    FacadeServeurJeu::getInstance()->setCallbackManagedStartDownload(pCallback);
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

    instance_->libererMemoire();
    delete instance_;
    instance_ = 0;

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
    HANDLE wHandle;
    FacadePortability::createThread(wHandle, DeamonTick, this);
    if(wHandle==NULL)
    {
        throw std::runtime_error("Erreur lors de la creation du thread de reception");
    }

    mCallbackManagedStartDownload = NULL;
    
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
/// @fn void FacadeServeurJeu::libererMemoire()
///
/// Liberation de la memoire
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void FacadeServeurJeu::libererMemoire()
{

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