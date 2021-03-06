//////////////////////////////////////////////////////////////////////////////
/// @file Paquet.h
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <time.h>
#include "UsinePaquet.h"
#include "NetworkEnums.h"
#include "PacketsDefinitions.h"
class UsinePaquet;


///////////////////////////////////////////////////////////////////////////
/// @class Paquet
/// @brief Classe qui represente un paquet (abstrait)
///
/// @author Mathieu Parent
/// @date 2012-12-01
///////////////////////////////////////////////////////////////////////////
class Paquet
{
public:
	friend class UsinePaquet;

	virtual ~Paquet();

    static const unsigned int sequenceIdentification = 0xBD7DFF6F;
    static int CompteurNumeroPaquet;

	virtual PacketTypes getOperation() const
    {
        if(mData)
        {
            return (PacketTypes)mData->GetType();
        }
        return (PacketTypes)PT_NONE;
    }

    inline int getNbAssociatedQueries() const { return mNbAssociatedQueries; }
    // Attention, si la mauvaise valeur est mise, cela peut causer des GROS problemes
    inline void setNbAssociatedQueries(int pNbAssociatedQueries) { checkf(pNbAssociatedQueries != 0);{ mNbAssociatedQueries = pNbAssociatedQueries;} }
    // Fonction qui decremente le compteur de queries associees a ce Paquet, Si compteur = 0, on appelle le destructeur
    void removeAssociatedQuery();

    // Execution du Paquet avec un runnable
    inline int run() { return mRunnableFunction(this); }

    inline void setRunnable(PaquetRunnableFunc pRunnable) { mRunnableFunction = pRunnable; }

    inline int getNumeroPaquet() const {return mNumeroPaquet;}

    inline int getLatency() const { return mLatency; }
    inline void setLatency(int val) { mLatency = val; mLastTick = clock(); }

    inline clock_t getLastTick() const { return mLastTick; }
    inline void setLastTick(clock_t val) { mLastTick = val; }

    inline void forceSendBrokenSocket() {mForceSendEvenWhenSocketNotConnected = true;}

    inline bool isForcedToSendOnBrokenSocket() {return mForceSendEvenWhenSocketNotConnected;}
    inline PacketDataBase* getData() const { return mData; }
    inline void setData(PacketDataBase* val) { mData = val; }

	Paquet();
private:
	int mNbAssociatedQueries; // Nb de fois que le paquet doit etre envoyer (different de 1 si on fait un envoie de masse)
    int mNumeroPaquet;

    int mLatency;
    clock_t mLastTick;
    
    bool mForceSendEvenWhenSocketNotConnected;
    PacketDataBase* mData;
    PaquetRunnableFunc mRunnableFunction;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

