#include "PacketHandler.h"
#include "../PacketReader.h"
#include "../PacketBuilder.h"
#include "../Paquets/Paquet.h"


HeaderPaquet PacketHandler::handlePacketHeaderReception( PacketReader& pPacketReader )
{
    char wStringIdentification[Paquet::sequenceIdentificationLength];
#if HANDLE_CHARACTERE_0
    pPacketReader.readString((uint8_t*)wStringIdentification, Paquet::sequenceIdentificationLength - 1);
#else
    pPacketReader.readString((uint8_t*)wStringIdentification, Paquet::sequenceIdentificationLength);
#endif

    if ( Paquet::sequenceIdentification != wStringIdentification)
        return HeaderPaquet();
    HeaderPaquet wHeaderPaquet;
    wHeaderPaquet.type = (PacketTypes)pPacketReader.readInteger();
    wHeaderPaquet.numeroPaquet = pPacketReader.readInteger();
    wHeaderPaquet.taillePaquet = pPacketReader.readInteger();
    return wHeaderPaquet;
}

void PacketHandler::handlePacketPreparation( Paquet* pPaquet, PacketBuilder& pPacketBuilder )
{
    pPacketBuilder.includeStringLength(false);
    PacketTypes wType = pPaquet->getOperation();
    pPacketBuilder << Paquet::sequenceIdentification << wType;
    pPacketBuilder << pPaquet->getNumeroPaquet() << getPacketSize(pPaquet);
    pPacketBuilder.includeStringLength(true);
    handlePacketPreparationSpecific(pPaquet, pPacketBuilder);
}

int PacketHandler::getPacketSize( Paquet* pPaquet ) const
{
    // Size de Paquet
    return getPacketHeaderSize() + getPacketSizeSpecific(pPaquet);
}


PacketHandler* PaquetHandlersArray[NB_PACKET_TYPES] =
{
    new PacketHandlerConnAutomatique,//CONN_AUTOMATIQUE,
    new PacketHandlerEvent          ,//EVENT,
    new PacketHandlerLoginInfo      ,//LOGIN_INFO,
    new PacketHandlerChatMessage    ,//CHAT_MESSAGE,
    new PacketHandlerUserStatus     ,//USER_STATUS,
    new PacketHandlerGameStatus     ,//GAME_STATUS,
    new PacketHandlerGameCreation   ,//GAME_CREATION_REQUEST,
    new PacketHandlerGameConnection ,//GAME_CONNECTION,
    new PacketHandlerMaillet        ,//MAILLET,
    new PacketHandlerRondelle       ,//RONDELLE,
    new PacketHandlerGameEvent      ,//GAME_EVENT,
    new PacketHandlerPortal         ,//PORTAL,
    new PacketHandlerBonus          ,//BONUS,
};

