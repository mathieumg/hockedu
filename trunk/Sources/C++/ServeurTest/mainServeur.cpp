


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




    //Socket* wSocket = new Socket("127.0.0.1", 5010, TCP);


    //wGestionnaireReseau->saveSocket("bob", wSocket);

	system("pause");

	return 0;
}