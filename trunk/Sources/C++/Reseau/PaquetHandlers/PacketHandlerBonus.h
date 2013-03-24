
#pragma once
#include "PacketHandler.h"
#include "../Paquets/PaquetBonus.h"
#include <hash_map>

typedef int (*PaquetBonusBuilder) (PaquetBonusInfos*, PacketBuilder&);
typedef PaquetBonusInfos* (*PaquetBonusReader) (PacketReader&);
typedef int (*PaquetBonusRunnable) (PaquetBonus*);




class PacketHandlerBonus :public PacketHandler {
public:
    PacketHandlerBonus();

    // Le controlleur s'occupe de l'initialisation
    static std::hash_map<PaquetBonusType, PaquetBonusRunnable> mRunnableList;

protected:
    virtual void handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable = NULL); // Méthode pour lire les données d'un paquet pour un paquet de base
    virtual void handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder); // Méthode pour construire le paquet à envoyer
    virtual int getPacketSizeSpecific( Paquet* pPaquet ) const;


    // Nouvelles methodes
    int getPacketBonusSize( PaquetBonus* pPaquet ) const;

    std::hash_map<PaquetBonusType, int>                 mPaquetSizes;
    std::hash_map<PaquetBonusType, PaquetBonusBuilder>  mPaquetBuilders;
    std::hash_map<PaquetBonusType, PaquetBonusReader>   mPaquetReaders;


};