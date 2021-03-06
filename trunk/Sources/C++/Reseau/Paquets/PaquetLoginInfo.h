//////////////////////////////////////////////////////////////////////////////
/// @file PaquetLoginInfo.h
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Paquet.h"

class UsinePaquetLoginInfo;

///////////////////////////////////////////////////////////////////////////
/// @class PaquetLoginInfo
/// @brief Classe qui represente un paquet pour l'authentification des utilisateurs
///
/// @author Mathieu Parent
/// @date 2013-02-11
///////////////////////////////////////////////////////////////////////////
class PaquetLoginInfo : public Paquet {
private:
    friend class UsinePaquetLoginInfo;

    std::string mUsername; // Nom d'utilisateur
    std::string mPassword; // Mot de passe


protected:
	PaquetLoginInfo();

public:
    virtual PacketTypes getOperation() const { return LOGIN_INFO; }

    inline std::string getUsername() const { return mUsername; }
    inline void setUsername(const std::string& val) { mUsername = val; }

    inline std::string getPassword() const { return mPassword; }
	inline void setPassword(const std::string& val) { mPassword = val; }

	~PaquetLoginInfo();
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

