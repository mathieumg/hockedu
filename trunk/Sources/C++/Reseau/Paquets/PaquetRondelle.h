//////////////////////////////////////////////////////////////////////////////
/// @file PaquetRondelle.h
/// @author Mathieu Parent
/// @date 2013-03-18
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"
#include "Vecteur.h"

class UsinePaquetRondelle;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetRondelle
/// @brief Classe qui represente un paquet pour les infos de la rondelle
///
/// @author Mathieu Parent
/// @date 2013-03-18
///////////////////////////////////////////////////////////////////////////
class PaquetRondelle : public Paquet {
private:
    friend class UsinePaquetRondelle;

    int mGameId;
    
    Vecteur3 mPosition;
    Vecteur3 mVelocite;
    float mVitesseRotation;
    

protected:
	PaquetRondelle();

public:
    virtual PacketTypes getOperation() const { return RONDELLE; }


    inline int getGameId() const { return mGameId; }
    inline void setGameId(int val) { mGameId = val; }

    inline float getVitesseRotation() const { return mVitesseRotation; }
    inline void setVitesseRotation(float val) { mVitesseRotation = val; }

    inline Vecteur3 getVelocite() const { return mVelocite; }
    inline void setVelocite(const Vecteur3& val) { mVelocite = val; }

    inline Vecteur3 getPosition() const { return mPosition; }
    inline void setPosition(const Vecteur3& val) { mPosition = val; }



	~PaquetRondelle();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

