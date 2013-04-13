
#include "PaquetUserStatus.h"
#include "PacketHandler.h"



void PacketHandlerUserStatus::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetUserStatus* wPaquet = (PaquetUserStatus*) UsinePaquet::creerPaquet(USER_STATUS);
        int wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setUserName((char*) wBuffer);
        delete[] wBuffer;

        wPaquet->setConnectionState( (ConnectionState) pPacketReader.readInteger());

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }
}

void PacketHandlerUserStatus::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;

    pPacketBuilder << wPaquet->getUserName()
        << wPaquet->getConnectionState();
}



int PacketHandlerUserStatus::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;


    return  getSizeForString(wPaquet->getUserName())
        + getSizeForInt();

}
