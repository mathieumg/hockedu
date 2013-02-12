

#include "..\Paquets\PaquetUserStatus.h"
#include "..\PacketBuilder.h"
#include "PacketHandlerUserStatus.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "..\PacketReader.h"



void PacketHandlerUserStatus::handlePacketReceptionSpecific(PacketReader& pPacketReader)
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) GestionnaireReseau::obtenirInstance()->creerPaquet(USER_STATUS);
    int wArraySize = pPacketReader.readInteger();
    uint8_t* wBuffer = new uint8_t[wArraySize];
    pPacketReader.readString(wBuffer, wArraySize);
	wPaquet->setUserName((char*) wBuffer);
    //delete wBuffer;

	wPaquet->setConnectionState( (ConnectionState) pPacketReader.readInteger());

	// Seul les Clients devraient recevoir ceci

	time_t wT = time(0);
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
	std::cout << " " << wPaquet->getUserName();
	switch(wPaquet->getConnectionState())
	{
	case CONNECTED:
        GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_CONNECTED,wPaquet->getUserName());
		break;
	case NOT_CONNECTED:
        GestionnaireReseau::obtenirInstance()->transmitEvent(SERVER_USER_DISCONNECTED,wPaquet->getUserName());
		break;
	case CONNECTING:
		std::cout << " is reconnecting" << std::endl;
		break;
	}




	/*

    if(GestionnaireReseau::getNetworkMode() == SERVER)
    {
        // On veut relayer le message a une personne en particulier ou a tout le monde
        // On ne call donc pas delete dessus tout suite

        if(wPaquet->IsTargetGroup())
        {
            // On modifie le paquet pour que le timestamp soit sync pour tout a l'heure du serveur
            wPaquet->setTimestamp(time(0));
            // On envoie a tout le monde
            std::set<std::string> wListeIgnore;
            //wListeIgnore.insert(wPaquet->getOrigin()); // Decommenter pour ne pas recevoir ses propres messages
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
	*/
    
}

void PacketHandlerUserStatus::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;


    pPacketBuilder << wPaquet->getUserName()
        << wPaquet->getConnectionState();
    
}



int PacketHandlerUserStatus::getPacketSize( Paquet* pPaquet ) const
{
    PaquetUserStatus* wPaquet = (PaquetUserStatus*) pPaquet;

    
    return PacketBuilder::getSizeForString(wPaquet->getUserName())
        + PacketBuilder::getSizeForInt()
        + PacketHandlerBase::getPacketSize(pPaquet);
    
}
