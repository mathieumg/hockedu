

#include <iostream>
#include "..\Reseau\GestionnaireReseauServeur.h"
#include "Reseau\Socket.h"
#include "Reseau\GestionnaireReseau.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"

int main(void)  {

    std::cout << "--------- Serveur Prototype (Chat) ---------" << std::endl << std::endl;

    GestionnaireReseauServeur::obtenirInstance();

    Socket* wSocket = new Socket("127.0.0.1", 5010, TCP);


    GestionnaireReseau::obtenirInstance()->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);

    std::cout << "Server running, Press Enter to exit at any time.";
    getchar(); // Pause until enter is pressed

    return 0;
}