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
enum PaquetBonusAction {BONUS_ACTION_SPAN, BONUS_ACTION_EXECUTE, BONUS_ACTION_END};


//class PaquetBonusInfos {};


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
    PaquetBonusAction mBonusAction;
    
    int mGameId;
    Vecteur2 mBonusPosition;

protected:
	PaquetBonus();

public:
	virtual PacketTypes getOperation() const { return BONUS; }

    inline const Vecteur2& getBonusPosition() const { return mBonusPosition; }
    inline void setBonusPosition(const Vecteur2& val) { mBonusPosition = val; }

    PaquetBonusAction getBonusAction() const { return mBonusAction; }
    void setBonusAction(PaquetBonusAction val) { mBonusAction = val; }

    inline PaquetBonusType getBonusType() const { return mBonusType; }
    inline void setBonusType(PaquetBonusType val) { mBonusType = val; }

    inline int getGameId() const { return mGameId; }
    inline void setGameId(int val) { mGameId = val; }

    ~PaquetBonus();

};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

