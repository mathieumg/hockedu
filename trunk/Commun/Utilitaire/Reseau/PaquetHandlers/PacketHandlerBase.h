#pragma once
#include "PacketHandler.h"
class PacketHandlerBase :
    public PacketHandler
{
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader) {}
protected:
    virtual HeaderPaquet& handlePacketHeaderReceptionBase(PacketReader& pPacketReader, HeaderPaquet& pHeaderPaquet); // M�thode pour lire le header d'un paquet
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder) {}
    virtual void handlePacketPreparationBase(Paquet* pPaquet, PacketBuilder& pPacketBuilder, int& pPaquetSize);  // M�thode pour construire le paquet � envoyer pour un paquet de base
    virtual int getPacketSize(Paquet* pPaquet) const;
    

};

