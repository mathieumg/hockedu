
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
#include "..\Paquets\PaquetPortal.h"


void PacketHandlerPortal::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable)
{
    if(pRunnable)
    {
        PaquetPortal* wPaquet = (PaquetPortal*) UsinePaquet::creerPaquet(PORTAL);

        wPaquet->setGameId(pPacketReader.readInteger());
        float wX = pPacketReader.readFloat();
        float wY = pPacketReader.readFloat();
        wPaquet->setPosition(Vecteur3(wX, wY, 0));

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }
}

void PacketHandlerPortal::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetPortal* wPaquet = (PaquetPortal*) pPaquet;

    pPacketBuilder << wPaquet->getGameId()
                   << wPaquet->getPosition()[VX]
                   << wPaquet->getPosition()[VY];
}

