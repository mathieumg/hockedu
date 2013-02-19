//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionReseau.cpp
/// @author Mathieu Parent
/// @date 2013-01-05
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "ExceptionReseau.h"
#include <strstream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include "../GestionnaireReseau.h"
struct ErrorEntry
{
    int nID;
    const char* pcMessage;

    ErrorEntry(int id, const char* pc = 0) :
        nID(id),
        pcMessage(pc)
    {
    }

    bool operator<(const ErrorEntry& rhs) const
    {
        return nID < rhs.nID;
    }
};
#ifdef WINDOWS
ErrorEntry ExceptionReseau::mSocketErrorsList[] = {
    ErrorEntry(0,                  "No error"),
    ErrorEntry(WSAEINTR,           "Interrupted system call"),
    ErrorEntry(WSAEBADF,           "Bad file number"),
    ErrorEntry(WSAEACCES,          "Permission denied"),
    ErrorEntry(WSAEFAULT,          "Bad address"),
    ErrorEntry(WSAEINVAL,          "Invalid argument"),
    ErrorEntry(WSAEMFILE,          "Too many open sockets"),
    ErrorEntry(WSAEWOULDBLOCK,     "Operation would block"),
    ErrorEntry(WSAEINPROGRESS,     "Operation now in progress"),
    ErrorEntry(WSAEALREADY,        "Operation already in progress"),
    ErrorEntry(WSAENOTSOCK,        "Socket operation on non-socket"),
    ErrorEntry(WSAEDESTADDRREQ,    "Destination address required"),
    ErrorEntry(WSAEMSGSIZE,        "Message too long"),
    ErrorEntry(WSAEPROTOTYPE,      "Protocol wrong type for socket"),
    ErrorEntry(WSAENOPROTOOPT,     "Bad protocol option"),
    ErrorEntry(WSAEPROTONOSUPPORT, "Protocol not supported"),
    ErrorEntry(WSAESOCKTNOSUPPORT, "Socket type not supported"),
    ErrorEntry(WSAEOPNOTSUPP,      "Operation not supported on socket"),
    ErrorEntry(WSAEPFNOSUPPORT,    "Protocol family not supported"),
    ErrorEntry(WSAEAFNOSUPPORT,    "Address family not supported"),
    ErrorEntry(WSAEADDRINUSE,      "Address already in use"),
    ErrorEntry(WSAEADDRNOTAVAIL,   "Can't assign requested address"),
    ErrorEntry(WSAENETDOWN,        "Network is down"),
    ErrorEntry(WSAENETUNREACH,     "Network is unreachable"),
    ErrorEntry(WSAENETRESET,       "Net connection reset"),
    ErrorEntry(WSAECONNABORTED,    "Software caused connection abort"),
    ErrorEntry(WSAECONNRESET,      "Connection reset by peer"),
    ErrorEntry(WSAENOBUFS,         "No buffer space available"),
    ErrorEntry(WSAEISCONN,         "Socket is already connected"),
    ErrorEntry(WSAENOTCONN,        "Socket is not connected"),
    ErrorEntry(WSAESHUTDOWN,       "Can't send after socket shutdown"),
    ErrorEntry(WSAETOOMANYREFS,    "Too many references, can't splice"),
    ErrorEntry(WSAETIMEDOUT,       "Connection timed out"),
    ErrorEntry(WSAECONNREFUSED,    "Connection refused"),
    ErrorEntry(WSAELOOP,           "Too many levels of symbolic links"),
    ErrorEntry(WSAENAMETOOLONG,    "File name too long"),
    ErrorEntry(WSAEHOSTDOWN,       "Host is down"),
    ErrorEntry(WSAEHOSTUNREACH,    "No route to host"),
    ErrorEntry(WSAENOTEMPTY,       "Directory not empty"),
    ErrorEntry(WSAEPROCLIM,        "Too many processes"),
    ErrorEntry(WSAEUSERS,          "Too many users"),
    ErrorEntry(WSAEDQUOT,          "Disc quota exceeded"),
    ErrorEntry(WSAESTALE,          "Stale NFS file handle"),
    ErrorEntry(WSAEREMOTE,         "Too many levels of remote in path"),
    ErrorEntry(WSASYSNOTREADY,     "Network system is unavailable"),
    ErrorEntry(WSAVERNOTSUPPORTED, "Winsock version out of range"),
    ErrorEntry(WSANOTINITIALISED,  "WSAStartup not yet called"),
    ErrorEntry(WSAEDISCON,         "Graceful shutdown in progress"),
    ErrorEntry(WSAHOST_NOT_FOUND,  "Host not found"),
    ErrorEntry(WSANO_DATA,         "No host data of that type was found")
};
const int ExceptionReseau::mErrorMessagesAmount = sizeof(mSocketErrorsList) / sizeof(ErrorEntry);
#elif defined(LINUX)
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn ExceptionReseau::ExceptionReseau( const std::string& message, const int& pErrorId )
///
/// Constructeur par defaut
///
/// @params[in] : message	: message de l'exception
/// @params[in] : pErrorId	: error id
///
////////////////////////////////////////////////////////////////////////
ExceptionReseau::ExceptionReseau( const std::string& pMessage, const int& pErrorId )
    :mErrorId(pErrorId), runtime_error(pMessage)
{
	GestionnaireReseau::sendMessageToLog(pMessage);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn ExceptionReseau::ExceptionReseau( const std::string& message )
///
/// Constructeur par parametre qui ne prend que le message
///
/// @params[in] : message	: message de l'exception
///
////////////////////////////////////////////////////////////////////////
ExceptionReseau::ExceptionReseau( const std::string& pMessage )
#ifdef WINDOWS
	:mErrorId(WSAGetLastError()), runtime_error(pMessage)
#elif defined(LINUX)
    :mErrorId(-1), runtime_error(pMessage)
#endif
{
	GestionnaireReseau::sendMessageToLog(pMessage);
}

//// WSAGetLastErrorMessage ////////////////////////////////////////////
// A function similar in spirit to Unix's perror() that tacks a canned
// interpretation of the value of WSAGetLastError() onto the end of a
// passed string, separated by a ": ".  Generally, you should implement
// smarter error handling than this, but for default cases and simple
// programs, this function is sufficient.
//
// This function returns a pointer to an internal static buffer, so you
// must copy the data from this function before you call it again.  It
// follows that this function is also not thread-safe.
//
//
#ifdef WINDOWS
// Version de la fonction WSAGetLastErrorMessage du cours de reseau, modifie pour avoir un string en retour
std::string ExceptionReseau::getLastErrorMessage(const char* pcMessagePrefix, int nErrorID/* = 0*/)
{
    // Build basic error string
    static char acErrorBuffer[256];
    std::ostrstream outs(acErrorBuffer, sizeof(acErrorBuffer));
    outs << pcMessagePrefix << ": ";

    // Tack appropriate canned message onto end of supplied message
    // prefix. Note that we do a binary search here: mSocketErrorsList must be
    // sorted by the error constant's value.
    ErrorEntry* pEnd = mSocketErrorsList + mErrorMessagesAmount;
    ErrorEntry Target(nErrorID ? nErrorID : WSAGetLastError());
    ErrorEntry* it = std::lower_bound(mSocketErrorsList, pEnd, Target);
    if ((it != pEnd) && (it->nID == Target.nID))
    {
        outs << it->pcMessage;
    }
    else
    {
        // Didn't find error in list, so make up a generic one
        outs << "unknown error";
    }
    outs << " (" << Target.nID << ")";

    // Finish error message off and return it.
    outs << std::ends;
    acErrorBuffer[sizeof(acErrorBuffer) - 1] = '\0';
    return std::string(acErrorBuffer);
}
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn const char* ExceptionReseau::what()
///
/// Methode qui surcharge la methode what() de runtime_error et qui retourne la chaine qui represente l'erreur
///
/// @return const char* : Chaine qui represente le probleme
///
////////////////////////////////////////////////////////////////////////
const char* ExceptionReseau::what()
{
    std::ostringstream message;
    message << runtime_error::what() << " " << getLastErrorMessage("\nCode d'erreur", mErrorId);
    return message.str().c_str();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ExceptionReseau::throwMe()
///
/// Methode qui throw l'erreur elle-meme
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ExceptionReseau::throwMe()
{
    throw *this; // Throw une exception de type ExceptionReseau
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



