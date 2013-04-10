#ifdef WINDOWS
#include <winsock2.h>
#include <Mswsock.h>
#endif
#include "Socket.h"
#include "ExceptionsReseau/ExceptionReseau.h"
#include "GestionnaireReseau.h"
#include "ExceptionsReseau/ExceptionReseauSocketDeconnecte.h"
#include "Utilitaire.h"


#include "PacketBuilder.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::Socket(const std::string& pDestinationIP, const int& pPortNumber, ConnectionType conType/*=TCP*/, InternetProtocol ipProtocol /*=IPv4*/)
///
/// Constructeur principal
///
/// @param[in] const std::string& pDestinationIP    : Adresse destination sous forme de chaine de caracteres
/// @param[in] const int& pPortNumber               : Numero de port
/// @param[in] ConnectionType pConType              : Type de connection (TCP = default ou UDP)
/// @param[in] InternetProtocol pIpProtocol         : Protocole IP (IPv4 = default ou IPv6)
///
////////////////////////////////////////////////////////////////////////
Socket::Socket(const std::string& pDestinationIP, const int& pPortNumber, ConnectionType pConType/*=TCP*/, InternetProtocol pIpProtocol /*=IPv4*/)
    :mIpProtocol(pIpProtocol),mFlags(0),mOnConnectionCallback(NULL),mId("")
{
	mConnectionType = pConType;
	mSocketInfo = new sockaddr_in;
	memset((char*) mSocketInfo, 0, sizeof(*mSocketInfo));
	mSocketInfo->sin_addr.s_addr = inet_addr(pDestinationIP.c_str());
	mSocketInfo->sin_family = pIpProtocol;
    mSocketInfo->sin_port = htons(pPortNumber);
    mConnectionState = NOT_CONNECTED;
    mIndexPaquet = -1;

	if(pConType == UDP)
	{
		//UDP datagram
		mSocket = socket(pIpProtocol, SOCK_DGRAM, IPPROTO_UDP);
        mConnectionState = CONNECTED;
	}
	else if(pConType == TCP)
	{
		//TCP stream
		mSocket = socket(pIpProtocol, SOCK_STREAM, IPPROTO_TCP);
	}

	if(mSocket == INVALID_SOCKET)
	{
		throw ExceptionReseau("Could not initialize socket.");
	}

    FacadePortability::createMutex(mMutexActiviteSocket);

	BOOL option = TRUE;
	setsockopt(SOL_SOCKET, SO_REUSEADDR, (uint8_t*)&option, sizeof(option));
}




////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::Socket( SOCKET socket, sockaddr_in* socketInfo, ConnectionType pConnectionType /* = TCP*/)
///
/// Constructeur secondaire qui peut etre appele lors d'un accept de socket de type TCP (Private)
/// Note: Le socket passe en parametre est suppose etre connecte
///
/// @param[in] SOCKET socket                : Socket de winsock deja connecte
/// @param[in] sockaddr_in* socketInfo      : sockaddr_in associe au socket passe en parametre
///
////////////////////////////////////////////////////////////////////////
Socket::Socket( HANDLE_SOCKET socket, sockaddr_in* socketInfo, ConnectionType pConnectionType)
    :mFlags(0),mId("")
{
    mIpProtocol =  IPv4;
    mConnectionType = pConnectionType;
    mSocketInfo = socketInfo;
    mSocket = socket;
    mIndexPaquet = -1;
    if(pConnectionType = TCP)
    {
	    mConnectionState = CONNECTING;
    }
    else
    {
        mConnectionState = CONNECTED;
    }

    FacadePortability::createMutex(mMutexActiviteSocket);

    if(mSocket == INVALID_SOCKET)
    {
        int i = FacadePortability::getLastError();
        throw ExceptionReseau("Could not initialize socket (Copy).");
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::~Socket()
///
/// Destructeur par default qui s'occupe de fermer le socket
///
////////////////////////////////////////////////////////////////////////
Socket::~Socket()
{
	//delete mSocketInfo; // TODO: Trouver comment delete ca
	if(mSocket == INVALID_SOCKET)
    {
		return;
    }
    disconnect();

}




////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::bind( unsigned short port )
///
/// Binds socket to accept connections from ipAddr on specified port.
///
/// @param[in] unsigned short port
///
/// @return
///
////////////////////////////////////////////////////////////////////////
void Socket::bind( uint8_t* ipAddr, uint16_t port )
{
	//Structure definition for the bind call.
	mSocketInfo->sin_port = htons(port);
	mSocketInfo->sin_addr.s_addr = inet_addr((char*)ipAddr);

	int ret;

	//Binds the socket
	ret = ::bind(mSocket, (sockaddr*)mSocketInfo, sizeof(*mSocketInfo));
	if(ret == -1)
	{
		throw ExceptionReseau("Error while binding socket.");
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::bind( unsigned short port )
///
/// Binds socket to accept connections from ipAddr on specified port.
///
/// @param[in] unsigned short port
///
/// @return
///
////////////////////////////////////////////////////////////////////////
void Socket::bind()
{
    //Structure definition for the bind call.
    sockaddr_in* wStructServer;
    if( mConnectionType == TCP )
    {
        wStructServer = new sockaddr_in;
        wStructServer->sin_family = mSocketInfo->sin_family;
        wStructServer->sin_addr.s_addr = inet_addr(getAdresseSource().c_str());
        wStructServer->sin_port = mSocketInfo->sin_port; // Deja en format reseau
    }
    else
    {
        // Sinon on utilise le socketInfo directement
        wStructServer = mSocketInfo;
    }
#if !SHIPPING
    std::stringstream message;
    message << (mConnectionType == TCP ? "TCP" : "UDP") << " BIND ON" << std::endl;
    message << "IP  : " << inet_ntoa(wStructServer->sin_addr) << std::endl;
    message << "PORT: "<< ntohs(mSocketInfo->sin_port) << std::endl;
    std::cout << message.str();
#endif

    int ret;

    //Binds the socket
    ret = ::bind(mSocket, (sockaddr*)wStructServer, sizeof(*wStructServer));
    if( mConnectionType == TCP )
    {
        delete wStructServer;
    }
    if(ret == -1)
    {
        throw ExceptionReseau("Error while binding socket.");
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::recvfrom( __out uint8_t* msg, uint32_t msglen,__out sockaddr* from )
///
/// Receives data from an unconnected datagram socket.
///
/// @param[in] __out char * msg The received data.
/// @param[in] __out sockaddr * from The information on the socket that sent the message.
/// @param[in] bool pBlock  : True is the call can block and wait for input
///
/// @return uint32_t    : the size received
///
////////////////////////////////////////////////////////////////////////
uint32_t Socket::recvfrom( uint8_t* msg, uint32_t msglen, sockaddr* from, bool pBlock/*=true*/ )
{
#ifdef WINDOWS
    int fromlen = sizeof(*from);
#elif defined(LINUX)
    unsigned int fromlen = sizeof(*from);
#endif
	// :: pour dire que c'est la fonction globale de ce nom et pas la methode
    if(!pBlock)
    {
        if(!attendreSocket(0)) // select retourne le nombre de sockets qui ne bloqueront pas et qui font partis de readfds
        {
            // On ne peut pas appeler recvfrom car ca va bloquer
            return 0;
        }
    }


    // On effectue le traitement sur le socket
    int wReturnValue = ::recvfrom(mSocket, (char*)msg, msglen, 0, from, &fromlen);

	if(wReturnValue == -1)
	{
        GestionnaireReseau::obtenirInstance()->throwExceptionReseau();
	}
    return wReturnValue;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::sendto( uint8_t* msg, uint32_t msglen, sockaddr* to )
///
/// Sends a message to an unconnected datagram socket.
///
/// @param[in] uint8_t* msg Message to send.
/// @param[in] uint32_t msglen The length of the message to send.
/// @param[in] sockaddr * to Information regarding the socket to send the data to.
/// @param[in] bool pBlock  : True is the call can block and wait for input
///
/// @return uint32_t    : size sent
///
////////////////////////////////////////////////////////////////////////
uint32_t Socket::sendto( const uint8_t* msg, uint32_t msglen,sockaddr* to, bool pBlock /* = false */ )
{
    if(!pBlock)
    {
        if(!attendreSocket(0)) // select retourne le nombre de sockets qui ne bloqueront pas et qui font partis de readfds
        {
            // On ne peut pas appeler sendto car ca va bloquer (buffer plein)
            return 0;
        }
    }

	if(::sendto(mSocket, (char*)msg, msglen, 0, to, sizeof(*to)) == -1)
	{
		GestionnaireReseau::obtenirInstance()->throwExceptionReseau();
	}
    return msglen;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::recv( __out uint8_t* buf, uint32_t bufLen )
///
/// Receives data from the socket.
///
/// @param[in] __out uint8_t * buf
/// @param[in] uint32_t bufLen
/// @param[in] bool pBlock  : True is the call can block and wait for input
///
/// @return uint32_t The amount of bytes read.
///
////////////////////////////////////////////////////////////////////////
uint32_t Socket::recv( uint8_t* buf, uint32_t bufLen, bool pBlock /* = false*/ )
{
    if(mConnectionType == UDP)
    {
        sockaddr_in wTemp;
        return recvfrom(buf, bufLen, (sockaddr*)&wTemp, pBlock);
    }
    else
    {
        if(!pBlock)
        {
            if(!attendreSocket(0)) // select retourne le nombre de sockets qui ne bloqueront pas et qui font partis de readfds
            {
                // On ne peut pas appeler recv car ca va bloquer
                return 0;
            }
        }


        uint32_t ret = ::recv(mSocket, (char*)buf, bufLen, 0);

        if(ret == -1)
        {
            GestionnaireReseau::obtenirInstance()->throwExceptionReseau();
        }

        return ret;
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::getaddrinfo( uint8_t* addr, uint16_t port, __out addrinfo* result )
///
/// Gets the address information from the specified string.
///
/// @param[in] uint8_t * addr The string from which to get the address info
/// @param[in] uint16_t port The port on which to get the address info
/// @param[in] __out addrinfo * result The
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Socket::getaddrinfo( uint8_t* addr, uint16_t port, addrinfo* result )
{
	char portString[5];

	sprintf_s(portString, "%d", port);

	//std::cout << (char*)addr << std::endl;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if(::getaddrinfo((char*)addr, portString, &hints, &result))
	{
		throw ExceptionReseau("Error while getting the address information.");
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::freeaddrinfo( addrinfo* addr )
///
/// Frees an addrinfo's allocated memory.
///
/// @param[in] addrinfo * addr Pointer to the addrinfo to deallocate.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Socket::freeaddrinfo( addrinfo* addr )
{
	::freeaddrinfo(addr);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::connect( sockaddr* name, sockaddr* namelen )
///
/// Establishes a connection to a specified socket.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Socket::connect( )
{
	if(::connect(mSocket, (sockaddr*) mSocketInfo, sizeof(sockaddr_in)) == -1)
	{
        int wError = FacadePortability::getLastError();
		throw ExceptionReseau("Error while connecting to the specified address: " + getAdresseDestination());
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::send( uint8_t* msg, uint32_t msglen )
///
/// Sends a message to the socket.
///
/// @param[in] uint8_t * msg Message to send
/// @param[in] uint32_t msglen Length of the message to send
/// @param[in] bool pBlock  : True is the call can block and wait for input
///
/// @return uint32_t    : The size sent
///
////////////////////////////////////////////////////////////////////////
uint32_t Socket::send( const uint8_t* msg, uint32_t msglen, bool pBlock /* = false */ )
{
    uint32_t lenSent = -1;
    if(mConnectionType == UDP)
    {
        // Si udp, on appelle la methode sendto
        lenSent = sendto(msg, msglen, (sockaddr*)mSocketInfo, pBlock);
    }
    else
    {
        if(!pBlock)
        {
            if(!attendreSocket(0)) // select retourne le nombre de sockets qui ne bloqueront pas et qui font partis de readfds
            {
                // On ne peut pas appeler send car ca va bloquer
                return 0;
            }
        }
        lenSent = ::send(mSocket, (const char*)msg, msglen, 0);
    }
    if(lenSent == -1)
    {
        GestionnaireReseau::obtenirInstance()->throwExceptionReseau("Erreur lors du send");
    }
    return lenSent;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::setsockopt( uint32_t level, uint32_t optionName, uint8_t* optionValue, uint32_t optionSize )
///
///
///
/// @param[in] uint32_t level
/// @param[in] uint32_t optionName
/// @param[in] uint8_t * optionValue
/// @param[in] uint32_t optionSize
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Socket::setsockopt( uint32_t level, uint32_t optionName, uint8_t* optionValue, uint32_t optionSize )
{
	if(::setsockopt(mSocket, level, optionName, (char*)&optionValue, sizeof(optionSize)) == -1)
	{
		throw ExceptionReseau("Error while setting socket options.");
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::getIpAddress() const
///
/// Returns the IP address associated with the current socket. (Source)
///
///
/// @return std::string The IP address
///
////////////////////////////////////////////////////////////////////////
std::string Socket::getAdresseSource() const
{
    try
    {
	    return GestionnaireReseau::obtenirInstance()->getAdresseIPLocaleAssociee(inet_ntoa(mSocketInfo->sin_addr));
    }
    catch(ExceptionReseau&)
    {
        return "";
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn Socket::getAdresseDestination() const
///
/// Returns the IP address associated with the current socket. (Destination)
///
///
/// @return std::string The IP address
///
////////////////////////////////////////////////////////////////////////
std::string Socket::getAdresseDestination() const
{
	return inet_ntoa(mSocketInfo->sin_addr);
}



// THIS METHOD CAN BLOCK!!!!!!!!!!!!!!!!!
////////////////////////////////////////////////////////////////////////
///
/// @fn bool Socket::initClient(  )
///
/// Initialise la connection d'un socket de client
///
///
/// @return bool : indique si le comportement de la connection est normal
///                vrai si le processus de connection doit continuer
///                faux si on arrete les tentatives
///
////////////////////////////////////////////////////////////////////////
ConnectionState Socket::initClient()
{
    // Doit avoir le mutex pour faire quoique ce soit
    FacadePortability::takeMutex(mMutexActiviteSocket);

    // par défaut on tente la reconnection, si la connection réussi, celle-ci override le comportement de reconnection
    ConnectionState attemptReconnect = CONNECTING;
    if(isPendingCancel() || isPendingDelete())
    {
        GestionnaireReseau::obtenirInstance()->transmitEvent(CONNECTION_CANCELED);
        attemptReconnect = NOT_CONNECTED;
    }
    else
    {
        // Si le socket est mauvais, on le reinitialise
        if(mSocket == INVALID_SOCKET)
        {
            if(mConnectionType == UDP)
            {
                mSocket = socket(mIpProtocol, SOCK_DGRAM, IPPROTO_UDP);
            }
            else if(mConnectionType == TCP)
            {
                mSocket = socket(mIpProtocol, SOCK_STREAM, IPPROTO_TCP);
            }
        }

        if(mConnectionType == UDP)
        {
            //UDP datagram

            // Dans ce cas, c'est que le client ne pouvait plus parler au serveur (pas encore gerer. a faire plus tard)
            // Pour l'instant, on est "toujours" connecte
            setConnectionState(CONNECTED);

        }
        else if(mConnectionType == TCP)
        {
            // TCP CLIENT
            try
            {
                int wConfirmation = USER_DISCONNECTED;
                connect();
                // Si connect fonctionne, il faut envoyer notre nom de player au serveur
                std::string wPlayerName = GestionnaireReseau::obtenirInstance()->getPlayerName();
                send((uint8_t*) wPlayerName.c_str(), (uint32_t) (wPlayerName.length()+1), true); // +1 pour avoir le caractere de fin de string

                // On recoit le message de confirmation, ne pas bloquer
                char wReceptionValue[3];
                if(attendreSocket(15)) // select retourne le nombre de sockets qui ne bloqueront pas et qui font partis de readfds
                {
                    recv((uint8_t*) &wReceptionValue, 3, true);
                    if(sscanf_s(wReceptionValue,"%i",&wConfirmation) == 0)
                    {
                        // sscanf ne fonctionne pas
                        wConfirmation = USER_DISCONNECTED;
                    }

                    if(wConfirmation == SEND_PASSWORD_REQUEST)
                    {
                        // On doit envoyer le mot de passe
                        std::string wPassword = GestionnaireReseau::obtenirInstance()->getPlayerPassword();
                        send((uint8_t*) wPassword.c_str(), (uint32_t) (wPassword.length()+1), true); // +1 pour avoir le caractere de fin de string
                        char wReceptionValue2[3];
                        recv((uint8_t*) &wReceptionValue2, 3, true);
                        if(sscanf_s(wReceptionValue2,"%i",&wConfirmation) == 0)
                        {
                            // sscanf ne fonctionne pas
                            wConfirmation = USER_DISCONNECTED;
                        }

                    }

                }
                else
                {
                    // Timeout
                    wConfirmation = RECONNECTION_TIMEOUT;
                }

                if(wConfirmation != USER_CONNECTED)
                {
                    // Connection refusee
                    // probleme lors de la confirmation de la connection, on envoi l'événement et on arrete d'essayer de ce connecter
                    attemptReconnect = NOT_CONNECTED;
                    disconnect();
                    GestionnaireReseau::obtenirInstance()->transmitEvent(EventCodes(wConfirmation));
                    GestionnaireReseau::obtenirInstance()->sendMessageToLog("Connection refusee. Type: TCP CLIENT. Adresse: " + getAdresseDestination());
                }
                else
                {
                    attemptReconnect = CONNECTED;
                }
            }
            catch(ExceptionReseau&)
            {
                // Could not connect
                // on n'arrive pas à ce connecter, on relance la tentative un peu plus tard
                attemptReconnect = CONNECTING;

                GestionnaireReseau::obtenirInstance()->sendMessageToLog("Appel a connect() impossible. Type: TCP CLIENT. Adresse: " + getAdresseDestination());
            }

        }
    }
    FacadePortability::releaseMutex(mMutexActiviteSocket);

    return attemptReconnect;
}

// THIS METHOD CAN BLOCK!!!!!!!!!!!!!!!!!
////////////////////////////////////////////////////////////////////////
///
/// @fn bool Socket::initServer(  )
///
/// Initialise la connection d'un socket de serveur
///
///
/// @return bool : indique si le comportement de la connection est normal
///                vrai si le processus de connection doit continuer
///                faux si on arrete les tentatives
///
////////////////////////////////////////////////////////////////////////
ConnectionState Socket::initServer()
{
    // Doit avoir le mutex pour faire quoique ce soit
    FacadePortability::takeMutex(mMutexActiviteSocket);

    // par défaut on tente la reconnection, si la connection réussi, celle-ci override le comportement de reconnection
    ConnectionState attemptReconnect = CONNECTING;
    if(isPendingCancel() || isPendingDelete())
    {
        GestionnaireReseau::obtenirInstance()->transmitEvent(CONNECTION_CANCELED);
        attemptReconnect = NOT_CONNECTED;
    }
    else
    {
        // Si le socket est mauvais, on le reinitialise
        if(mSocket == INVALID_SOCKET)
        {
            if(mConnectionType == UDP)
            {
                mSocket = socket(mIpProtocol, SOCK_DGRAM, IPPROTO_UDP);
            }
            else if(mConnectionType == TCP)
            {
                mSocket = socket(mIpProtocol, SOCK_STREAM, IPPROTO_TCP);
            }
        }

        if(mConnectionType == UDP)
        {
            //UDP datagram

            try
            {
                bind();
            }
            catch(ExceptionReseau&)
            {

            }

        }
        else if(mConnectionType == TCP)
        {
            throw ExceptionReseau("Operation invalide. Init Server ne devrait pas etre appele dans le cas d'un socket TCP. Le client se charge de la creation d'un nouveau Socket.");
            attemptReconnect = NOT_CONNECTED;

        }
    }
    FacadePortability::releaseMutex(mMutexActiviteSocket);
    return attemptReconnect;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool Socket::attendreSocket( const SOCKET& pSocket, const int& pTimeout ) const
///
/// Methode pour attendre et voir si un socket se lebere dans un delais donne
///
/// @param[in] const int& pTimeout      : Duree max d'attente (en sec)
///
/// @return bool    : True si Socket libre au return, False si Timeout atteint et socket encore occuppe
///
////////////////////////////////////////////////////////////////////////
bool Socket::attendreSocket( const int& pTimeout ) const
{
    //checkf(pTimeout>=0);
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(mSocket, &readfds); // Set the File Descriptor to the one of the socket
    timeval tv = { pTimeout }; // Set timeout
    return select((int)mSocket+1, &readfds, NULL, NULL, &tv) > 0; // select retourne le nombre de sockets qui ne bloqueront pas et qui font partis de readfds
    // Premier parametre pour linux seulement, ignore en Windows
}


void Socket::disconnect()
{
    try
    {
        FacadePortability::closeSocket(mSocket);
    }
    catch(...){}
    mSocket = INVALID_SOCKET;
}






void Socket::setConnectionState( ConnectionState pConnectionState )
{
    // On ne peut pas changer le state d'un socket UDP
    if(mConnectionType == UDP)
    {
        return;
    }

    if(mConnectionState != pConnectionState && pConnectionState == CONNECTED && mOnConnectionCallback)
    {
        mOnConnectionCallback();
        mOnConnectionCallback = NULL;
    }
	mConnectionState = pConnectionState;

	ConnectionStateEvent event;
	event.mState = mConnectionState;
	GestionnaireReseau::obtenirInstance()->socketConnectionStateEvent(shared_from_this(),event);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void Socket::cancelConnection()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void Socket::cancelConnection()
{
    flagToCancel();
}



void Socket::setSocketInfo( sockaddr_in* pSockInfo )
{
    //checkf(pSockInfo);
    // delete the old socketInfo
    delete mSocketInfo;
    mSocketInfo = pSockInfo;
}



void Socket::setOnConnectionCallback( OnConnectionCallback pOnConnectionSendCallback )
{
    mOnConnectionCallback = pOnConnectionSendCallback;
    // Si deja connecte, on appelle immediatement le callback
    if(mConnectionState == CONNECTED && mOnConnectionCallback)
    {
        mOnConnectionCallback();
    }
    
}
























