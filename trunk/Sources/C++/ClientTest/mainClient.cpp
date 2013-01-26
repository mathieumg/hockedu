


#include <stdlib.h>
#include <stdio.h>
#include "Reseau\GestionnaireReseau.h"
#include <iostream>
#include "Reseau\PaquetHandlers\PacketHandlerString.h"
#include "Reseau\UsinePaquets\UsinePaquetString.h"
#include <sstream>
#include "Reseau\Paquets\PaquetTest.h"

using namespace std;

int main(void)  {

	
	printf("client\n");
	
    cout << "Entrer le nom a utiliser:";
    string wNom;
    cin >> wNom;

    cout << endl << "Entrer l'adresse du serveur ou rien pour localhost:";
    char wIpServeur[15];
    cin.ignore();
    cin.getline(wIpServeur, 15, '\n');

    if(!strcmp(wIpServeur, ""))
    {
        strncpy_s(wIpServeur, 15, "127.0.0.1", 15);
    }


	GestionnaireReseau::setNetworkMode(CLIENT);
	GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
	
	Socket* wSocket = new Socket(wIpServeur, 5010, TCP);


	wGestionnaireReseau->saveSocket(wNom, wSocket);
	
    Sleep(2000);

    while(true)
    {
        PaquetTest* wPaquet = (PaquetTest*) wGestionnaireReseau->creerPaquet("Test");
        wPaquet->setMessage("!!!!!!!!!!! BOB EST BEAU !!!!!!!!!!!");
        wPaquet->setInt(1);
        wPaquet->setFloat(1.0f/3.0f);
        try
        {
            wGestionnaireReseau->envoyerPaquet(wSocket, wPaquet);
        }
        catch(ExceptionReseau&)
        {
            // On ne peut pas envoyer le paquet pour le moment

        }

    }
    
	system("pause");
	return 0;
}