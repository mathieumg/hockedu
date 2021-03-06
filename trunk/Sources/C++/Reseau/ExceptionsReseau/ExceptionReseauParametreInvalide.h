//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionReseauParametreInvalide.h
/// @author Mathieu Parent
/// @date 2013-01-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ExceptionReseau.h"


///////////////////////////////////////////////////////////////////////////
/// @class ExceptionReseauParametreInvalide
/// @brief Classe qui represente une exception reseau de parametre invalide passe a une fonction de Winsock
///
/// @author Mathieu Parent
/// @date 2013-01-21
///////////////////////////////////////////////////////////////////////////
class ExceptionReseauParametreInvalide : public ExceptionReseau
{
public:
	ExceptionReseauParametreInvalide(const std::string& pMessage);

    virtual void throwMe();


private:
    
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

