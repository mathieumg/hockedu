#include "PacketHandler.h"
#include "../PacketReader.h"
#include "../PacketBuilder.h"
#include "../Paquets/Paquet.h"


HeaderPaquet PacketHandler::handlePacketHeaderReception( PacketReader& pPacketReader )
{
    int wArraySize = (uint32_t) (Paquet::sequenceIdentification.size()+1);
    uint8_t* wBuffer = new uint8_t[wArraySize];
    pPacketReader.readString(wBuffer, wArraySize);
    std::string wStringIdentification = std::string((char*)wBuffer);
    delete wBuffer;
    if (wStringIdentification != Paquet::sequenceIdentification)
        return HeaderPaquet();
    HeaderPaquet wHeaderPaquet;
    wHeaderPaquet.type = (PacketTypes)pPacketReader.readInteger();
    wHeaderPaquet.numeroPaquet = pPacketReader.readInteger();
    wHeaderPaquet.taillePaquet = pPacketReader.readInteger();
    return wHeaderPaquet;
}

void PacketHandler::handlePacketPreparation( Paquet* pPaquet, PacketBuilder& pPacketBuilder )
{
    pPacketBuilder.includeStringLength(false);
    PacketTypes wType = pPaquet->getOperation();
    pPacketBuilder << pPaquet->sequenceIdentification << wType;
    // Size du paquet courant
    pPacketBuilder << pPaquet->getNumeroPaquet() << getPacketSize(pPaquet);
    pPacketBuilder.includeStringLength(true);
    handlePacketPreparationSpecific(pPaquet, pPacketBuilder);
}

int PacketHandler::getPacketSize( Paquet* pPaquet ) const
{
    // Size de Paquet
    int wTaillePaquet = getPacketHeaderSize();
    wTaillePaquet += getPacketSizeSpecific(pPaquet);
    return wTaillePaquet;
}

int PacketHandler::getPacketHeaderSize()
{
    int wTailleHeader = (int)(Paquet::sequenceIdentification.length()+1 + sizeof(BASE));
    wTailleHeader += sizeof(Paquet::CompteurNumeroPaquet) + sizeof(wTailleHeader);
    return wTailleHeader;
}



