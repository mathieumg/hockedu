#include "SocketTCPServeur.h"
#include "ExceptionsReseau\ExceptionReseau.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn SocketTCPServeur::SocketTCPServeur(const std::string& pDestinationIP, const int& pPortNumber, ConnectionType conType/*=TCP*/, InternetProtocol ipProtocol /*=IPv4*/)
///
/// Constructeur principal
///
/// @param[in] const std::string& pDestinationIP    : Adresse destination sous forme de chaine de caracteres
/// @param[in] const int& pPortNumber               : Numero de port
/// @param[in] ConnectionType pConType              : Type de connection (TCP = default ou UDP)
/// @param[in] InternetProtocol pIpProtocol         : Protocole IP (IPv4 = default ou IPv6)
///
////////////////////////////////////////////////////////////////////////
SocketTCPServeur::SocketTCPServeur(const std::string& pDestinationIP, const int& pPortNumber, ConnectionType pConType/*=TCP*/, InternetProtocol pIpProtocol /*=IPv4*/)
	:Socket(pDestinationIP, pPortNumber, pConType, pIpProtocol)
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn SocketTCPServeur::~SocketTCPServeur()
///
/// Destructeur par default qui s'occupe de fermer le socket 
///
////////////////////////////////////////////////////////////////////////
SocketTCPServeur::~SocketTCPServeur()
{
}




////////////////////////////////////////////////////////////////////////
///
/// @fn SocketTCPServeur::listen( uint32_t nbConnections )
///
/// Listens to 
///
/// @param[in] uint32_t nbConnections
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SocketTCPServeur::listen( uint32_t nbConnections /* = 1*/)
{
    if(::listen(mSocket, nbConnections) == -1)
    {
        throw ExceptionReseau("Error while trying to listen to the socket.");
    }   
}



////////////////////////////////////////////////////////////////////////
///
/// @fn SocketTCPServeur::accept( __out sockaddr* addr, uint32_t addrlen )
///
/// Accepts an incoming connection.
///
/// @param[in] __out sockaddr * addr Optional, returns the address of the connecting socket.
/// @param[in] uint32_t addrlen Length of addr.
///
/// @return Socket* : Pointeur vers le nouveau Socket qui est connecte
///
////////////////////////////////////////////////////////////////////////
Socket* SocketTCPServeur::accept( __out sockaddr* addr, uint32_t* addrlen )
{
    sockaddr_in* temp;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    SOCKET wTempSocket;
#else
    int wTempSocket;
#endif
    if(addr == NULL && addrlen == NULL)
    {

        int taille = sizeof(*temp);
        wTempSocket = ::accept(mSocket, (sockaddr*)temp, &taille);
    }
    else
    {
        wTempSocket = ::accept(mSocket, addr, (int*)addrlen);
    }
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    if(wTempSocket == INVALID_SOCKET)
#else
    if(wTempSocket == -1)
#endif
    {
        throw ExceptionReseau("Erreur a la connection du socket ");
    }



    if(addr == NULL && addrlen == NULL)
    {
        return new Socket(wTempSocket, temp, TCP);
    }
    else
    {
        sockaddr_in* addr2 = new sockaddr_in;
        memcpy(addr2, addr, sizeof(addr));
        return new Socket(wTempSocket, (sockaddr_in*)addr, TCP);
    }

}














