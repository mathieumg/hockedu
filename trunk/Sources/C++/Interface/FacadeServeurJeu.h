
#ifdef CREATING_DLL
#define DLLEXPORT_SERVEUR_JEU __declspec(dllexport)
#elif defined(LOADING_DLL)
#define DLLEXPORT_SERVEUR_JEU __declspec(dllimport)
#else
#define DLLEXPORT_SERVEUR_JEU
#endif

#include <string>
#include "../../C#/UIHeavyClient/UIHeavyClient/Enum_Declarations.h"


typedef void (*CallbackDone)(std::string pFilepath);
typedef void (*ManagedStartMapDownload)(int pUserId, int pMapId, CallbackDone pCallbackFromManaged);

/// ne pas rename, utis/ comme extern ailleur
extern std::string NETWORK_LOG_FILE_NAME;
extern std::string NETWORK_PACKET_SENT_FILE_NAME;
extern std::string NETWORK_PACKET_RECV_FILE_NAME;
extern "C" 
{

    DLLEXPORT_SERVEUR_JEU void InitDLLServeurJeu();
    DLLEXPORT_SERVEUR_JEU void ConnectMasterServer(const std::string& wMasterServerIP);
    DLLEXPORT_SERVEUR_JEU const char* ObtenirAdresseIpLocaleAssociee(unsigned int minAdress, unsigned int maxAdress);
    DLLEXPORT_SERVEUR_JEU void SetStartMapDownloadCallback(ManagedStartMapDownload pCallback);
    DLLEXPORT_SERVEUR_JEU void CleanUp();


    class ControllerInterface;
    class Paquet;
   
}



//////////////////////////////////////////////////////////////////////////////
/// @file FacadeServeurJeu.h
/// @author Mathieu Parent
/// @date 2013-03-18
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __APPLICATION_FACADESERVEURJEU_H__
#define __APPLICATION_FACADESERVEURJEU_H__



///////////////////////////////////////////////////////////////////////////
/// @class FacadeServeurJeu
/// @brief Classe qui constitue une interface (une fa�ade) sur l'ensemble
///        du mod�le du serveur de jeu
///
/// @author Mathieu Parent
/// @date 2013-03-18
///////////////////////////////////////////////////////////////////////////
class FacadeServeurJeu
{
public:
    

    /// Obtient l'instance unique de la classe.
    static FacadeServeurJeu* getInstance();
    /// Lib�re l'instance unique de la classe.
    static void libererInstance();
    /// Checks if the instance exists
    static bool Exists(){return !!instance_;}



    /// Anime la sc�ne.
    void animer( const float& temps);


    /// Send event to the controller
    static void transmitEvent( EventCodes pCode, ... );

    bool downloadMap(int pUserId, int pMapId, CallbackDone pCallbackDone) const;

private:
    /// Constructeur par d�faut.
    FacadeServeurJeu();
    /// Destructeur.
    ~FacadeServeurJeu();
    /// Constructeur copie.  D�clar� mais non d�fini pour �viter qu'il soit
    /// g�n�r� par le compilateur.
    FacadeServeurJeu(const FacadeServeurJeu&);
    /// Op�rateur d'assignation.  D�clar� mais non d�fini pour �viter qu'il
    /// soit g�n�r� par le compilateur.
    FacadeServeurJeu& operator =(const FacadeServeurJeu&);

    /// Pointeur vers l'instance unique de la classe.
    static FacadeServeurJeu* instance_;

    // interval entre les ticks (en ms)
    int mTickInterval;

    void* mDaemon;
    /// Accesseurs
public:
    inline int getTickInterval() const { return mTickInterval; }
};








#endif // __APPLICATION_FACADESERVEURJEU_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
