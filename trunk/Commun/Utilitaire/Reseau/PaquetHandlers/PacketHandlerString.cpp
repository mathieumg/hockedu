#include "PacketHandlerString.h"
#include <iostream>
#include <sstream>
#include "Reseau/Paquets\PaquetString.h"

void PacketHandlerString::handlePacketReceptionSpecific(PacketReader& pPacketReader)
{
    std::string str =  std::string((char*)pPacketReader.readString());
    std::cout << str << std::endl;
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
