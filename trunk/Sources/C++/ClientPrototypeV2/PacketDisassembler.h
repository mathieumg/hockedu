
#ifndef PACKET_DISASSEMBLER_H
#define PACKET_DISASSEMBLER_H

#include <string>
#include <winsock2.h>
#define NETWORK_BUFFER_SIZE 128
#define BROADCAST_PORT 5051
#define NEUTRAL_CHAR 126 // == '~' 
#define END_OF_MESSAGE_CHAR 64 // == '@'

using namespace std;

enum Operation {
	INVALID_OPERATION,
	SEND_FILENAME,
	SEND_PORT_NUMBER,
	RETRIEVE_SERVER_IP,
	STOP_BROADCAST,
	START_BROADCAST,
	START_BROADCAST_OK,
	START_BROADCAST_NOT_OK,
	DISCONNECT,
	EXIT_APPLICATION,
	NB_OPERATIONS
};


class PacketDisassembler {

public:
	PacketDisassembler();
	PacketDisassembler(string pPacketContent);
	string buildPacket() const;
	void decodePacket(string pPacketContent);
	void setOperation(Operation pOperation);
	void setMessage(string pMessage);
	Operation getOperation() const;
	string getMessage() const;

	bool receivePacket(SOCKET pSocket);
	bool sendPacket(SOCKET pSocket) const;
	bool sendPacket(SOCKET pSocket, Operation pOperation, string pMessage);
private:
	Operation mOperation;
	string mMessage;
	string mInputBuffer;
};

#endif