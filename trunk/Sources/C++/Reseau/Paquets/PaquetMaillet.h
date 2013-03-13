//////////////////////////////////////////////////////////////////////////////
/// @file PaquetMaillet.h
/// @author Mathieu Parent
/// @date 2013-03-04
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"
#include "Vecteur.h"

class UsinePaquetMaillet;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetMaillet
/// @brief Classe qui represente un paquet pour les infos du maillet
///
/// @author Mathieu Parent
/// @date 2013-03-04
///////////////////////////////////////////////////////////////////////////
class PaquetMaillet : public Paquet {
private:
    friend class UsinePaquetMaillet;


    // Position du maillet
    Vecteur3 mPosition;
    
    // I de la game dans lequel le maillet est
    int mGameId;
    
    // Dit s'il s'agit du joueur gauche ou du joueur droit
    bool mEstAGauche;
    



protected:
	PaquetMaillet();

public:
	virtual PacketTypes getOperation() const { return MAILLET; }

    inline const Vecteur3& getPosition() const { return mPosition; }
    inline void setPosition(const Vecteur3& val) { mPosition = val; }

    inline int getGameId() const { return mGameId; }
    inline void setGameId(int val) { mGameId = val; }

    inline bool getEstAGauche() const { return mEstAGauche; }
    inline void setEstAGauche(bool val) { mEstAGauche = val; }


	~PaquetMaillet();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

