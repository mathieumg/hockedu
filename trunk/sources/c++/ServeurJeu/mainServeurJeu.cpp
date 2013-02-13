

#include <iostream>
#include "ControllerServeurJeu.h"

int main(void)  {

    std::cout << "--------- Serveur Jeu ---------" << std::endl << std::endl;
    /*
    GestionnaireReseauServeurJeu::obtenirInstance();
	GestionnaireReseau* wGestReseau = GestionnaireReseau::obtenirInstance();

    
    GestionnaireReseau::obtenirInstance()->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    GestionnaireReseau::obtenirInstance()->setController(new ControllerServeurJeu());

    

    */






    // To prevend the server to exit without killing it
    while(true)
    {
        getchar(); // Pause until enter is pressed
    }

    return 0;
}
