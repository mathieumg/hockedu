

#include <iostream>
#include <sstream>
#include <wtypes.h>
#include <stdexcept>
#include "../Interface/FacadeServeurJeu.h"
#include "Menu.h"
#include "MenuOptionCallVoid.h"
#include <list>
#include <time.h>




const char flagChar = '-';



// Fonction pour les flags qui ont un string apres la flag
// Ex: -m 192.168.0.0.1
void parseParamWithArg(std::string& pOutParam, int pLoopCounter, int argc, char* argv[])
{
    if(pLoopCounter == argc-1) // Si dernier arg, erreur
    {
        throw std::runtime_error("Valeur manquante pour le flag " + std::string(argv[pLoopCounter]));
    }

    // Flag pour donner l'adresse du serveur maitre
    pOutParam = argv[pLoopCounter+1];

    if(pOutParam[0] == flagChar)
    {
        throw std::runtime_error("Valeur manquante pour le flag " + std::string(argv[pLoopCounter]));
    }
}
std::string wMasterServerIP = "173.231.120.124"; // Adresse par defaut du serveur maitre

void TerminateServer()
{
    CleanUp();
    exit(0);
}

void DoNothing()
{

}

void connect(std::string ip)
{
    std::cout << "Connecting to master server on adress : " << ip << std::endl;
    ConnectMasterServer(ip);
}

void ReconnectDefaultMaster()
{
    CleanUp();
    InitDLLServeurJeu();
    connect(wMasterServerIP);
}

void ManualConnect()
{
    CleanUp();
    InitDLLServeurJeu();
    std::cout << "Enter ip adress : ";
    char wInput [256];
    fgets ( wInput, 256, stdin );
    connect(wInput);
}

void ConnectLocal()
{
    CleanUp();
    InitDLLServeurJeu();
    // Par defaut on essaie de mettre une adresse de l'ecole (132.xxx.xxx.xxx)
    // Apres on essaie de mettre une adresse residentielle (192.168.xxx.xxx)
    // Sinon on met 127.0.0.1 (on ne change rien)

    // small hack to work on polytechnic extern adress
    const unsigned int preferredSchool  = (132<<24) + (207 << 16);  

    //192.168.0.0 - 192.168.255.255
    const unsigned int preferredLan1Min = (192<<24) + (168 << 16);
    const unsigned int preferredLan1Max = (192<<24) + (168 << 16) + (255 << 8) + (255);

    //172.16.0.0 - 172.31.255.255
    const unsigned int preferredLan2Min = (172<<24) + (16 << 16);
    const unsigned int preferredLan2Max = (172<<24) + (31 << 16) + (255 << 8) + (255);

    const char* wLocalIp = ObtenirAdresseIpLocaleAssociee(preferredSchool,preferredSchool);
    if(strlen(wLocalIp) == 0)
    {
        delete[] wLocalIp;
        wLocalIp = ObtenirAdresseIpLocaleAssociee(preferredLan1Min,preferredLan1Max);
        if(strlen(wLocalIp)  == 0)
        {
            wLocalIp = ObtenirAdresseIpLocaleAssociee(preferredLan2Min,preferredLan2Max);
            if(strlen(wLocalIp)  == 0)
            {
                delete[] wLocalIp;
                connect("127.0.0.1"); // fallback if not a on a lan network
                return;
            }
        }
    }
    connect(wLocalIp);
    delete[] wLocalIp;
}

int mainServeurJeu(const char* pMasterServerIp)  {
    
    std::cout << "--------- Serveur Jeu ---------" << std::endl << std::endl;
    //std::string wMasterServerIP = "192.168.0.105"; // Adresse par defaut du serveur maitre
    
    //********** Liste des flags
    // -mIP     Adresse IPv4 du serveur Maitre
    //**********

    // Appel a la fonction d'initialisation
    std::string ipArg = pMasterServerIp;
    if(ipArg == "local")
    {
        ConnectLocal();
    }
    else if(ipArg != "")
    {
        CleanUp();
        InitDLLServeurJeu();
        connect(ipArg);
    }
    else
    {
        // Connect master server
        ReconnectDefaultMaster();
    }

    // stress test
//     srand( (unsigned int) clock()+(unsigned int) time);
//     while(1)
//     {
//         if(rand()&1)
//         {
//             ConnectLocal();
//         }
//         else
//         {
//             ReconnectDefaultMaster();
//         }
//         int t = rand()%100+300;
//         Sleep(t);
//     }



    // Affichage du menu
    Menu wMenu("Choose an option");
    wMenu.addMenuOption(0, new MenuOptionCallVoid("Connect Local", ConnectLocal));
    wMenu.addMenuOption(1, new MenuOptionCallVoid("Manual Connect Master Server", ManualConnect));
    wMenu.addMenuOption(2, new MenuOptionCallVoid("Reconnect Master Server", ReconnectDefaultMaster));
    wMenu.addMenuOption(3, new MenuOptionCallVoid("Exit Server", TerminateServer));

    while(true)
    {
        wMenu.displayAndPick();
    }
    
    return 0;
}
