

#include "PacketHandlerEvent.h"
#include "../Paquets/PaquetEvent.h"


void PacketHandlerEvent::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetEvent* wPaquet = (PaquetEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(EVENT);
        uint32_t wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMessage((char*)wBuffer);
        delete wBuffer;
        wPaquet->setErrorCode(pPacketReader.readInteger());

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }


//     GestionnaireReseau::obtenirInstance()->transmitEvent(wPaquet->getErrorCode(), wPaquet->getMessage());
//
//
//     wPaquet->removeAssociatedQuery(); // delete

}

void PacketHandlerEvent::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;

    pPacketBuilder << wPaquet->getMessage()
        << wPaquet->getErrorCode();

}



int PacketHandlerEvent::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetEvent* wPaquet = (PaquetEvent*) pPaquet;

    return  getSizeForString(wPaquet->getMessage())
        + getSizeForInt();
}
