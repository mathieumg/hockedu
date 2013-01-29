//////////////////////////////////////////////////////////////////////////////
/// @file PaquetChatUserStatus.h
/// @author Mathieu Parent
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Reseau\Paquets\PaquetBase.h"

class UsinePaquetChatUserStatus;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetChatUserStatus
/// @brief Classe qui represente une modification du status de connection d'un user connecte au Chat
///
/// @author Mathieu Parent
/// @date 2013-01-28
///////////////////////////////////////////////////////////////////////////
class PaquetChatUserStatus : public PaquetBase {
private:
    friend UsinePaquetChatUserStatus;

    std::string mUserName;
	ConnectionState mConnectionState;

protected:
	PaquetChatUserStatus();

public:

	std::string getUserName() const { return mUserName; }
	void setUserName(const std::string& val) { mUserName = val; }

	ConnectionState getConnectionState() const { return mConnectionState; }
	void setConnectionState(ConnectionState val) { mConnectionState = val; }


	~PaquetChatUserStatus();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

