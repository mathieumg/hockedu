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
    // Size de Paquet
    int mTaillePaquet = (int)(Paquet::sequenceIdentification.length()+1 + GestionnaireReseau::longueurMaxOperationReseau);
    // Size de PaquetBase
    mTaillePaquet += sizeof(PaquetBase::CompteurNumeroPaquet) + sizeof(mTaillePaquet);
    return mTaillePaquet; // TODO: REMOVE HARDCODED VALUE
}
