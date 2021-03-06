#pragma once
#include <memory>
#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include "FacadePortability.h"
#include <functional>
#include "NetworkEnums.h"

#ifdef LINUX
#define INVALID_SOCKET -1
#define SD_BOTH 2
#endif

enum InternetProtocol {IPv4 = AF_INET, IPv6 = AF_INET6, UNSPECIFIED = AF_UNSPEC};



/// lorsque changement pour ne plus deriver de string, cacher une string pour l'utilisation dans le logging debug
class SocketIdentifer : public std::string
{
public:
    /// constructor
    SocketIdentifer(const std::string& s):std::string(s){}
    SocketIdentifer(const char* s):std::string(s){}

    const std::string& ToString()const{return *this;}
    const char* ToCString()const{return std::string::c_str();}
private:
    const char *c_str() const{}
};


typedef std::function<void()> OnConnectionCallback;

enum SocketFlags
{
    SOCKET_FLAGS_PENDING_DELETE = (1<<0),
    SOCKET_FLAGS_PENDING_CANCEL = (1<<1),
};

class Socket : public std::enable_shared_from_this<Socket>
{
public:
	Socket(const std::string& pDestinationIP, const int& pPortNumber, ConnectionType conType = TCP, InternetProtocol ipProtocol = IPv4);
    Socket(HANDLE_SOCKET socket, sockaddr_in* socketInfo, ConnectionType pConnectionType); // Pas bien adaptee pour notre projet
    ~Socket();

    void bind( uint8_t* ipAddr, const uint16_t port);
    void bind();

	uint32_t recv( uint8_t* buf, uint32_t bufLen, bool pBlock = false);
	uint32_t send( const uint8_t* msg, uint32_t msglen, bool pBlock = false);
	uint32_t recvfrom(uint8_t* msg, uint32_t msglen, sockaddr* from, bool pBlock = true);
	uint32_t sendto(const uint8_t* msg, uint32_t msglen, sockaddr* to, bool pBlock = false);
	static void getaddrinfo( uint8_t* addr, uint16_t port, addrinfo* result);
	static void freeaddrinfo(addrinfo* addr);
	void setsockopt(uint32_t level, uint32_t optionName, uint8_t* optionValue, uint32_t optionSize);
    void setSocketInfo(sockaddr_in* pSockInfo);

	std::string getAdresseSource() const;
	std::string getAdresseDestination() const;

	inline ConnectionType getConnectionType() const {return mConnectionType;}
	inline ConnectionState getConnectionState() const {return mConnectionState;}
	void setConnectionState(ConnectionState pConnectionState);

    inline HANDLE_MUTEX getMutexActiviteSocket() const { return mMutexActiviteSocket; }

    // Methode pour initialiser le Socket en mode Client
    ConnectionState initClient();

    // Methode pour initialiser le Socket en mode Server
    ConnectionState initServer();

    void cancelConnection();

    // Methode pour deconnecter le socket (s'il est brise par exemple)
    void disconnect();

    // Methode qui attend que le socket soit lisible avec un Timeout max (True si libre)
    bool attendreSocket(const int& pTimeout) const;

    /// Accessors of mPendingDelete
    inline bool isPendingDelete() const { return !!(mFlags & SOCKET_FLAGS_PENDING_DELETE); }
    inline void flagToDelete() { mFlags |= SOCKET_FLAGS_PENDING_DELETE; }
    inline bool isPendingCancel() const { return !!(mFlags & SOCKET_FLAGS_PENDING_CANCEL); }
    inline void flagToCancel() { mFlags |= SOCKET_FLAGS_PENDING_CANCEL; }
    inline void removeCancelFlag(){  mFlags &= ~SOCKET_FLAGS_PENDING_CANCEL; }

    inline int getIndexPaquet() const { return mIndexPaquet; }
    inline void setIndexPaquet(int val) { mIndexPaquet = val; }

    void setOnConnectionCallback(OnConnectionCallback pOnConnectionSendCallback);

    /// Accessors of mId
    inline const SocketIdentifer& getId() const { return mId; }
    inline void setId(const SocketIdentifer& pVal) { mId = pVal; }

private:

    HANDLE_MUTEX mMutexActiviteSocket;
    int mFlags;

    void connect();
    OnConnectionCallback mOnConnectionCallback;

	InternetProtocol mIpProtocol;
	ConnectionType mConnectionType;
	ConnectionState mConnectionState;
	//Will stock the information of the socket
	sockaddr_in* mSocketInfo;
    int mIndexPaquet;
    SocketIdentifer mId;

protected:


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    SOCKET mSocket;
#else
    int mSocket;
#endif

};

typedef std::shared_ptr<Socket> SPSocket;
