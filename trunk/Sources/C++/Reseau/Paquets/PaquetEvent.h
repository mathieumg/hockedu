//////////////////////////////////////////////////////////////////////////////
/// @file PaquetEvent.h
/// @author Mathieu Parent
/// @date 2013-01-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"

class UsinePaquetEvent;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetEvent
/// @brief Classe qui represente un paquet qui contient un message d'erreur
///
/// @author Mathieu Parent
/// @date 2013-01-29
///////////////////////////////////////////////////////////////////////////
class PaquetEvent : public Paquet {
private:
    friend class UsinePaquetEvent;

    std::string mMessage; // Message a envoyer
    int mErrorCode; // Code d'erreur


protected:
	PaquetEvent();

public:
	virtual PacketTypes getOperation() const { return EVENT; }

    inline std::string getMessage() const { return mMessage; }
    inline void setMessage(const std::string& val) { mMessage = val; }

    int getErrorCode() const { return mErrorCode; }
    void setErrorCode(int val) { mErrorCode = val; }

	~PaquetEvent();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

