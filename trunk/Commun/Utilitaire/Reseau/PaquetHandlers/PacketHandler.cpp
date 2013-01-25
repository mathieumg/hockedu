#include "PacketHandler.h"
#include "..\PacketReader.h"
#include "..\PacketBuilder.h"
#include "..\Paquets\Paquet.h"


HeaderPaquet PacketHandler::handlePacketHeaderReception( PacketReader& pPacketReader )
{
    std::string wStringIdentification = std::string((char*)pPacketReader.readString(Paquet::sequenceIdentification.size()+1));
    if (wStringIdentification != Paquet::sequenceIdentification)
        return HeaderPaquet();
    HeaderPaquet hp;
    const int longueurType = GestionnaireReseau::longueurMaxOperationReseau;
    strncpy_s(hp.type, longueurType, (const char*)pPacketReader.readString(longueurType), longueurType);
    return handlePacketHeaderReceptionBase(pPacketReader, hp);
}

void PacketHandler::handlePacketPreparation( Paquet* pPaquet, PacketBuilder& pPacketBuilder )
{
    pPacketBuilder.includeStringLength(false);
    std::string wType = pPaquet->getOperation();
    pPacketBuilder << pPaquet->sequenceIdentification << wType;
    pPacketBuilder.addPadding(GestionnaireReseau::longueurMaxOperationReseau - wType.length() - 1);
    int wSize = getPacketSize(pPaquet);
    handlePacketPreparationBase(pPaquet, pPacketBuilder, wSize);
    pPacketBuilder.includeStringLength(true);
    handlePacketPreparationSpecific(pPaquet, pPacketBuilder);
}



int PacketHandler::getPacketSize( Paquet* pPaquet ) const
{
    return 0; // Ne devrait pas etre appele
}



