
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
#include "..\Paquets\PaquetMaillet.h"


void PacketHandlerMaillet::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetMaillet* wPaquet = (PaquetMaillet*) GestionnaireReseau::obtenirInstance()->creerPaquet(MAILLET);

        wPaquet->setGameId(pPacketReader.readInteger());

        float wX = pPacketReader.readFloat();
        float wY = pPacketReader.readFloat();
        wPaquet->setPosition(Vecteur3(wX, wY, 0));

        wPaquet->setEstAGauche(pPacketReader.readBool());


        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }


}

void PacketHandlerMaillet::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;


    pPacketBuilder  << wPaquet->getGameId()
                    << wPaquet->getPosition()[VX]
                    << wPaquet->getPosition()[VY]
                    << wPaquet->getEstAGauche();

}



int PacketHandlerMaillet::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetMaillet* wPaquet = (PaquetMaillet*) pPaquet;

    return getSizeForInt() + 2* getSizeForFloat() + getSizeForBool();

}
