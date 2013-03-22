#include "PacketHandler.h"
#include "../Paquets/PaquetGameEvent.h"

void PacketHandlerGameEvent::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetGameEvent* wPaquet = (PaquetGameEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_EVENT);

        // GameId
        wPaquet->setGameId(pPacketReader.readInteger());

        // Player1Name
        int wArraySize      = pPacketReader.readInteger();
        uint8_t* wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setPlayer1Name(std::string((char*) wBuffer));
        delete wBuffer;

        // Player2Name
        wArraySize      = pPacketReader.readInteger();
        wBuffer    = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setPlayer2Name(std::string((char*) wBuffer));
        delete wBuffer;

        // EventCode
        wPaquet->setEvent((GameEventCode) pPacketReader.readInteger());

        // EventOnPlayerLeft
        wPaquet->setEventOnPlayerLeft(pPacketReader.readBool());


        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }



}

void PacketHandlerGameEvent::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetGameEvent* wPaquet   = (PaquetGameEvent*) pPaquet;

    pPacketBuilder  << wPaquet->getGameId()
                    << wPaquet->getPlayer1Name()
                    << wPaquet->getPlayer2Name()
                    << wPaquet->getEvent()
                    << wPaquet->getEventOnPlayerLeft();

}



int PacketHandlerGameEvent::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetGameEvent* wPaquet = (PaquetGameEvent*) pPaquet;

    return getSizeForInt()                            // GameId
        + getSizeForString(wPaquet->getPlayer1Name()) // Player1Name
        + getSizeForString(wPaquet->getPlayer2Name()) // Player2Name
        + getSizeForInt()                             // EventCode
        + getSizeForBool()                            // Event on left
        ;

}