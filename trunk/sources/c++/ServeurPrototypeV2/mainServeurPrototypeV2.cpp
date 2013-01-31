

#include <iostream>
#include "..\Reseau\GestionnaireReseauServeur.h"
#include "Reseau\Socket.h"
#include "Reseau\GestionnaireReseau.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "ControllerServeurPrototype.h"

int main(void)  {

    std::cout << "--------- Serveur Prototype (Chat) ---------" << std::endl << std::endl;

    GestionnaireReseauServeur::obtenirInstance();
	GestionnaireReseau* wGestReseau = GestionnaireReseau::obtenirInstance();

    
    GestionnaireReseau::obtenirInstance()->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    GestionnaireReseau::obtenirInstance()->setController(new ControllerServeurPrototype());

    std::cout << "Server running, Press Enter to exit at any time.";
    getchar(); // Pause until enter is pressed

    return 0;
}
