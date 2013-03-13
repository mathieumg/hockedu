
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
// #include "../Paquets/PaquetChatMessage.h"
// #include "../RelayeurMessage.h"
// #include <time.h>
// #include <sstream>
// #include <iomanip>
// #ifndef _SERVER
// #include "../GestionnaireReseauClientLourd.h"
// #endif
#include "..\Paquets\PaquetRondelle.h"


void PacketHandlerRondelle::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetRondelle* wPaquet = (PaquetRondelle*) GestionnaireReseau::obtenirInstance()->creerPaquet(RONDELLE);
//         int wArraySize = pPacketReader.readInteger();
//         uint8_t* wBuffer = new uint8_t[wArraySize];
//         pPacketReader.readString(wBuffer, wArraySize);
//         wPaquet->setMessage((char*) wBuffer);
//         delete wBuffer;
//         wPaquet->setTimestamp( pPacketReader.read64bInteger());
//         wPaquet->setIsTargetGroup(pPacketReader.readBool());
// 
//         wArraySize = pPacketReader.readInteger();
//         wBuffer = new uint8_t[wArraySize];
//         pPacketReader.readString(wBuffer, wArraySize);
//         wPaquet->setGroupName((char*) wBuffer);
//         delete wBuffer;
// 
//         wArraySize = pPacketReader.readInteger();
//         wBuffer = new uint8_t[wArraySize];
//         pPacketReader.readString(wBuffer, wArraySize);
//         wPaquet->setOrigin((char*) wBuffer);
//         delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }



}

void PacketHandlerRondelle::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetRondelle* wPaquet = (PaquetRondelle*) pPaquet;

//     if("" == wPaquet->getOrigin())
//     {
//         throw ExceptionReseau("Le champ origine du PaquetChatMessage est necessaire.");
//     }
// 
//     pPacketBuilder << wPaquet->getMessage()
//         << wPaquet->getTimestamp()
//         << wPaquet->IsTargetGroup()
//         << wPaquet->getGroupName()
//         <<wPaquet->getOrigin();

}



int PacketHandlerRondelle::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetRondelle* wPaquet = (PaquetRondelle*) pPaquet;


//     return  getSizeForString(wPaquet->getMessage())
//         + getSizeFor64bInteger()
//         + getSizeForBool()
//         +  getSizeForString(wPaquet->getGroupName())
//         +  getSizeForString(wPaquet->getOrigin());
    return 0;

}
