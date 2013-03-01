

#include <iostream>
#include <sstream>
#include <wtypes.h>

typedef void (*FunctionFunc)();

int main(void)  {

    std::cout << "--------- Serveur Jeu ---------" << std::endl << std::endl;
    HINSTANCE wDLL = LoadLibrary(TEXT("RazerGame.dll"));

    if (!wDLL)
    {
        std::cout << "Erreur a l'ouverture de la DLL (RazerGame.dll)." << std::endl;
        system("pause");
        return -1;
    }


    FunctionFunc InitDLLServeurJeu = (FunctionFunc)GetProcAddress(wDLL, "InitDLLServeurJeu");

    // Appel a la fonction d'initialisation
    InitDLLServeurJeu();

    
    // On s'authentifie au serveur maitre (localhost opur l'instant)
    // Connexion au serveur maitre









    // To prevend the server to exit without killing it
    while(true)
    {
        getchar(); // Pause until enter is pressed
    }

    FreeLibrary(wDLL);

    return 0;
}
