

#include <iostream>
#include "..\Reseau\PaquetHandlers\PacketHandlerChatMessage.h"
#include "Reseau\GestionnaireReseau.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "ControllerServeurMaitre.h"
#include "..\Reseau\GestionnaireReseauServeurMaitre.h"

int main(void)  {

    std::cout << "--------- Serveur Maitre ---------" << std::endl << std::endl;

    GestionnaireReseauServeurMaitre::obtenirInstance();
	GestionnaireReseau* wGestReseau = GestionnaireReseau::obtenirInstance();

    
    GestionnaireReseau::obtenirInstance()->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    GestionnaireReseau::obtenirInstance()->setController(new ControllerServeurMaitre());

    





    // To prevend the server to exit without killing it
    while(true)
    {
        getchar(); // Pause until enter is pressed
    }

    return 0;
}
