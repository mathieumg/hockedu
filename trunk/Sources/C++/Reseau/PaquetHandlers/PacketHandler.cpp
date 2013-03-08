#include "PacketHandler.h"
#include "../PacketReader.h"
#include "../PacketBuilder.h"
#include "../Paquets/Paquet.h"


HeaderPaquet PacketHandler::handlePacketHeaderReception( PacketReader& pPacketReader )
{
    char wStringIdentification[Paquet::sequenceIdentificationLength];
    pPacketReader.readString((uint8_t*)wStringIdentification, Paquet::sequenceIdentificationLength);

    if ( Paquet::sequenceIdentification != wStringIdentification)
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
    pPacketBuilder << Paquet::sequenceIdentification << wType;
    // Size du paquet courant
    pPacketBuilder << pPaquet->getNumeroPaquet() << getPacketSize(pPaquet);
    pPacketBuilder.includeStringLength(true);
    handlePacketPreparationSpecific(pPaquet, pPacketBuilder);
}

int PacketHandler::getPacketSize( Paquet* pPaquet ) const
{
    // Size de Paquet
    return getPacketHeaderSize() + getPacketSizeSpecific(pPaquet);
}




