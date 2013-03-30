//////////////////////////////////////////////////////////////////////////////
/// @file ExceptionReseau.cpp
/// @author Mathieu Parent
/// @date 2013-01-05
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "../GestionnaireReseau.h"
#include <strstream>
#include <algorithm>
#include <sstream>
#include <ctime>
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

ErrorEntry ExceptionReseau::mSocketErrorsList[] = {
    ErrorEntry(RAZORGAME_NO_ERROR,        "No error"),
    ErrorEntry(RAZORGAME_EBADF,           "Bad file number"),
    ErrorEntry(RAZORGAME_EACCES,          "Permission denied"),
    ErrorEntry(RAZORGAME_EFAULT,          "Bad address"),
    ErrorEntry(RAZORGAME_EINVAL,          "Invalid argument"),
    ErrorEntry(RAZORGAME_EMFILE,          "Too many open sockets"),
    ErrorEntry(RAZORGAME_EWOULDBLOCK,     "Operation would block"),
    ErrorEntry(RAZORGAME_EINPROGRESS,     "Operation now in progress"),
    ErrorEntry(RAZORGAME_EALREADY,        "Operation already in progress"),
    ErrorEntry(RAZORGAME_ENOTSOCK,        "Socket operation on non-socket"),
    ErrorEntry(RAZORGAME_EDESTADDRREQ,    "Destination address required"),
    ErrorEntry(RAZORGAME_EMSGSIZE,        "Message too long"),
    ErrorEntry(RAZORGAME_EPROTOTYPE,      "Protocol wrong type for socket"),
    ErrorEntry(RAZORGAME_ENOPROTOOPT,     "Bad protocol option"),
    ErrorEntry(RAZORGAME_EPROTONOSUPPORT, "Protocol not supported"),
    ErrorEntry(RAZORGAME_ESOCKTNOSUPPORT, "Socket type not supported"),
    ErrorEntry(RAZORGAME_EOPNOTSUPP,      "Operation not supported on socket"),
    ErrorEntry(RAZORGAME_EPFNOSUPPORT,    "Protocol family not supported"),
    ErrorEntry(RAZORGAME_EAFNOSUPPORT,    "Address family not supported"),
    ErrorEntry(RAZORGAME_EADDRINUSE,      "Address already in use"),
    ErrorEntry(RAZORGAME_EADDRNOTAVAIL,   "Can't assign requested address"),
    ErrorEntry(RAZORGAME_ENETDOWN,        "Network is down"),
    ErrorEntry(RAZORGAME_ENETUNREACH,     "Network is unreachable"),
    ErrorEntry(RAZORGAME_ENETRESET,       "Net connection reset"),
    ErrorEntry(RAZORGAME_ECONNABORTED,    "Software caused connection abort"),
    ErrorEntry(RAZORGAME_ECONNRESET,      "Connection reset by peer"),
    ErrorEntry(RAZORGAME_ENOBUFS,         "No buffer space available"),
    ErrorEntry(RAZORGAME_EISCONN,         "Socket is already connected"),
    ErrorEntry(RAZORGAME_ENOTCONN,        "Socket is not connected"),
    ErrorEntry(RAZORGAME_ESHUTDOWN,       "Can't send after socket shutdown"),
    ErrorEntry(RAZORGAME_ETOOMANYREFS,    "Too many references, can't splice"),
    ErrorEntry(RAZORGAME_ETIMEDOUT,       "Connection timed out"),
    ErrorEntry(RAZORGAME_ECONNREFUSED,    "Connection refused"),
    ErrorEntry(RAZORGAME_ELOOP,           "Too many levels of symbolic links"),
    ErrorEntry(RAZORGAME_ENAMETOOLONG,    "File name too long"),
    ErrorEntry(RAZORGAME_EHOSTDOWN,       "Host is down"),
    ErrorEntry(RAZORGAME_EHOSTUNREACH,    "No route to host"),
    ErrorEntry(RAZORGAME_ENOTEMPTY,       "Directory not empty"),
    ErrorEntry(RAZORGAME_EUSERS,          "Too many users"),
    ErrorEntry(RAZORGAME_EDQUOT,          "Disc quota exceeded"),
    ErrorEntry(RAZORGAME_ESTALE,          "Stale NFS file handle")
#ifdef WINDOWS
    ,
    ErrorEntry(RAZORGAME_EDISCON,         "Graceful shutdown in progress"),
    ErrorEntry(RAZORGAME_EREMOTE,         "Too many levels of remote in path"),
    ErrorEntry(RAZORGAME_EPROCLIM,        "Too many processes"),
    ErrorEntry(RAZORGAME_SYSNOTREADY,     "Network system is unavailable"),
    ErrorEntry(RAZORGAME_VERNOTSUPPORTED, "Winsock version out of range"),
    ErrorEntry(RAZORGAME_NOTINITIALISED,  "WSAStartup not yet called"),
    ErrorEntry(RAZORGAME_HOST_NOT_FOUND,  "Host not found"),
    ErrorEntry(RAZORGAME_NO_DATA,         "No host data of that type was found")
#endif
};
const int ExceptionReseau::mErrorMessagesAmount = sizeof(mSocketErrorsList) / sizeof(ErrorEntry);

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
	:mErrorId(FacadePortability::getLastError()), runtime_error(pMessage)
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
    ErrorEntry Target(nErrorID ? nErrorID : FacadePortability::getLastError());
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



