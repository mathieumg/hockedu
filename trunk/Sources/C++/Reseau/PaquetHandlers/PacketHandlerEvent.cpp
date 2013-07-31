

#include "PacketHandler.h"
#include "PaquetEvent.h"


void PacketHandlerEvent::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetEvent* wPaquet = (PaquetEvent*) UsinePaquet::creerPaquet(EVENT);
        uint32_t wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMessage((char*)wBuffer);
        delete wBuffer;
        wPaquet->setEventCode(pPacketReader.readInteger());

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
        << wPaquet->getEventCode();

}
