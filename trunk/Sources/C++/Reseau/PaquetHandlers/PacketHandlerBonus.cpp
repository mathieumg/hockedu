
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
#include "../Paquets/PaquetBonus.h"


void PacketHandlerBonus::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable)
{
    if(pRunnable)
    {
        PaquetBonus* wPaquet = (PaquetBonus*) UsinePaquet::creerPaquet(BONUS);

        wPaquet->setGameId(pPacketReader.readInteger());

        wPaquet->setBonusType((BonusType)pPacketReader.readInteger());
        wPaquet->setBonusAction((PaquetBonusAction)pPacketReader.readInteger());

        float wX = pPacketReader.readFloat();
        float wY = pPacketReader.readFloat();
        wPaquet->setBonusPosition(Vecteur2(wX, wY));

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }
}

void PacketHandlerBonus::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetBonus* wPaquet = (PaquetBonus*) pPaquet;

    pPacketBuilder << wPaquet->getGameId()
                   << wPaquet->getBonusType()
                   << wPaquet->getBonusAction()
                   << wPaquet->getBonusPosition()[VX]
                   << wPaquet->getBonusPosition()[VY];
}

