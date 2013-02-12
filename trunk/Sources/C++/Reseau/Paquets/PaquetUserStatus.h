//////////////////////////////////////////////////////////////////////////////
/// @file PaquetUserStatus.h
/// @author Mathieu Parent
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Reseau\Paquets\PaquetBase.h"

class UsinePaquetUserStatus;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetUserStatus
/// @brief Classe qui represente une modification du status de connection d'un user connecte au Chat
///
/// @author Mathieu Parent
/// @date 2013-01-28
///////////////////////////////////////////////////////////////////////////
class PaquetUserStatus : public PaquetBase {
private:
    friend UsinePaquetUserStatus;

    std::string mUserName;
	ConnectionState mConnectionState;

protected:
	PaquetUserStatus();

public:

	std::string getUserName() const { return mUserName; }
	void setUserName(const std::string& val) { mUserName = val; }

	ConnectionState getConnectionState() const { return mConnectionState; }
	void setConnectionState(ConnectionState val) { mConnectionState = val; }


	~PaquetUserStatus();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

