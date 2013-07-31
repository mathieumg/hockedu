#include "PacketHandler.h"
#include "../PacketReader.h"
#include "../PacketBuilder.h"
#include "../Paquets/Paquet.h"


HeaderPaquet PacketHandler::handlePacketHeaderReception( PacketReader& pPacketReader )
{
    unsigned int seqId = pPacketReader.readUnsignedInteger();
//     if ( Paquet::sequenceIdentification != seqId)
//         return HeaderPaquet();

    HeaderPaquet wHeaderPaquet;
    wHeaderPaquet.type = (PacketTypes)pPacketReader.readInteger();
    wHeaderPaquet.numeroPaquet = pPacketReader.readInteger();
    wHeaderPaquet.taillePaquet = pPacketReader.readInteger();
    return wHeaderPaquet;
}

void PacketHandler::handlePacketPreparation( Paquet* pPaquet, PacketBuilder& pPacketBuilder )
{
    PacketTypes wType = pPaquet->getOperation();
    // cast le type en int pour s'assurer d'utiliser 4 octet et que la lecture de l'autre cote soit cohérente
    pPacketBuilder << Paquet::sequenceIdentification << (int)wType;
    pPacketBuilder << pPaquet->getNumeroPaquet() << (int)0;
    handlePacketPreparationSpecific(pPaquet, pPacketBuilder);
    pPacketBuilder.setSize();
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

