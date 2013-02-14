

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
    size_t taillePaquet;
};

class PacketHandler {
    
public:
    virtual HeaderPaquet handlePacketHeaderReception(PacketReader& pPacketReader); // Méthode pour lire le header d'un paquet
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL) = 0; // Méthode pour lire les données d'un paquet pour un paquet de base
    void handlePacketPreparation(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Methode pour la construction du paquet pour l'envoie sur le reseau
    int getPacketSize(Paquet* pPaquet) const;
protected:
    virtual int getPacketSizeSpecific(Paquet* pPaquet) const = 0 { return 0; };
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder) = 0; // Méthode pour construire le paquet à envoyer
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

