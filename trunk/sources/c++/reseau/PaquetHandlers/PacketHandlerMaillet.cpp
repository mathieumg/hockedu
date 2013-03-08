
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
#include "..\Paquets\PaquetMaillet.h"


void PacketHandlerMaillet::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetMaillet* wPaquet = (PaquetMaillet*) GestionnaireReseau::obtenirInstance()->creerPaquet(MAILLET);

        float wX = pPacketReader.readFloat();
        float wY = pPacketReader.readFloat();
        wPaquet->setPosition(Vecteur3(wX, wY, 0));

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }


}

void PacketHandlerMaillet::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;

    pPacketBuilder  << wPaquet->getPosition()[VX]
                    << wPaquet->getPosition()[VY];

}



int PacketHandlerMaillet::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;

    return 2* getSizeForFloat();

}
