
#pragma once
#include "Reseau\PaquetHandlers\PacketHandlerBase.h"

class PacketHandlerChatUserStatus :public PacketHandlerBase {
protected:
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader); // M�thode pour lire les donn�es d'un paquet pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // M�thode pour construire le paquet � envoyer

    virtual int getPacketSize( Paquet* pPaquet ) const;

};