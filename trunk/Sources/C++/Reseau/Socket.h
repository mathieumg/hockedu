#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <tr1/memory>
#endif
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdexcept>


enum InternetProtocol {IPv4 = AF_INET, IPv6 = AF_INET6, UNSPECIFIED = AF_UNSPEC};
enum ConnectionType {UDP, TCP};
enum ConnectionState {CONNECTED, CONNECTING, NOT_CONNECTED};

enum SocketFlags
{
    SOCKET_FLAGS_PENDING_DELETE = (1<<0),
    SOCKET_FLAGS_PENDING_CANCEL = (1<<1),
};

class Socket : public std::tr1::enable_shared_from_this<Socket>
{
public:
	Socket(const std::string& pDestinationIP, const int& pPortNumber, ConnectionType conType = TCP, InternetProtocol ipProtocol = IPv4);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    Socket(SOCKET socket, sockaddr_in* socketInfo, ConnectionType pConnectionType); // Pas bien adaptee pour notre projet
#else
    Socket(int socket, sockaddr_in* socketInfo, ConnectionType pConnectionType);
#endif
    ~Socket();

    void bind( uint8_t* ipAddr, const uint16_t port);
    void bind();

	uint32_t recv( __out uint8_t* buf, uint32_t bufLen, bool pBlock = false);
	uint32_t send( const uint8_t* msg, uint32_t msglen, bool pBlock = false);
	uint32_t recvfrom(__out uint8_t* msg, uint32_t msglen, __out sockaddr* from, bool pBlock = true);
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

    inline HANDLE getMutexActiviteSocket() const { return mMutexActiviteSocket; }

    // Methode pour connecter les sockets (bind() pour UDP et connect ou accept pour TCP)
    ConnectionState init();
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
private:

    HANDLE mMutexActiviteSocket;
    int mFlags;

    void connect();


	InternetProtocol mIpProtocol;
	ConnectionType mConnectionType;
	ConnectionState mConnectionState;
	//Will stock the information of the socket
	sockaddr_in* mSocketInfo;

protected:


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    SOCKET mSocket;
#else
    int mSocket;
#endif

};

typedef std::shared_ptr<Socket> SPSocket;
