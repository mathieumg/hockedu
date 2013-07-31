
#include "PacketHandler.h"
#include "PaquetGameStatus.h"


void PacketHandlerGameStatus::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameStatus* wPaquet = (PaquetGameStatus*) UsinePaquet::creerPaquet(GAME_STATUS);
        PartieServeurs* wGameInfos = wPaquet->getGameInfos();

        // ServerId
        wPaquet->getGameInfos()->setServerId(pPacketReader.readInteger());

        // GameId
        wPaquet->getGameInfos()->setUniqueGameId(pPacketReader.readInteger());

        // Player1Name
        int wArraySize      = pPacketReader.readInteger();
        uint8_t* wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wGameInfos->setPlayerName1((char*) wBuffer);
        delete wBuffer;

        // Player2Name
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wGameInfos->setPlayerName2((char*) wBuffer);
        delete wBuffer;

        // MapName
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wGameInfos->setMapName((char*) wBuffer);
        delete wBuffer;

        // GameName
        wArraySize  = pPacketReader.readInteger();
        wBuffer     = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wGameInfos->setGameName((char*) wBuffer);
        delete wBuffer;

        //P1 score
        wGameInfos->setPlayer1Score(pPacketReader.readInteger());

        // P2 Score
        wGameInfos->setPlayer2Score(pPacketReader.readInteger());

        // Time
        wGameInfos->setTime(pPacketReader.read64bInteger());

        // GameStatus
        wGameInfos->setGameStatus((GameStatus) pPacketReader.readInteger());

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }



}

void PacketHandlerGameStatus::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameStatus* wPaquet   = (PaquetGameStatus*) pPaquet;

    PartieServeurs* wGameInfos  = wPaquet->getGameInfos();

    pPacketBuilder 
        << wGameInfos->getServerId()
        << wGameInfos->getUniqueGameId()
        << wGameInfos->getPlayer1Name()
        << wGameInfos->getPlayer2Name()
        << wGameInfos->getMapName()
        << wGameInfos->getGameName()
        << wGameInfos->getPlayer1Score()
        << wGameInfos->getPlayer2Score()
        << wGameInfos->getTime()     // time_t est represente par un int64
        << wGameInfos->getGameStatus();

}
