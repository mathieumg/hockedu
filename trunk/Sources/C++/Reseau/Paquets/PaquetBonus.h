//////////////////////////////////////////////////////////////////////////////
/// @file PaquetBonus.h
/// @author Mathieu Parent
/// @date 2013-03-24
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"
#include "Vecteur.h"

class UsinePaquetBonus;

// Differents types de paquets
enum PaquetBonusType {BONUS_MAILLET_MURETS, BONUS_GOALER, BONUS_NB};


class PaquetBonusInfos {};


///////////////////////////////////////////////////////////////////////////
/// @class PaquetBonus
/// @brief Classe qui represente un paquet qui contient les informations necessaires aux bonus
///
/// @author Mathieu Parent
/// @date 2013-03-24
///////////////////////////////////////////////////////////////////////////
class PaquetBonus : public Paquet {
private:
    friend class UsinePaquetBonus;

    
    PaquetBonusType mBonusType;
    int mGameId;
    Vecteur2 mBonusPosition;



    PaquetBonusInfos* mInfos;


protected:
	PaquetBonus();

public:
	virtual PacketTypes getOperation() const { return BONUS; }

    void setPaquetInfos(PaquetBonusInfos* pInfos) {if(mInfos) {delete mInfos;} mInfos = pInfos;}
    PaquetBonusInfos* getPaquetInfos() {return mInfos;}

    inline const Vecteur2& getBonusPosition() const { return mBonusPosition; }
    inline void setBonusPosition(const Vecteur2& val) { mBonusPosition = val; }

    inline PaquetBonusType getBonusType() const { return mBonusType; }
    inline void setBonusType(PaquetBonusType val) { mBonusType = val; }

    inline int getGameId() const { return mGameId; }
    inline void setGameId(int val) { mGameId = val; }

    ~PaquetBonus();

};


////////// Enumeration des infos des differents types de paquets

// Utilisation comme une struct
class PaquetBonusInfosGoaler : public PaquetBonusInfos
{
public:
    bool onLeftPlayer;
};


class PaquetBonusInfosMailletMurets : public PaquetBonusInfos
{
public:
    bool onLeftPlayer;
};







///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

