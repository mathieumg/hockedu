#include "PacketHandler.h"
#include <iostream>
#include <sstream>
#include "../Paquets/PaquetConnAutomatique.h"
#include <sstream>
#include "../GestionnaireReseau.h"

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