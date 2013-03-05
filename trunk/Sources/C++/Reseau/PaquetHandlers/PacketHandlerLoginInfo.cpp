
#include <iostream>
#include <sstream>
#include "PacketHandlerLoginInfo.h"
#include "../Paquets/PaquetLoginInfo.h"
#include "Utilitaire.h"
#include "../ObjetsGlobaux/JoueurServeurs.h"
#ifdef SERVER
#include "../ServeurMaitre/FacadeServeurMaitre.h"
#endif


void PacketHandlerLoginInfo::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{

    if(pRunnable)
    {
        PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) GestionnaireReseau::obtenirInstance()->creerPaquet(LOGIN_INFO);

        int wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setUsername((char*) wBuffer);
        delete wBuffer;


        wArraySize = pPacketReader.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setPassword((char*) wBuffer);
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();


//     #ifdef SERVER
//
//         // On envoie un event au gestionnaire reseau
//         GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTING, wPaquet->getUsername());
//
//         // On sauvearde le joueur
//         JoueurServeurs* wJoueur = new JoueurServeurs(wPaquet->getUsername());
//         FacadeServeurMaitre::obtenirInstance()->saveJoueurConnecting(wJoueur);
//
//         // On traite la demande avec la BD
//
//
//     #endif
    }

}

void PacketHandlerLoginInfo::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;


    pPacketBuilder << wPaquet->getUsername()
        << wPaquet->getPassword();

}



int PacketHandlerLoginInfo::getPacketSize( Paquet* pPaquet ) const
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;


    return  getSizeForString(wPaquet->getUsername())
        +  getSizeForString(wPaquet->getPassword());

}
