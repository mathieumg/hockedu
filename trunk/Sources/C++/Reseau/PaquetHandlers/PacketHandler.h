

#pragma once
class Paquet;
#include "PacketBuilder.h"
#include "PacketReader.h"
#include "UsinePaquet.h"
#include <string>
#include "PaquetRunnable.h"

///////////////////////////////////////////////////////////////////////////
/// @class PaquetHandler
/// @brief Classe (abstraite) pour la gestion des paquets a la reception (doit savoir quoi faire des paquets recus)
///
/// ** Design pattern "Command"
///
/// @author Mathieu Parent
/// @date 2012-12-01
///////////////////////////////////////////////////////////////////////////

struct HeaderPaquet
{
    PacketTypes type;
    int numeroPaquet;
    int taillePaquet;
};

class PacketHandler {
public:
#define getPacketHeaderSize() (sizeof(unsigned int) + sizeof(int) + sizeof(int) + sizeof(int))

    static HeaderPaquet handlePacketHeaderReception(PacketReader& pPacketReader); // Méthode pour lire le header d'un paquet
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL); // Méthode pour lire les données d'un paquet pour un paquet de base
    void handlePacketPreparation(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Methode pour la construction du paquet pour l'envoie sur le reseau
    static PacketHandler* GetHandler(unsigned int type); 
protected:
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Méthode pour construire le paquet à envoyer
private:
    int packetType;
};

#define HANDLER_DECLARATION(name)                                                                                \
class PacketHandler##name :public PacketHandler {                                                                \
protected:                                                                                                       \
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL);\
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder);                \
};                                                                                                               \

HANDLER_DECLARATION(UserStatus);
HANDLER_DECLARATION(Rondelle);
HANDLER_DECLARATION(Maillet);
HANDLER_DECLARATION(LoginInfo);
HANDLER_DECLARATION(GameStatus);
HANDLER_DECLARATION(GameCreation);
HANDLER_DECLARATION(GameConnection);
HANDLER_DECLARATION(GameEvent);
HANDLER_DECLARATION(Portal);
HANDLER_DECLARATION(Bonus);

extern PacketHandler* PaquetHandlersArray[NB_PACKET_TYPES];

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

