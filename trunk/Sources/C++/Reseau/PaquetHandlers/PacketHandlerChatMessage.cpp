
#include <iostream>
#include <sstream>
#include "PacketHandler.h"
#include "../Paquets/PaquetChatMessage.h"
#include "../RelayeurMessage.h"
#include <time.h>
#include <sstream>
#include <iomanip>
#ifndef _SERVER
#include "../GestionnaireReseauClientLourd.h"
#endif


void PacketHandlerChatMessage::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet(CHAT_MESSAGE);
        int wArraySize = pPacketReader.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setMessage((char*) wBuffer);
        delete wBuffer;
        wPaquet->setTimestamp( pPacketReader.read64bInteger());
        wPaquet->setIsTargetGroup(pPacketReader.readBool());

        wArraySize = pPacketReader.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setGroupName((char*) wBuffer);
        delete wBuffer;

        wArraySize = pPacketReader.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pPacketReader.readString(wBuffer, wArraySize);
        wPaquet->setOrigin((char*) wBuffer);
        delete wBuffer;

        wPaquet->setRunnable(pRunnable);
        wPaquet->run();

    }


// #ifdef _SERVER
//     //if(GestionnaireReseau::getNetworkMode() == SERVER)
//     {
//         std::ostringstream wTimeOutput;
//         time_t wT = time(0);
//         struct tm wTime;
//         if(_localtime64_s(&wTime, &wT))
//         {
//             std::cout << "[00:00:00]";
//         }
//         else
//         {
//             wTimeOutput << std::setfill('0') << "["
//                 << std::setw(2) << wTime.tm_hour
//                 << std::setw(1) << ":"
//                 << std::setw(2) << wTime.tm_min
//                 << std::setw(1) << ":"
//                 << std::setw(2) << wTime.tm_sec
//                 << std::setw(1) << "]";
//         }
//
//
//         wTimeOutput << "[" << wPaquet->getOrigin() << "]: " << wPaquet->getMessage() << std::endl;
//         std::cout << wTimeOutput.str();
//
//         // On veut relayer le message a une personne en particulier ou a tout le monde
//         // On ne call donc pas delete dessus tout suite
//
//         if(wPaquet->IsTargetGroup())
//         {
//             // On envoie a tout le monde
//             //std::set<std::string> wListeIgnore;
//             //wListeIgnore.insert(wPaquet->getOrigin()); // Decommenter pour ne pas recevoir ses propres messages
//             RelayeurMessage::obtenirInstance()->relayerPaquetGlobalement(wPaquet, NULL, TCP);
//         }
//         else
//         {
//             // On l'envoie a la personne dans groupName seulement
//             RelayeurMessage::obtenirInstance()->relayerPaquet(wPaquet->getGroupName(), wPaquet, TCP);
//         }
//
//     }
// #else
//     //else if(GestionnaireReseau::getNetworkMode() == CLIENT)
//     {
//
//
//         GestionnaireReseau::obtenirInstance()->transmitEvent(CHAT_MESSAGE_RECEIVED,wPaquet->getOrigin(),wPaquet->getMessage());
//
//         wPaquet->removeAssociatedQuery();
//     }
// #endif

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



int PacketHandlerChatMessage::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) pPaquet;


    return  getSizeForString(wPaquet->getMessage())
        + getSizeFor64bInteger()
        + getSizeForBool()
        +  getSizeForString(wPaquet->getGroupName())
        +  getSizeForString(wPaquet->getOrigin());

}
