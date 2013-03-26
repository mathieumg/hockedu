

#pragma once
class Paquet;
#include "../PacketBuilder.h"
#include "../PacketReader.h"
#include "../GestionnaireReseau.h"
#include <string>

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
#define getPacketHeaderSize() (Paquet::sequenceIdentificationLength + sizeof(PacketTypes) + sizeof(int) + sizeof(int))

    static HeaderPaquet handlePacketHeaderReception(PacketReader& pPacketReader); // Méthode pour lire le header d'un paquet
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL) = 0; // Méthode pour lire les données d'un paquet pour un paquet de base
    void handlePacketPreparation(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Methode pour la construction du paquet pour l'envoie sur le reseau
    int getPacketSize(Paquet* pPaquet) const;
protected:
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder) = 0; // Méthode pour construire le paquet à envoyer
    virtual int getPacketSizeSpecific(Paquet* pPaquet) const = 0;
};

#define HANDLER_DECLARATION(name)                                                                                \
class PacketHandler##name :public PacketHandler {                                                                \
protected:                                                                                                       \
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL);\
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder);                \
    virtual int getPacketSizeSpecific( Paquet* pPaquet ) const;                                                  \
};                                                                                                               \

HANDLER_DECLARATION(UserStatus);
HANDLER_DECLARATION(Test);
HANDLER_DECLARATION(Rondelle);
HANDLER_DECLARATION(Maillet);
HANDLER_DECLARATION(LoginInfo);
HANDLER_DECLARATION(GameStatus);
HANDLER_DECLARATION(GameCreation);
HANDLER_DECLARATION(GameConnection);
HANDLER_DECLARATION(GameRegistration);
HANDLER_DECLARATION(GameEvent);
HANDLER_DECLARATION(Event);
HANDLER_DECLARATION(ConnAutomatique);
HANDLER_DECLARATION(AuthentificationServeurJeu);
HANDLER_DECLARATION(ChatMessage);

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

