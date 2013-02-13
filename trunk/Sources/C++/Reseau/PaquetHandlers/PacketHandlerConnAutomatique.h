
#pragma once
#include "PacketHandlerBase.h"

class PacketHandlerConnAutomatique :public PacketHandlerBase {
public:
    virtual ~PacketHandlerConnAutomatique() {}
    PacketHandlerConnAutomatique() {}
protected:
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL); // Méthode pour lire les données d'un paquet pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Méthode pour construire le paquet à envoyer
};