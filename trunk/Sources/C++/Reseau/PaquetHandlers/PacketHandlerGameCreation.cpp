#include "PacketHandler.h"
#include "../Paquets/PaquetGameCreation.h"

void PacketHandlerGameCreation::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameCreation* wPaquet = (PaquetGameCreation*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_CREATION_REQUEST);

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

        // GameId
        wPaquet->setGameId(pPacketReader.readInteger());

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }
}

void PacketHandlerGameCreation::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameCreation* wPaquet   = (PaquetGameCreation*) pPaquet;

    pPacketBuilder  << wPaquet->getUsername()
                    << wPaquet->getGameName()
                    << wPaquet->getMapName()
                    << wPaquet->getGameId();

}



int PacketHandlerGameCreation::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetGameCreation* wPaquet = (PaquetGameCreation*) pPaquet;

    return getSizeForString(wPaquet->getUsername()) // Username
        +  getSizeForString(wPaquet->getGameName()) // GameName
        +  getSizeForString(wPaquet->getMapName()) // MapName
        + getSizeForInt() // GameId
        ;

}