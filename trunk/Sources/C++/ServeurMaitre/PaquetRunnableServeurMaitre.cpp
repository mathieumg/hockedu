

#include "PaquetRunnableServeurMaitre.h"
#include "..\ServeurMaitre\FacadeServeurMaitre.h"
#include "..\Reseau\Paquets\PaquetLoginInfo.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\ObjetsGlobaux\JoueurServeurs.h"


/// ***** PAR CONVENTION, METTRE Master A LA FIN DU NOM DES DELEGATES


int PaquetRunnable::RunnableLoginInfoServerMaster( Paquet* pPaquet )
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;

    // Code pour l'authentification des users du cote du serveur maitre


    // On envoie un event au gestionnaire reseau
    GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTING, wPaquet->getUsername());

    // On sauvearde le joueur
    JoueurServeurs* wJoueur = new JoueurServeurs(wPaquet->getUsername());
    FacadeServeurMaitre::obtenirInstance()->saveJoueurConnecting(wJoueur);

    // On traite la demande avec la BD



    return 0;
}




