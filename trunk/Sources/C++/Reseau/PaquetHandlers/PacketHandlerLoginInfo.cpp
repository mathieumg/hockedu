
#include "PacketHandler.h"
#include "PaquetLoginInfo.h"

void PacketHandlerLoginInfo::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{

    if(pRunnable)
    {
        PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) UsinePaquet::creerPaquet(LOGIN_INFO);

        int wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setUsername((char*) wBuffer);
        delete wBuffer;


        wArraySize = pPacketReader.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setPassword((char*) wBuffer);
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }

}

void PacketHandlerLoginInfo::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;


    pPacketBuilder << wPaquet->getUsername()
        << wPaquet->getPassword();

}



int PacketHandlerLoginInfo::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetLoginInfo* wPaquet = (PaquetLoginInfo*) pPaquet;


    return  getSizeForString(wPaquet->getUsername())
        +  getSizeForString(wPaquet->getPassword());

}
