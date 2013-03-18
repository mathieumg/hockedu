

#include <iostream>
#include <sstream>
#include <wtypes.h>
#include <stdexcept>
#include "../Interface/FacadeServeurJeu.h"
#include "Menu.h"
#include "MenuOptionCallVoid.h"

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








void TerminateServer()
{
    exit(0);
}

void DoNothing()
{

}




int main(int argc, char* argv[])  {
    
    std::cout << "--------- Serveur Jeu ---------" << std::endl << std::endl;

    std::string wMasterServerIP = "127.0.0.1"; // Adresse par defaut du serveur maitre

    
    //********** Liste des flags
    // -mIP     Adresse IPv4 du serveur Maitre
    //**********
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
    InitDLLServeurJeu(wMasterServerIP);


    // Affichage du menu
    Menu wMenu("Choose an option");
    wMenu.addMenuOption(0, new MenuOptionCallVoid("Do Nothing", DoNothing));
    wMenu.addMenuOption(1, new MenuOptionCallVoid("Exit Server", TerminateServer));

    while(true)
    {
        wMenu.displayAndPick();
    }

    
    return 0;
}
