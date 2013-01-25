//////////////////////////////////////////////////////////////////////////////
/// @file PaquetConnAutomatique.h
/// @author Mathieu Parent
/// @date 2013-01-13
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"

class UsinePaquetConnAutomatique;

enum BroadcastType {CONNECTION_REQUEST, CONNECTION_RESPONSE};

///////////////////////////////////////////////////////////////////////////
/// @class PaquetConnAutomatique
/// @brief Classe qui represente un paquet pour la connection automatique
///
/// @author Mathieu Parent
/// @date 2013-01-13
///////////////////////////////////////////////////////////////////////////
class PaquetConnAutomatique : public Paquet {
private:
	friend UsinePaquetConnAutomatique;
	
	std::string mRequestIP;
	std::string mResponseIP;
	std::string mRequestHostname;
	std::string mResponseHostname;
	ConnectionType mNewConnectionType;
	BroadcastType mBroadcastType;
	int mPortNumber;
	
protected:
	PaquetConnAutomatique(const std::string& operation);

public:
	
	ConnectionType getNewConnectionType() const { return mNewConnectionType; }
	void setNewConnectionType(ConnectionType val) { mNewConnectionType = val; }

	BroadcastType getBroadcastType() const { return mBroadcastType; }
	void setBroadcastType(BroadcastType val) { mBroadcastType = val; }

	std::string getResponseIP() const { return mResponseIP; }
	void setResponseIP(std::string val) { mResponseIP = val; }

	std::string getRequestIP() const { return mRequestIP; }
	void setRequestIP(std::string val) { mRequestIP = val; }

	std::string getResponseHostname() const { return mResponseHostname; }
	void setResponseHostname(std::string val) { mResponseHostname = val; }

	std::string getRequestHostname() const { return mRequestHostname; }
	void setRequestHostname(std::string val) { mRequestHostname = val; }

	int getPortNumber() const { return mPortNumber; }
	void setPortNumber(int val) { mPortNumber = val; }

	~PaquetConnAutomatique();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

