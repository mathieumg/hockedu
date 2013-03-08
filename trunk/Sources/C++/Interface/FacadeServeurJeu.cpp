
#include "FacadeServeurJeu.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\ControllerServeurJeu.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetUserStatus.h"
#include "..\Reseau\PaquetHandlers\PacketHandler.h"
#include "..\Reseau\UsinePaquets\UsinePaquetGameStatus.h"
#include "..\Reseau\UsinePaquets\UsinePaquetAuthentificationServeurJeu.h"
#include "..\reseau\UsinePaquets\UsinePaquetMaillet.h"

void InitDLLServeurJeu()
{
    // Initialisation du GestionnaireReseau
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();

    wGestionnaireReseau->setController(new ControllerServeurJeu);
    wGestionnaireReseau->initServer();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau(CHAT_MESSAGE, new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    wGestionnaireReseau->ajouterOperationReseau(USER_STATUS, new PacketHandlerUserStatus, new UsinePaquetUserStatus);
    wGestionnaireReseau->ajouterOperationReseau(GAME_STATUS, new PacketHandlerGameStatus, new UsinePaquetGameStatus);
    wGestionnaireReseau->ajouterOperationReseau(AUTHENTIFICATION_SERVEUR_JEU, new PacketHandlerAuthentificationServeurJeu, new UsinePaquetAuthentificationServeurJeu);
    wGestionnaireReseau->ajouterOperationReseau(MAILLET, new PacketHandlerMaillet, new UsinePaquetMaillet);

    
}




