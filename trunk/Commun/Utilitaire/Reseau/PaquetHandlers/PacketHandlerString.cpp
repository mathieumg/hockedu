#include "PacketHandlerString.h"
#include <iostream>
#include <sstream>
#include "Reseau/Paquets\PaquetString.h"

void PacketHandlerString::handlePacketReceptionSpecific(PacketReader& pPacketReader)
{
    int wArraySize =pPacketReader.readInteger();
    uint8_t* wBuffer = new uint8_t[wArraySize];
    pPacketReader.readString(wBuffer, wArraySize);
    std::string str =  std::string((char*)wBuffer);
    //std::cout << str << std::endl;
    delete wBuffer;
}

void PacketHandlerString::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    pPacketBuilder << ((PaquetString*)pPaquet)->getMessage();
}



int PacketHandlerString::getPacketSize(Paquet* pPaquet) const
{
    PaquetString* wPaquet = (PaquetString*) pPaquet;

    return PacketBuilder::getSizeForString(wPaquet->getMessage()) + PacketHandlerBase::getPacketSize(pPaquet);
}
