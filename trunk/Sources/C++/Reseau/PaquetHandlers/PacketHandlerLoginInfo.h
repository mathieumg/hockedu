
#pragma once
#include "PacketHandlerBase.h"

class PacketHandlerLoginInfo :public PacketHandlerBase {
protected:
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL); // M�thode pour lire les donn�es d'un paquet pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // M�thode pour construire le paquet � envoyer

    virtual int getPacketSize( Paquet* pPaquet ) const;

};