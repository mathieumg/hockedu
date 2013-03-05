
#include "PacketHandlerGameStatus.h"
#include "../Paquets/PaquetGameStatus.h"


void PacketHandlerGameStatus::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameStatus* wPaquet = (PaquetGameStatus*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_STATUS);
        PartieServeurs* wGameInfos = wPaquet->getGameInfos();

        // GameId
        wPaquet->setGameId(pPacketReader.readInteger());

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

        //P1 score
        wGameInfos->setPlayer1Score(pPacketReader.readInteger());

        // P2 Score
        wGameInfos->setPlayer2Score(pPacketReader.readInteger());

        // Time
        wGameInfos->setTime(pPacketReader.readInteger());

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }



}

void PacketHandlerGameStatus::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameStatus* wPaquet   = (PaquetGameStatus*) pPaquet;


    PartieServeurs* wGameInfos  = wPaquet->getGameInfos();

    pPacketBuilder << wGameInfos->getUniqueGameId()
        << wGameInfos->getPlayer1Name()
        << wGameInfos->getPlayer2Name()
        << wGameInfos->getPlayer1Score()
        << wGameInfos->getPlayer2Score()
        << wGameInfos->getTime(); // time_t est represente par iun int

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
