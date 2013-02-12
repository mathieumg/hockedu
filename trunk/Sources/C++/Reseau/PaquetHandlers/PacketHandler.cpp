#include "PacketHandler.h"
#include "..\PacketReader.h"
#include "..\PacketBuilder.h"
#include "..\Paquets\Paquet.h"


HeaderPaquet PacketHandler::handlePacketHeaderReception( PacketReader& pPacketReader )
{
    int wArraySize = (uint32_t) (Paquet::sequenceIdentification.size()+1);
    uint8_t* wBuffer = new uint8_t[wArraySize];
    pPacketReader.readString(wBuffer, wArraySize);
    std::string wStringIdentification = std::string((char*)wBuffer);
    delete wBuffer;
    if (wStringIdentification != Paquet::sequenceIdentification)
        return HeaderPaquet();
    HeaderPaquet hp;
    hp.type = (PacketTypes)pPacketReader.readInteger();
    handlePacketHeaderReceptionBase(pPacketReader, hp);
    return hp;
}

void PacketHandler::handlePacketPreparation( Paquet* pPaquet, PacketBuilder& pPacketBuilder )
{
    pPacketBuilder.includeStringLength(false);
    PacketTypes wType = pPaquet->getOperation();
    pPacketBuilder << pPaquet->sequenceIdentification << wType;
    int wSize = getPacketSize(pPaquet);
    handlePacketPreparationBase(pPaquet, pPacketBuilder, wSize);
    pPacketBuilder.includeStringLength(true);
    handlePacketPreparationSpecific(pPaquet, pPacketBuilder);
}



int PacketHandler::getPacketSize( Paquet* pPaquet ) const
{
    return 0; // Ne devrait pas etre appele
}



