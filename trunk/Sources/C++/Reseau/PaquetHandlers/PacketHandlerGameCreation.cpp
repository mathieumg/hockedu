#include "PacketHandler.h"
#include "../Paquets/PaquetGameCreation.h"

void PacketHandlerGameCreation::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameCreation* wPaquet = (PaquetGameCreation*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_CREATION_REQUEST);

        // GameId
        wPaquet->setGameId(pPacketReader.readInteger());

        // ServerId
        wPaquet->setServerId(pPacketReader.readUnsignedInteger());

        // Username
        int wArraySize      = pPacketReader.readInteger();
        uint8_t* wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setUsername(std::string((char*) wBuffer));
        delete wBuffer;

        // GameName
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setGameName(std::string((char*) wBuffer));
        delete wBuffer;

        // MapName
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMapName(std::string((char*) wBuffer));
        delete wBuffer;

        // Password
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setPassword(std::string((char*) wBuffer));
        delete wBuffer;

        // ServerIP
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setServerIP(std::string((char*) wBuffer));
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }
}

void PacketHandlerGameCreation::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameCreation* wPaquet   = (PaquetGameCreation*) pPaquet;

    pPacketBuilder  << wPaquet->getGameId()
                    << wPaquet->getServerId()
                    << wPaquet->getUsername()
                    << wPaquet->getGameName()
                    << wPaquet->getMapName()
                    << wPaquet->getPassword()
                    << wPaquet->getServerIP();
}



int PacketHandlerGameCreation::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetGameCreation* wPaquet = (PaquetGameCreation*) pPaquet;

    return getSizeForString(wPaquet->getUsername()) // Username
        +  getSizeForString(wPaquet->getGameName()) // GameName
        +  getSizeForString(wPaquet->getMapName()) // MapName
        +  getSizeForString(wPaquet->getPassword()) // Password
        +  getSizeForString(wPaquet->getServerIP()) // Server IP
        +  getSizeForInt() // GameId
        +  getSizeForInt(); // Server Id
}