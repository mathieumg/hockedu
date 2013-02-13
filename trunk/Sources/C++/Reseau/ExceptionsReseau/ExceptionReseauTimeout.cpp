//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionReseauTimeout.cpp
/// @author Mathieu Parent
/// @date 2013-01-21
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "ExceptionReseauTimeout.h"


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




ExceptionReseauTimeout::ExceptionReseauTimeout( const std::string& pMessage )
    :ExceptionReseau(pMessage)
{

}

void ExceptionReseauTimeout::throwMe()
{
    throw *this;
}