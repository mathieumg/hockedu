#include "PacketHandler.h"
#include "../Paquets/PaquetConnAutomatique.h"

void PacketHandlerConnAutomatique::handlePacketReceptionSpecific( PacketReader& pPacketReader, PaquetRunnableFunc pRunnable /* = NULL*/ )
{
    if(pRunnable)
    {
	    // Implementer


    }
}


void PacketHandlerConnAutomatique::handlePacketPreparationSpecific( Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
	// Stuff a Charles
}


int PacketHandlerConnAutomatique::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    // Stuff a Charles
    return 0;
}