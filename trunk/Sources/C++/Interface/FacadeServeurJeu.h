
#ifdef CREATING_DLL
#define DLLEXPORT_SERVEUR_JEU __declspec(dllexport)
#elif defined(LOADING_DLL)
#define DLLEXPORT_SERVEUR_JEU __declspec(dllimport)
#else
#define DLLEXPORT_SERVEUR_JEU
#endif

#include <string>
// Test pour l'appel de la DLL depuis le C#
extern "C" 
{
    DLLEXPORT_SERVEUR_JEU void InitDLLServeurJeu(std::string& pMasterServerIP);

    class ControllerInterface;
    class Paquet;
    //////////////////////////////////////////////////////////////////////////
    /// Network functions
    //DLLEXPORT_SERVEUR_JEU void envoyerPaquet( Paquet* pPaquet );
    //__declspec(dllexport) void recevoirPaquet( Paquet* pPaquet );
    /// 
    //////////////////////////////////////////////////////////////////////////
   
}