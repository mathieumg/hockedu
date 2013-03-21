#include "PacketHandler.h"
#include "../Paquets/PaquetGameRegistration.h"

void PacketHandlerGameRegistration::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameRegistration* wPaquet = (PaquetGameRegistration*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_REGISTRATION);

        //ServerId
        wPaquet->setServerId(pPacketReader.readInteger());

        // GameId
        wPaquet->setGameId(pPacketReader.readInteger());

        // GameName
        int wArraySize       = pPacketReader.readInteger();
        uint8_t* wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setGameName(std::string((char*) wBuffer));
        delete wBuffer;

        // MapName
        wArraySize = pPacketReader.readInteger();
        wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMapName(std::string((char*) wBuffer));
        delete wBuffer;

        // Username
        wArraySize = pPacketReader.readInteger();
        wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setUsername(std::string((char*) wBuffer));
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }
}

void PacketHandlerGameRegistration::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameRegistration* wPaquet   = (PaquetGameRegistration*) pPaquet;

    pPacketBuilder  << wPaquet->getServerId()
                    << wPaquet->getGameId()
                    << wPaquet->getGameName()
                    << wPaquet->getMapName()
                    << wPaquet->getUsername();
}

int PacketHandlerGameRegistration::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetGameRegistration* wPaquet = (PaquetGameRegistration*) pPaquet;

    return getSizeForInt() // ServerId
        +  getSizeForInt() // GameId
        +  getSizeForString(wPaquet->getGameName()) // GameName
        +  getSizeForString(wPaquet->getMapName()) // MapName
        +  getSizeForString(wPaquet->getUsername());  // Username
}