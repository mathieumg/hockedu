


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "..\Reseau\GestionnaireReseauClientLourd.h"
#include "Reseau\Socket.h"
#include <string>
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include <time.h>
#include "Menu.h"
#include "MenuOption.h"
#include <sstream>
#include "..\Reseau\ControllerCPP.h"


using namespace std;

int main(void)  {

    
    GestionnaireReseauClientLourd::obtenirInstance();

    ControllerCPP* wControlleur = new ControllerCPP();
    GestionnaireReseau::obtenirInstance()->setController(wControlleur);


    std::cout << "--------- Client Prototype (Chat) ---------" << std::endl << std::endl;

    std::cout << "Entrer un nom d'utilisateur: ";
    char wName[50];
    cin.getline(wName, 50);

    Menu wMenu("Choose the IP of the server");
    wMenu.addMenuOption(0, new MenuOption("127.0.0.1"));
	wMenu.addMenuOption(1, new MenuOption("192.168.0.196"));
	wMenu.addMenuOption(2, new MenuOption("192.168.0.151"));
    wMenu.addMenuOption(3, new MenuOption("173.177.0.193"));
    std::string wServerIp = wMenu.displayAndPick();

    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection(wName, wServerIp, UDP);


	/*GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection(wName, wServerIp, TCP);

    std::cout << "Entrer un message et appuyez sur Entree: " << std::endl;

    while(true)
    {
        
        char wBuffer[2000];
        cin.getline(wBuffer, 2000);

        // Remonte std::cout d'une ligne (pas parfait, mais fait la job)
        HANDLE ConHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if(ConHandle != INVALID_HANDLE_VALUE)
        {
            CONSOLE_SCREEN_BUFFER_INFO SBInfo;
            GetConsoleScreenBufferInfo(ConHandle, &SBInfo);

            COORD Pos;
            Pos.X=0;
            Pos.Y=SBInfo.dwCursorPosition.Y-1;

            SetConsoleCursorPosition(ConHandle,Pos);
        }
			PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatMessage");

			

			wPaquet->setMessage(wBuffer);
			wPaquet->setIsTargetGroup(true);
			wPaquet->setGroupName("groupe");
			wPaquet->setTimestamp(time(0));
			wPaquet->setOrigin(wName);

			GestionnaireReseau::obtenirInstance()->envoyerPaquet(wName, wPaquet,TCP);
		


        
    }

    */

    system("pause");
    delete wControlleur;

    return 0;
}