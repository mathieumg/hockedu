//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionReseau.h
/// @author Mathieu Parent
/// @date 2013-01-05
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <exception>
#include "Reseau/Socket.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////
/// @class ExceptionReseau
/// @brief Classe qui represente une exception reseau de base
///
/// @author Mathieu Parent
/// @date 2013-01-05
///////////////////////////////////////////////////////////////////////////
struct ErrorEntry;
class ExceptionReseau : public std::runtime_error
{
public:
	ExceptionReseau(const std::string& pMessage);
	ExceptionReseau(const std::string& pMessage, const int& pErrorId);
    const char* what();
	inline int getErrorId() const {return mErrorId;}
    virtual void throwMe();
	
    static std::string getLastErrorMessage(const char* pcMessagePrefix, int nErrorID/* = 0*/);
private:
    
    
    static ErrorEntry mSocketErrorsList[];
    const static int mErrorMessagesAmount;
    const int mErrorId;
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

