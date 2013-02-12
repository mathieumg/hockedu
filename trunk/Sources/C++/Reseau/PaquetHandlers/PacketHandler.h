

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
    char type[GestionnaireReseau::longueurMaxOperationReseau];
    int numeroPaquet;
    size_t taillePaquet;
};

class PacketHandler {
    
public:
    virtual HeaderPaquet handlePacketHeaderReception(PacketReader& pPacketReader); // Méthode pour lire le header d'un paquet
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL) = 0; // Méthode pour lire les données d'un paquet pour un paquet de base
    void handlePacketPreparation(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Methode pour la construction du paquet pour l'envoie sur le reseau
    virtual int getPacketSize(Paquet* pPaquet) const;
protected:
    virtual HeaderPaquet& handlePacketHeaderReceptionBase(PacketReader& pPacketReader, HeaderPaquet& pHeaderPaquet) = 0; // Méthode pour lire le header d'un paquet
    virtual void handlePacketPreparationBase(Paquet* pPaquet, PacketBuilder& pPacketBuilder, int& pPaquetSize) = 0;  // Méthode pour construire le paquet à envoyer pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder) = 0; // Méthode pour construire le paquet à envoyer
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

