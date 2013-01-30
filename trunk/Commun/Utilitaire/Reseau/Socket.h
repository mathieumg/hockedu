#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#endif
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include <memory>

enum InternetProtocol {IPv4 = AF_INET, IPv6 = AF_INET6, UNSPECIFIED = AF_UNSPEC};
enum ConnectionType {UDP, TCP};
enum ConnectionState {CONNECTED, CONNECTING, NOT_CONNECTED};



class Socket : public std::enable_shared_from_this<Socket>
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
	uint32_t sendto(uint8_t* msg, uint32_t msglen, sockaddr* to, bool pBlock = false);
	static void getaddrinfo( uint8_t* addr, uint16_t port, addrinfo* result);
	static void freeaddrinfo(addrinfo* addr);
	void setsockopt(uint32_t level, uint32_t optionName, uint8_t* optionValue, uint32_t optionSize);
	
	std::string getAdresseSource() const;
	std::string getAdresseDestination() const;

	inline ConnectionType getConnectionType() const {return mConnectionType;}
	inline ConnectionState getConnectionState() const {return mConnectionState;}
	void setConnectionState(ConnectionState pConnectionState);

    inline HANDLE getMutexActiviteSocket() const { return mMutexActiviteSocket; }

    // Methode pour connecter les sockets (bind() pour UDP et connect ou accept pour TCP)
    void init();

    // Methode pour deconnecter le socket (s'il est brise par exemple)
    void disconnect();

    // Methode qui attend que le socket soit lisible avec un Timeout max (True si libre)
    bool attendreSocket(const int& pTimeout) const;

    /// Accessors of mPendingDelete
    inline bool isPendingDelete() const { return mPendingDelete; }
    inline void flagToDelete() { mPendingDelete = true; }

private:
    
    HANDLE mMutexActiviteSocket;
    bool mPendingDelete;

    

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
