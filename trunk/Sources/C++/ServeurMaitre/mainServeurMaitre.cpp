

#include <iostream>
#include "../Reseau/GestionnaireReseauServeurMaitre.h"
#include "ControllerServeurMaitre.h"
#include "../Reseau/PaquetHandlers/PacketHandlerChatMessage.h"
#include "../Reseau/UsinePaquets/UsinePaquetChatMessage.h"
#include "FacadeServeurMaitre.h"

int main(void)  {

    std::cout << "--------- Serveur Maitre ---------" << std::endl << std::endl;

    GestionnaireReseauServeurMaitre::obtenirInstance();
	GestionnaireReseau* wGestReseau = GestionnaireReseau::obtenirInstance();

    GestionnaireReseau::obtenirInstance()->ajouterOperationReseau(CHAT_MESSAGE, new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    GestionnaireReseau::obtenirInstance()->setController(new ControllerServeurMaitre());

    FacadeServeurMaitre::obtenirInstance();









    // To prevend the server to exit without killing it
    while(true)
    {
        getchar(); // Pause until enter is pressed
    }

    return 0;
}
