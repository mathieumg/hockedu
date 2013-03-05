#include "PacketHandlerTest.h"
#include <iostream>
#include <sstream>
#include "../GestionnaireReseau.h"
#include "../Paquets/PaquetTest.h"

void PacketHandlerTest::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{

    if(pRunnable)
    {
        PaquetTest* wPaquet = (PaquetTest*) GestionnaireReseau::obtenirInstance()->creerPaquet(TEST);

        int wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMessage((char*)wArraySize);
        wPaquet->setInt(pPacketReader.readInteger());
        wPaquet->setFloat(pPacketReader.readFloat());
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();


    }
}

void PacketHandlerTest::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetTest* wPaquet = ((PaquetTest*)pPaquet);
    pPacketBuilder << wPaquet->getMessage() << wPaquet->getInt() << wPaquet->getFloat();
}



int PacketHandlerTest::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetTest* wPaquet = (PaquetTest*) pPaquet;

    return  getSizeForString(wPaquet->getMessage()) +
        getSizeForInt() +
        getSizeForFloat();
}
