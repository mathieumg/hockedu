#include "PacketHandlerTest.h"
#include <iostream>
#include <sstream>
#include "Reseau/Paquets\PaquetTest.h"
#include "Reseau/GestionnaireReseau.h"

void PacketHandlerTest::handlePacketReceptionSpecific(PacketReader& pPacketReader)
{
    PaquetTest* wPaquet = (PaquetTest*) GestionnaireReseau::obtenirInstance()->creerPaquet("Test");

    int wArraySize = pPacketReader.readInteger();
    uint8_t* wBuffer = new uint8_t[wArraySize];
    pPacketReader.readString(wBuffer, wArraySize);
    wPaquet->setMessage((char*)wArraySize);
    wPaquet->setInt(pPacketReader.readInteger());
    wPaquet->setFloat(pPacketReader.readFloat());


    std::cout << std::endl << "Message: "  << wPaquet->getMessage() << std::endl;
    std::cout << "Int: " << wPaquet->getInt() << std::endl;
    std::cout << "Float: " << wPaquet->getFloat() << std::endl;

    delete wBuffer;
    wPaquet->removeAssociatedQuery(); // delete
}

void PacketHandlerTest::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetTest* wPaquet = ((PaquetTest*)pPaquet);
    pPacketBuilder << wPaquet->getMessage() << wPaquet->getInt() << wPaquet->getFloat();
}



int PacketHandlerTest::getPacketSize( Paquet* pPaquet ) const
{
    PaquetTest* wPaquet = (PaquetTest*) pPaquet;

    return PacketBuilder::getSizeForString(wPaquet->getMessage()) + PacketBuilder::getSizeForInt() + PacketBuilder::getSizeForFloat() + PacketHandlerBase::getPacketSize(pPaquet);
}
