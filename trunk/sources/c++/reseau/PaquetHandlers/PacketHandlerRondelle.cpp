
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
#include "..\Paquets\PaquetRondelle.h"


void PacketHandlerRondelle::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetRondelle* wPaquet = (PaquetRondelle*) GestionnaireReseau::obtenirInstance()->creerPaquet(RONDELLE);


        wPaquet->setGameId(pPacketReader.readInteger());
        float wX = pPacketReader.readFloat();
        float wY = pPacketReader.readFloat();
        wPaquet->setPosition(Vecteur3(wX, wY, 0));

        wX = pPacketReader.readFloat();
        wY = pPacketReader.readFloat();
        wPaquet->setVelocite(Vecteur3(wX, wY, 0));

        wPaquet->setVitesseRotation(pPacketReader.readFloat());

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }



}

void PacketHandlerRondelle::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetRondelle* wPaquet = (PaquetRondelle*) pPaquet;

    
    pPacketBuilder << wPaquet->getGameId()
        << wPaquet->getPosition()[VX]
        << wPaquet->getPosition()[VY]
        << wPaquet->getVelocite()[VX]
        << wPaquet->getVelocite()[VY]
        <<wPaquet->getVitesseRotation();

}



int PacketHandlerRondelle::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetRondelle* wPaquet = (PaquetRondelle*) pPaquet;


    return  getSizeForInt()
            + 5* getSizeForFloat();

}
