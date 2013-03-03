
#include <iostream>
#include <sstream>
#include "PacketHandlerAuthentificationServeurJeu.h"
#include "../Paquets/PaquetAuthentificationServeurJeu.h"
#include "Utilitaire.h"
#include "../ObjetsGlobaux/JoueurServeurs.h"
#ifdef SERVER
#include "../ServeurMaitre/FacadeServeurMaitre.h"
#endif


void PacketHandlerAuthentificationServeurJeu::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{

    if(pRunnable)
    {
        //PaquetAuthentificationServeurJeu* wPaquet = (PaquetAuthentificationServeurJeu*) GestionnaireReseau::obtenirInstance()->creerPaquet(AUTHENTIFICATION_SERVEUR_JEU);
// 
//         int wArraySize = pPacketReader.readInteger();
//         uint8_t* wBuffer = new uint8_t[wArraySize];
//         pPacketReader.readString(wBuffer, wArraySize);
//         wPaquet->setUsername((char*) wBuffer);
//         delete wBuffer;
// 
// 
//         wArraySize = pPacketReader.readInteger();
//         wBuffer = new uint8_t[wArraySize];
//         pPacketReader.readString(wBuffer, wArraySize);
//         wPaquet->setPassword((char*) wBuffer);
//         delete wBuffer;
// 
//         wPaquet->setRunnable(pRunnable);
//         wPaquet->run();
// 

    }

}


void PacketHandlerAuthentificationServeurJeu::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetAuthentificationServeurJeu* wPaquet = (PaquetAuthentificationServeurJeu*) pPaquet;


//     pPacketBuilder << wPaquet->getUsername()
//         << wPaquet->getPassword();

}




int PacketHandlerAuthentificationServeurJeu::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetAuthentificationServeurJeu* wPaquet = (PaquetAuthentificationServeurJeu*) pPaquet;
    return 0;
// 
//     return PacketBuilder::getSizeForString(wPaquet->getUsername())
//         + PacketBuilder::getSizeForString(wPaquet->getPassword());

}
