


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "..\Reseau\GestionnaireReseauClientLourd.h"
#include "Reseau\Socket.h"
#include <string>
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include <time.h>


using namespace std;

int main(void)  {

    
    GestionnaireReseauClientLourd::obtenirInstance();


    std::cout << "--------- Client Prototype (Chat) ---------" << std::endl << std::endl;

    std::cout << "Entrer un nom d'utilisateur: ";
    char wName[50];
    cin.getline(wName, 50);




    Socket* wSocket = new Socket("127.0.0.1", 5010, TCP);
    GestionnaireReseau::obtenirInstance()->saveSocket(wName, wSocket);

    std::cout << "Entrer un message et appuyez sur Entree: " << std::endl;

    while(true)
    {
        
        char wBuffer[2000];
        cin.getline(wBuffer, 2000);

        PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatMessage");

        wPaquet->setMessage(wBuffer);
        wPaquet->setIsTargetGroup(true);
        wPaquet->setGroupName("groupe");
        wPaquet->setTimestamp(time(0));
        wPaquet->setOrigin(wName);

        GestionnaireReseau::obtenirInstance()->envoyerPaquet(wSocket, wPaquet);


        
    }

    /*
    for(int i=0; i<5000; ++i)
    {
        PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatMessage");

        wPaquet->setMessage("Wow salut gang");
        wPaquet->setIsTargetGroup(true);
        wPaquet->setGroupName("groupe");
        wPaquet->setTimestamp(i*2);
        wPaquet->setOrigin(wName);

        GestionnaireReseau::obtenirInstance()->envoyerPaquet(wSocket, wPaquet);
        Sleep(10);
    }
*/

    system("pause");

    return 0;
}