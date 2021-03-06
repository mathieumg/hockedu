//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionReseauBD.h
/// @author Mathieu Parent
/// @date 2013-02-19
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../../Reseau/ExceptionsReseau/ExceptionReseau.h"


///////////////////////////////////////////////////////////////////////////
/// @class ExceptionReseauBD
/// @brief Classe qui represente une exception reseau de base de donnee
///
/// @author Mathieu Parent
/// @date 2013-02-19
///////////////////////////////////////////////////////////////////////////
class ExceptionReseauBD : public ExceptionReseau
{
public:
	ExceptionReseauBD(const std::string& pMessage);

    virtual void throwMe();


private:

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

