


#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include "Reseau\GestionnaireReseau.h"
#include "Reseau\Socket.h"

using namespace std;

int main(void)  {


	printf("serveur\n");

    GestionnaireReseau::setNetworkMode(SERVER);
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();
    wGestionnaireReseau->init();



    //Socket* wSocket = new Socket("127.0.0.1", GestionnaireReseau::communicationPort, TCP);


    //wGestionnaireReseau->saveSocket("bob", wSocket);

	system("pause");

	return 0;
}