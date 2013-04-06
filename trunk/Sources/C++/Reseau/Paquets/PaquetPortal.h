//////////////////////////////////////////////////////////////////////////////
/// @file PaquetPortal.h
/// @author Vincent Lemire
/// @date 2013-04-05
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"
#include "Vecteur.h"

class UsinePaquetPortal;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetPortal
/// @brief Classe qui represente un paquet pour les infos des portails
///
/// @author Mathieu Parent
/// @date 2013-03-18
///////////////////////////////////////////////////////////////////////////
class PaquetPortal : public Paquet {
private:
    friend class UsinePaquetPortal;

    int mGameId;

    Vecteur3 mPosition;


protected:
    PaquetPortal();

public:
    virtual PacketTypes getOperation() const { return PORTAL; }


    inline int getGameId() const { return mGameId; }
    inline void setGameId(int val) { mGameId = val; }

    inline Vecteur3 getPosition() const { return mPosition; }
    inline void setPosition(const Vecteur3& val) { mPosition = val; }

    ~PaquetPortal();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

