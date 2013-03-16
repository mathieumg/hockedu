
#include "FacadeServeurJeu.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\ControllerServeurJeu.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetUserStatus.h"
#include "..\Reseau\PaquetHandlers\PacketHandler.h"
#include "..\Reseau\Paquets\PaquetEvent.h"
#include "..\Reseau\UsinePaquets\UsinePaquetGameStatus.h"
#include "..\Reseau\UsinePaquets\UsinePaquetAuthentificationServeurJeu.h"
#include "..\reseau\UsinePaquets\UsinePaquetMaillet.h"
#include "..\Reseau\UsinePaquets\UsinePaquetGameCreation.h"

void InitDLLServeurJeu(std::string& wMasterServerIP)
{
    // Initialisation du GestionnaireReseau
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();

    wGestionnaireReseau->setController(new ControllerServeurJeu);
    wGestionnaireReseau->initGameServer();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau(CHAT_MESSAGE, new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    wGestionnaireReseau->ajouterOperationReseau(USER_STATUS, new PacketHandlerUserStatus, new UsinePaquetUserStatus);
    wGestionnaireReseau->ajouterOperationReseau(GAME_STATUS, new PacketHandlerGameStatus, new UsinePaquetGameStatus);
    //wGestionnaireReseau->ajouterOperationReseau(AUTHENTIFICATION_SERVEUR_JEU, new PacketHandlerAuthentificationServeurJeu, new UsinePaquetAuthentificationServeurJeu);
    wGestionnaireReseau->ajouterOperationReseau(MAILLET, new PacketHandlerMaillet, new UsinePaquetMaillet);
    wGestionnaireReseau->ajouterOperationReseau(GAME_CREATION_REQUEST, new PacketHandlerGameCreation, new UsinePaquetGameCreation);

    wGestionnaireReseau->demarrerNouvelleConnection("MasterServer", wMasterServerIP, TCP);

    PaquetEvent* wPaquet = (PaquetEvent*)wGestionnaireReseau->creerPaquet(EVENT);
    wPaquet->setEventCode(GAME_SERVER_AUTHENTICATION_REQUEST);
    wPaquet->setMessage("");

    SPSocket wSocketMasterServer = wGestionnaireReseau->getSocket("MasterServer", TCP);
    const int MAX_ATTEMPTS = 10;
    int nbTries = 0;
    while (wSocketMasterServer->getConnectionState() == NOT_CONNECTED && nbTries < MAX_ATTEMPTS)
    {
        FacadePortability::sleep(1000);
        ++nbTries;
    }
    if(wSocketMasterServer->getConnectionState() == CONNECTING || wSocketMasterServer->getConnectionState() == CONNECTED)
    {
        wGestionnaireReseau->envoyerPaquet("MasterServer", wPaquet, TCP);
    }
    
}




