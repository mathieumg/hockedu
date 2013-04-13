

#include <iostream>
#include "../Reseau/GestionnaireReseauServeurMaitre.h"
#include "ControllerServeurMaitre.h"
#include "../Reseau/PaquetHandlers/PacketHandler.h"
#include "FacadeServeurMaitre.h"
#include "CommunicateurBD.h"

const char flagChar = '-';

/// ne pas rename, utis/ comme extern ailleur
extern std::string NETWORK_LOG_FILE_NAME;
extern std::string NETWORK_PACKET_SENT_FILE_NAME;
extern std::string NETWORK_PACKET_RECV_FILE_NAME;

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

#ifdef LINUX
    //daemon(0,0);
#endif
    NETWORK_LOG_FILE_NAME =         "SVRMASTER_LOG_";
    NETWORK_PACKET_SENT_FILE_NAME = "SVRMASTER_PACKET_SENT_";
    NETWORK_PACKET_RECV_FILE_NAME = "SVRMASTER_PACKET_RECV_";
    std::cout << "--------- Serveur Maitre ---------" << std::endl << std::endl;

    //********** Liste des flags
    // -bHost       Hostname de la BD
    // -bName       Nom de la BD
    // -bUser       Username a utiliser pour la connexion a la BD
    // -bPassword   Password a utiliser pour la connexion a la BD
    //**********
    std::string wBDHost     = "hockedu.com:3306";
    std::string wBDUser     = "mapary";
    std::string wDBPassword = "mz41wo13";
    std::string wDBName     = "poly_hockedu";

    try
    {
        for(int i = 0; i < argc; i++)
        {
            if(argv[i][0] == flagChar) // Si flag
            {
                std::string wFlag = argv[i]+1; // On laisse tomber le premier char car c'est le flagchar

                if(wFlag == "bHost")
                {
                    parseParamWithArg(wBDHost, i, argc, argv);
                }
                else if(wFlag == "bName")
                {
                    parseParamWithArg(wDBName, i, argc, argv);
                }
                else if(wFlag == "bUser")
                {
                    parseParamWithArg(wBDUser, i, argc, argv);
                }
                else if(wFlag == "bPassword")
                {
                    parseParamWithArg(wDBPassword, i, argc, argv);
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

    // Setup CommunicateurBD
    CommunicateurBD::setup(wBDHost, wDBName, wBDUser, wDBPassword);


    GestionnaireReseau::communicationPort = GestionnaireReseau::communicationPortMasterServer;
    GestionnaireReseauServeurMaitre::obtenirInstance();
	GestionnaireReseau* wGestReseau = GestionnaireReseau::obtenirInstance();


    GestionnaireReseau::obtenirInstance()->setController(new ControllerServeurMaitre());

    FacadeServeurMaitre::obtenirInstance();

    // To prevend the server to exit without killing it
    while(true)
    {
        getchar();
    }

    return 0;
}
