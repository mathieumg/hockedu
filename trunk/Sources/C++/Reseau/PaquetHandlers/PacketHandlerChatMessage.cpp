
#include <iostream>
#include <sstream>
#include "PacketHandlerChatMessage.h"
#include "..\Paquets\PaquetChatMessage.h"
#include "..\RelayeurMessage.h"
#include <time.h>
#include <sstream>
#include <iomanip>


void PacketHandlerChatMessage::handlePacketReceptionSpecific(PacketReader& pPacketReader)
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatMessage");
    wPaquet->setMessage((char*) pPacketReader.readString());
    wPaquet->setTimestamp( pPacketReader.read64bInteger());
    wPaquet->setIsTargetGroup(pPacketReader.readBool());
    wPaquet->setGroupName((char*) pPacketReader.readString());
    wPaquet->setOrigin((char*) pPacketReader.readString());


    if(GestionnaireReseau::getNetworkMode() == SERVER)
    {
        // On veut relayer le message a une personne en particulier ou a tout le monde
        // On ne call donc pas delete dessus tout suite

        if(wPaquet->IsTargetGroup())
        {
            // On envoie a tout le monde
            std::set<std::string> wListeIgnore;
            wListeIgnore.insert(wPaquet->getOrigin());
            RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(wPaquet, wListeIgnore, TCP);
        }
        else
        {
            // On l'envoie a la personne dans groupName seulement
            RelayeurMessage::obtenirInstance()->relayerPaquet(wPaquet->getGroupName(), wPaquet, TCP);
        }
    }
    else if(GestionnaireReseau::getNetworkMode() == CLIENT)
    {
        time_t wT = wPaquet->getTimestamp();
        struct tm wTime;
        if(_localtime64_s(&wTime, &wT))
        {
            std::cout << "[00:00:00]";
        }
        else
        {
            std::ostringstream wTimeOutput;
            wTimeOutput << std::setfill('0') << "["
                << std::setw(2) << wTime.tm_hour
                << std::setw(1) << ":"
                << std::setw(2) << wTime.tm_min
                << std::setw(1) << ":"
                << std::setw(2) << wTime.tm_sec
                << std::setw(1) << "]";
            std::cout << wTimeOutput.str();
        }
        

        std::cout  << "[" << wPaquet->getOrigin() << "]: " << wPaquet->getMessage() << std::endl;

        delete wPaquet;
    }

    
}

void PacketHandlerChatMessage::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    if("" == wPaquet->getOrigin())
    {
        throw ExceptionReseau("Le champ origine du PaquetChatMessage est necessaire.");
    }

    pPacketBuilder << wPaquet->getMessage()
        << wPaquet->getTimestamp()
        << wPaquet->IsTargetGroup()
        << wPaquet->getGroupName() 
        <<wPaquet->getOrigin();
    
}



int PacketHandlerChatMessage::getPacketSize( Paquet* pPaquet ) const
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;

    
    return PacketBuilder::getSizeForString(wPaquet->getMessage())
        + PacketBuilder::getSizeFor64bInteger()
        + PacketBuilder::getSizeForBool()
        + PacketBuilder::getSizeForString(wPaquet->getGroupName())
        + PacketBuilder::getSizeForString(wPaquet->getOrigin())
        + PacketHandlerBase::getPacketSize(pPaquet);
    
}