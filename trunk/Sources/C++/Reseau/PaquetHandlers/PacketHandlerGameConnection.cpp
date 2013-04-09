#include "PacketHandler.h"
#include "../Paquets/PaquetGameConnection.h"

void PacketHandlerGameConnection::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameConnection* wPaquet = (PaquetGameConnection*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_CONNECTION);

        // GameId
        wPaquet->setGameId(pPacketReader.readInteger());

        // GameServerId
        wPaquet->setGameServerId(pPacketReader.readInteger());

        // Username
        int wArraySize      = pPacketReader.readInteger();
        uint8_t* wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setUsername(std::string((char*) wBuffer));
        delete wBuffer;

        // Password
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setPassword(std::string((char*) wBuffer));
        delete wBuffer;

        // ConnectionState
        wPaquet->setConnectionState((GameConnectionState) pPacketReader.readInteger());
        
        // Server Ip
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setGameServerIp(std::string((char*) wBuffer));
        delete wBuffer;

        // Server Ip
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMapName(std::string((char*) wBuffer));
        delete wBuffer;

        // GameTime
        wPaquet->setGameTime(pPacketReader.readFloat());

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }
}

void PacketHandlerGameConnection::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameConnection* wPaquet   = (PaquetGameConnection*) pPaquet;

    pPacketBuilder  << wPaquet->getGameId()
                    << wPaquet->getGameServerId()
                    << wPaquet->getUsername()
                    << wPaquet->getPassword()
                    << wPaquet->getConnectionState()
                    << wPaquet->getGameServerIp()
                    << wPaquet->getMapName()
                    << wPaquet->getGameTime();

}



int PacketHandlerGameConnection::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetGameConnection* wPaquet = (PaquetGameConnection*) pPaquet;

    return getSizeForInt() // GameId
        + getSizeForInt() // GameServerId
        + getSizeForString(wPaquet->getUsername()) // Username
        + getSizeForString(wPaquet->getPassword()) // Password
        + getSizeForInt() // ConnectionState
        + getSizeForString(wPaquet->getGameServerIp()) // Server Ip
        + getSizeForString(wPaquet->getMapName())
        + getSizeForFloat()
        ;

}