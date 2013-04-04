//////////////////////////////////////////////////////////////////////////////
/// @file PaquetGameStatus.h
/// @author Mathieu Parent
/// @date 2013-02-17
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"
#include "../ObjetsGlobaux/PartieServeurs.h"

class UsinePaquetGameStatus;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetGameStatus
/// @brief Classe qui represente un paquet pour faire les echanges d'informations de partie
///
/// @author Mathieu Parent
/// @date 2013-02-17
///////////////////////////////////////////////////////////////////////////
class PaquetGameStatus : public Paquet {
private:
    friend class UsinePaquetGameStatus;

    PartieServeurs* mGame;

protected:
	PaquetGameStatus();

public:
    virtual PacketTypes getOperation() const { return GAME_STATUS; }

    inline PartieServeurs* getGameInfos() {return mGame;}
    inline void setGameInfos(PartieServeurs* pGame)
    {
        if(mGame)
        {
            delete mGame;
        }
        mGame = pGame;
    }


	~PaquetGameStatus();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

