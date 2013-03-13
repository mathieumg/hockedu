#include "PacketHandler.h"
#include "../Paquets/PaquetGameCreation.h"

void PacketHandlerGameStatus::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameCreation* wPaquet = (PaquetGameCreation*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_CREATION_REQUEST);

        // Player1Name
        int wArraySize      = pPacketReader.readInteger();
        uint8_t* wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setUsername(std::string((char*) wBuffer));
        delete wBuffer;

        // Player2Name
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setGameName(std::string((char*) wBuffer));
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }



}

void PacketHandlerGameStatus::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameCreation* wPaquet   = (PaquetGameCreation*) pPaquet;

    pPacketBuilder << wPaquet->getUsername()
        << wPaquet->getGameName();

}



int PacketHandlerGameStatus::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetGameStatus* wPaquet = (PaquetGameStatus*) pPaquet;
    PartieServeurs* wGameInfos = wPaquet->getGameInfos();

    return getSizeForInt() // GameId
        +  getSizeForString(wGameInfos->getPlayer1Name()) // Player1 Name
        +  getSizeForString(wGameInfos->getPlayer2Name()) // Player2 Name
        + getSizeForInt() // Player1 Score
        + getSizeForInt() // Player2 Score
        + getSizeForInt() // Time
        ;

}