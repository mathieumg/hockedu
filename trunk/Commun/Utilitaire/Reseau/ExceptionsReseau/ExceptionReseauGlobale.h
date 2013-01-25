//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionReseauGlobale.h
/// @author Mathieu Parent
/// @date 2013-01-21
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ExceptionReseau.h"


///////////////////////////////////////////////////////////////////////////
/// @class ExceptionReseauGlobale
/// @brief Classe qui represente une exception reseau globale (Le systeme ne peut pas fonctionner avec une de ces exceptions)
///
/// @author Mathieu Parent
/// @date 2013-01-21
///////////////////////////////////////////////////////////////////////////
class ExceptionReseauGlobale : public ExceptionReseau
{
public:
	ExceptionReseauGlobale(const std::string& pMessage);

    virtual void throwMe();


private:
    
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

