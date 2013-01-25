#include "PacketHandlerBase.h"
#include "..\Paquets\PaquetBase.h"


HeaderPaquet& PacketHandlerBase::handlePacketHeaderReceptionBase(PacketReader& pPacketReader, HeaderPaquet& pHeaderPaquet)
{
    pHeaderPaquet.numeroPaquet = pPacketReader.readInteger();
    pHeaderPaquet.taillePaquet = pPacketReader.readInteger();
    return pHeaderPaquet;
}


void PacketHandlerBase::handlePacketPreparationBase(Paquet* pPaquet, PacketBuilder& pPacketBuilder, int& pPaquetSize)
{
    // Size du paquet courant
    pPacketBuilder << ((PaquetBase*)pPaquet)->getNumeroPaquet() << pPaquetSize;
}



int PacketHandlerBase::getPacketSize(Paquet* pPaquet) const
{
    return 44; // TODO: REMOVE HARDCODED VALUE
}
