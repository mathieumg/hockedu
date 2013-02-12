
#pragma once
#include "PacketHandlerBase.h"

class PacketHandlerUserStatus :public PacketHandlerBase {
protected:
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader); // Méthode pour lire les données d'un paquet pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Méthode pour construire le paquet à envoyer

    virtual int getPacketSize( Paquet* pPaquet ) const;

};