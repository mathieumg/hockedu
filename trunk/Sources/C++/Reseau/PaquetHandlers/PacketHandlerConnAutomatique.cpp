#include "PacketHandlerConnAutomatique.h"
#include <iostream>
#include <sstream>
#include "..\Paquets\PaquetConnAutomatique.h"
#include <sstream>
#include "..\GestionnaireReseau.h"

void PacketHandlerConnAutomatique::handlePacketReceptionSpecific( PacketReader& pPacketReader )
{
	// On vient de recevoir un broadcast paquet
	// Si request, on repond avec notre adresse IP

	PaquetConnAutomatique* wPaquet = (PaquetConnAutomatique*) NULL;//paquet;

	if(wPaquet->getBroadcastType() == CONNECTION_REQUEST)
	{
		// Si on a recu une requete, on repond puis on essaie de se connecter a cette adresse

		PaquetConnAutomatique* wPaquetReponse = (PaquetConnAutomatique*) GestionnaireReseau::obtenirInstance()->creerPaquet("connAutomatique");
		wPaquetReponse->setRequestIP(wPaquet->getRequestIP());
		wPaquetReponse->setResponseIP(GestionnaireReseau::obtenirInstance()->getAdresseIPLocaleAssociee(wPaquet->getRequestIP()));
		wPaquetReponse->setNewConnectionType(wPaquet->getNewConnectionType());
		wPaquetReponse->setBroadcastType(CONNECTION_RESPONSE);

		GestionnaireReseau::obtenirInstance()->envoyerPaquetBroadcast(wPaquetReponse);

		GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection(wPaquet->getRequestHostname(), wPaquet->getRequestIP(), wPaquet->getNewConnectionType());
		

	}
	else if (wPaquet->getBroadcastType() == CONNECTION_RESPONSE)
	{
		// Si on a recu une reponse, on essaie de se connecter a cette adresse si l'adresse de requete dans le paquet est la notre
		std::string wAdresseRequest = wPaquet->getRequestIP();
		if(wAdresseRequest == GestionnaireReseau::obtenirInstance()->getAdresseIPLocaleAssociee(wAdresseRequest)) {
			// Reponse pour nous, on essaye de se connecter

			GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection(wPaquet->getResponseHostname(), wPaquet->getResponseIP(), wPaquet->getNewConnectionType());
			

		}
		// Sinon rien


	}

	// Connection a l'adresse
}


void PacketHandlerConnAutomatique::handlePacketPreparationSpecific( Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
	// Stuff a Charles
}

