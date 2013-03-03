

#include <iostream>
#include <sstream>
#include <wtypes.h>
#include <stdexcept>

typedef void (*FunctionFunc)();
char flagChar = '-';



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



int main(int argc, char* argv[])  {

    std::cout << "--------- Serveur Jeu ---------" << std::endl << std::endl;

    std::string wMasterServerIP = "173.177.0.193"; // Adresse par defaut du serveur maitre

    
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







    HINSTANCE wDLL = LoadLibrary(TEXT("RazerGame.dll"));

    if (!wDLL)
    {
        std::cout << "Erreur a l'ouverture de la DLL (RazerGame.dll)." << std::endl;
        system("pause");
        return -1;
    }


    FunctionFunc InitDLLServeurJeu = (FunctionFunc)GetProcAddress(wDLL, "InitDLLServeurJeu");

    // Appel a la fonction d'initialisation
    InitDLLServeurJeu();

    
    // On s'authentifie au serveur maitre (localhost opur l'instant)
    // Connexion au serveur maitre









    // To prevend the server to exit without killing it
    while(true)
    {
        getchar(); // Pause until enter is pressed
    }

    FreeLibrary(wDLL);

    return 0;
}
