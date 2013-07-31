
#include "PacketHandler.h"
#include "PaquetChatMessage.h"


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

    pPacketBuilder << wPaquet->getMessage()
        << wPaquet->getTimestamp()
        << wPaquet->IsTargetGroup()
        << wPaquet->getGroupName()
        <<wPaquet->getOrigin();

}

