
#pragma once
#include "PacketHandlerBase.h"

class PacketHandlerLoginInfo :public PacketHandlerBase {
protected:
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL); // Méthode pour lire les données d'un paquet pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Méthode pour construire le paquet à envoyer

    virtual int getPacketSize( Paquet* pPaquet ) const;

};