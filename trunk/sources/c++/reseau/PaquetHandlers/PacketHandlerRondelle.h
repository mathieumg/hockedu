
#pragma once
#include "PacketHandler.h"

class PacketHandlerRondelle :public PacketHandler {
protected:
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL); // M�thode pour lire les donn�es d'un paquet pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // M�thode pour construire le paquet � envoyer
    virtual int getPacketSizeSpecific( Paquet* pPaquet ) const;

};