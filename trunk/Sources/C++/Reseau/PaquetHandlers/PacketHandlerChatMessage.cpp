
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
#include "../Paquets/PaquetChatMessage.h"
#include "../RelayeurMessage.h"
#include <time.h>
#include <sstream>
#include <iomanip>


void PacketHandlerChatMessage::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetChatMessage* wPaquet = (PaquetChatMessage*) UsinePaquet::creerPaquet(CHAT_MESSAGE);
        int wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMessage((char*) wBuffer);
        delete wBuffer;
        wPaquet->setTimestamp( pPacketReader.read64bInteger());
        wPaquet->setIsTargetGroup(pPacketReader.readBool());

        wArraySize = pPacketReader.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setGroupName((char*) wBuffer);
        delete wBuffer;

        wArraySize = pPacketReader.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setOrigin((char*) wBuffer);
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }

}

void PacketHandlerChatMessage::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    if("" == wPaquet->getOrigin())
    {
        throw ExceptionReseau("Le champ origine du PaquetChatMessage est necessaire.");
    }

    pPacketBuilder << wPaquet->getMessage()
        << wPaquet->getTimestamp()
        << wPaquet->IsTargetGroup()
        << wPaquet->getGroupName()
        <<wPaquet->getOrigin();

}



int PacketHandlerChatMessage::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;


    return  getSizeForString(wPaquet->getMessage())
        + getSizeFor64bInteger()
        + getSizeForBool()
        +  getSizeForString(wPaquet->getGroupName())
        +  getSizeForString(wPaquet->getOrigin());

}
