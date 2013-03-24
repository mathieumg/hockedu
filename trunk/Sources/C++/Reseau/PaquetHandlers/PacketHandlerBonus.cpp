

#include "../Paquets/PaquetBonus.h"
#include "PacketHandlerBonus.h"



std::hash_map<PaquetBonusType, PaquetBonusRunnable> PacketHandlerBonus::mRunnableList;




////////// Declaration des differents delegates pour la reception des paquets de type bonus

PaquetBonusInfos* BonusMailletMuretReader(PacketReader& pPacketReader)
{
    PaquetBonusInfosMailletMurets* wInfos = new PaquetBonusInfosMailletMurets;
    wInfos->onLeftPlayer = pPacketReader.readBool();
    return wInfos;
}

PaquetBonusInfos* BonusGoalerReader(PacketReader& pPacketReader)
{
    PaquetBonusInfosGoaler* wInfos = new PaquetBonusInfosGoaler;
    wInfos->onLeftPlayer = pPacketReader.readBool();
    return wInfos;
}



////////// Declaration des differents delegates pour l'envoie des paquets de type bonus

int BonusMailletMuretBuilder(PaquetBonusInfos* pPaquetInfos, PacketBuilder& pPacketBuilder)
{
    PaquetBonusInfosMailletMurets* wInfos = (PaquetBonusInfosMailletMurets*) pPaquetInfos;
    pPacketBuilder << wInfos->onLeftPlayer;
    return 0;
}

int BonusGoalerBuilder(PaquetBonusInfos* pPaquetInfos, PacketBuilder& pPacketBuilder)
{
    PaquetBonusInfosGoaler* wInfos = (PaquetBonusInfosGoaler*) pPaquetInfos;
    pPacketBuilder << wInfos->onLeftPlayer;
    return 0;
}







PacketHandlerBonus::PacketHandlerBonus()
{
    // Ici on ajoute les differents elements pour les differents types de bonus

    /////// Size
    mPaquetSizes[BONUS_MAILLET_MURETS]  = getSizeForBool();
    mPaquetSizes[BONUS_GOALER]          = getSizeForBool();

    /////// Reader
    mPaquetReaders[BONUS_MAILLET_MURETS]    = BonusMailletMuretReader;
    mPaquetReaders[BONUS_GOALER]            = BonusGoalerReader;


    /////// Builder
    mPaquetBuilders[BONUS_MAILLET_MURETS]   = BonusMailletMuretBuilder;
    mPaquetBuilders[BONUS_GOALER]           = BonusGoalerBuilder;

}


void PacketHandlerBonus::handlePacketReceptionSpecific(PacketReader& pPacketReader, PaquetRunnableFunc pRunnable/* = NULL*/)
{
    if(pRunnable)
    {
        PaquetBonus* wPaquet = (PaquetBonus*) GestionnaireReseau::obtenirInstance()->creerPaquet(BONUS);
        
        // Read basic infos
        wPaquet->setBonusType((PaquetBonusType) pPacketReader.readInteger());
        wPaquet->setGameId(pPacketReader.readInteger());
        Vecteur2 wPos;
        wPos[VX] = pPacketReader.readFloat();
        wPos[VY] = pPacketReader.readFloat();

        // Read specific infos
        wPaquet->setPaquetInfos(mPaquetReaders[wPaquet->getBonusType()](pPacketReader));


        wPaquet->setRunnable(pRunnable);
        wPaquet->run();
    }



}

void PacketHandlerBonus::handlePacketPreparationSpecific(Paquet* pPaquet, PacketBuilder& pPacketBuilder)
{
    PaquetBonus* wPaquet = (PaquetBonus*) pPaquet;

    pPacketBuilder  << wPaquet->getGameId()
                    << wPaquet->getBonusType()
                    << wPaquet->getBonusPosition()[VX]
                    << wPaquet->getBonusPosition()[VY];

    // Handle specific preparation
    // checkf(wPaquet->getPaquetInfos(), "PacketInfo null a l'envoie du paquet bonus");
    // TODO: Quand le checkf va etre fix pour le serveur maitre, decommenter la ligne, throw en attendant
    if(!wPaquet->getPaquetInfos())
        throw ExceptionReseau("Paquet Info invalide a l'envoie");
    if(wPaquet->getPaquetInfos())
    {
        mPaquetBuilders[wPaquet->getBonusType()](wPaquet->getPaquetInfos(), pPacketBuilder);
    }

}



int PacketHandlerBonus::getPacketSizeSpecific( Paquet* pPaquet ) const
{
    PaquetBonus* wPaquet = (PaquetBonus*) pPaquet;

    return      getSizeForInt() *2
        + getSizeForFloat() *2
        + getPacketBonusSize(wPaquet);
}



int PacketHandlerBonus::getPacketBonusSize(PaquetBonus* pPaquet) const
{
    auto wIt = mPaquetSizes.find(pPaquet->getBonusType());
    if(wIt!= mPaquetSizes.end())
    {
        return wIt->second;
    }
    else
    {
        return 0;
    }
}




