#include "Socket.h"


class SocketTCPServeur : public Socket
{
public:
	SocketTCPServeur(const std::string& pDestinationIP, const int& pPortNumber, ConnectionType conType = TCP, InternetProtocol ipProtocol = IPv4);
	~SocketTCPServeur();
	
    void listen(uint32_t nbConnections = 1);
    SPSocket accept(__out sockaddr* addr, uint32_t* addrlen);

};

typedef std::shared_ptr<SocketTCPServeur> SPSocketTCPServer;
