

#include <iostream>
#include <sstream>
#include <wtypes.h>
#include <stdexcept>
#include "../Interface/FacadeServeurJeu.h"
#include "Menu.h"
#include "MenuOptionCallVoid.h"
#include <list>

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
std::string wMasterServerIP = "127.0.0.1"; // Adresse par defaut du serveur maitre

void TerminateServer()
{
    exit(0);
}

void DoNothing()
{

}

void ReconnectMaster()
{
    ConnectMasterServer(wMasterServerIP);
}

int main(int argc, char* argv[])  {
    
    std::cout << "--------- Serveur Jeu ---------" << std::endl << std::endl;
    //std::string wMasterServerIP = "192.168.0.105"; // Adresse par defaut du serveur maitre
    
    //********** Liste des flags
    // -mIP     Adresse IPv4 du serveur Maitre
    //**********
    bool wUsedArgForMasterServerIp = false;
    try
    {
        for(int i = 0; i < argc; i++) 
        {
            if(argv[i][0] == flagChar) // Si flag
            {
                std::string wFlag = argv[i]+1; // On laisse tomber le premier char car c'est le flagchar
                
                if(wFlag == "mIP")
                {
                    parseParamWithArg(wMasterServerIP, i, argc, argv);
                    wUsedArgForMasterServerIp = true;
                }
                else
                {
                    throw std::runtime_error("Flag invalide");
                }
                ++i;
            }
        }
    }
    catch(std::runtime_error& e)
    {
        std::cout << "Erreur dans les parametres" << std::endl;
        std::cout << e.what() << std::endl;
        return -1;
    }

    // Appel a la fonction d'initialisation
    InitDLLServeurJeu();

    if(!wUsedArgForMasterServerIp)
    {
        // Par defaut on essaie de mettre une adresse de l'ecole (132.xxx.xxx.xxx)
        // Apres on essaie de mettre une adresse residentielle (192.168.xxx.xxx)
        // Sinon on met 127.0.0.1 (on ne change rien)

        try 
        {
            char* wIp = ObtenirAdresseIpLocaleAssociee("132.207.0.0");
            wMasterServerIP = std::string(wIp);
            delete wIp;
        }
        catch(...)
        {
            try 
            {
                char* wIp = ObtenirAdresseIpLocaleAssociee("192.168.0.0");
                wMasterServerIP = std::string(wIp);
                delete wIp;
            }
            catch(...) {}
        }
        

    }

    // Connect master server
    ReconnectMaster();

    // Affichage du menu
    Menu wMenu("Choose an option");
    wMenu.addMenuOption(0, new MenuOptionCallVoid("Do Nothing", DoNothing));
    wMenu.addMenuOption(1, new MenuOptionCallVoid("Reconnect Master Server", ReconnectMaster));
    wMenu.addMenuOption(2, new MenuOptionCallVoid("Exit Server", TerminateServer));

    while(true)
    {
        wMenu.displayAndPick();
    }
    
    return 0;
}
