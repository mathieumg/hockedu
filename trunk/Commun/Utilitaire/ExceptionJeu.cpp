//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionJeu.cpp
/// @author Michael Ferris
/// @date 2013-03-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "ExceptionJeu.h"
#include "stdarg.h"
#ifndef __APPLE__

char* hackBuffer = new char[256];
////////////////////////////////////////////////////////////////////////
///
/// @fn  ExceptionJeu::ExceptionJeu( const char* pMessage, ... )
///
/// /*Description*/
///
/// @param[in] const char * pMessage
/// @param[in] ...
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ExceptionJeu::ExceptionJeu( const char* pMessage, ... ):
    // hack pour avoir acces a un attribut privé, de cette facon, 
    // la classe de base ne liberera pas la memoire de la chaine, so its up to us
    mErrorMessage(NULL),Super(pMessage?hackBuffer:NULL,0)
{
    if(pMessage)
    {
        GET_VARARGS( hackBuffer, 256, 255, pMessage, pMessage );
        // keep a pointer on it to free the memory in the destructor
        mErrorMessage = hackBuffer;

        // allocate a new array for next time
        hackBuffer = new char[256];
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  ExceptionJeu::~ExceptionJeu()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ExceptionJeu::~ExceptionJeu() throw()
{
    if(mErrorMessage)
    {
        free(const_cast<char *>(mErrorMessage));
    }
}

#else
ExceptionJeu::ExceptionJeu( const char* pMessage, ... ):
Super()
{}
ExceptionJeu::~ExceptionJeu() throw(){}

#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////