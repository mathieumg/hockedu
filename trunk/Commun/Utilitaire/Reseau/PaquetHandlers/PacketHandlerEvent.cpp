

#include "PacketHandlerEvent.h"
#include "..\Paquets\PaquetEvent.h"


void PacketHandlerEvent::handlePacketReceptionSpecific(PacketReader& pPacketReader)
{

    PaquetEvent* wPaquet = (PaquetEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet("Event");
    uint32_t wArraySize = pPacketReader.readInteger();
    uint8_t* wBuffer = new uint8_t[wArraySize];
    pPacketReader.readString(wBuffer, wArraySize);
    wPaquet->setMessage((char*)wBuffer);
    delete wBuffer;
    wPaquet->setErrorCode(pPacketReader.readInteger());

    GestionnaireReseau::obtenirInstance()->transmitEvent(wPaquet->getErrorCode(), wPaquet->getMessage());


    delete wPaquet;

}

void PacketHandlerEvent::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;

    pPacketBuilder << wPaquet->getMessage()
        << wPaquet->getErrorCode();

}



int PacketHandlerEvent::getPacketSize( Paquet* pPaquet ) const
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;

    return PacketBuilder::getSizeForString(wPaquet->getMessage())
        + PacketBuilder::getSizeForInt()
        + PacketHandlerBase::getPacketSize(pPaquet);
}
