

#include "PacketHandlerEvent.h"
#include "..\Paquets\PaquetEvent.h"


void PacketHandlerEvent::handlePacketReceptionSpecific(PacketReader& pPacketReader)
{
    
    PaquetEvent* wPaquet = (PaquetEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet("Error");
    wPaquet->setMessage((char*) pPacketReader.readString());
    wPaquet->setErrorCode(pPacketReader.readInteger());


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
