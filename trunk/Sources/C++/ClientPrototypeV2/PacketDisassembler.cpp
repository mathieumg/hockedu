
#include "Packetdisassembler.h"
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

#define OPERATION_HEADER_SIZE 2
#define NB_RETRIES 20


PacketDisassembler::PacketDisassembler() {
	mMessage = "";
	mInputBuffer = "";
	mOperation = (Operation) NULL;
}

PacketDisassembler::PacketDisassembler(string pPacketContent) {
	mMessage = "";
	mInputBuffer = "";
	mOperation = (Operation) NULL;
	decodePacket(pPacketContent);
}

void PacketDisassembler::decodePacket(string pPacketContent) {
	// Separates the Packet into it's type and content
	// The first byte is the operation, the rest is the message
	
	mOperation = (Operation) atoi(pPacketContent.substr(0, OPERATION_HEADER_SIZE).c_str());
	if(mOperation >= NB_OPERATIONS || mOperation == INVALID_OPERATION) {
		cout << "Invalid Operation" << endl;
	}
	if(pPacketContent.length() > OPERATION_HEADER_SIZE) {
		mMessage = pPacketContent.substr(OPERATION_HEADER_SIZE, pPacketContent.find_first_of(NEUTRAL_CHAR)-OPERATION_HEADER_SIZE);
	}
	else {
		mMessage = "";
	}
	
}


string PacketDisassembler::buildPacket() const {
	if(mOperation == NULL) {
		std::cout << "NULL variables" << std::endl;
	}
	std::ostringstream out;
	out << mOperation;
	while(out.str().length() < OPERATION_HEADER_SIZE) {
		out << " ";
	}
	out << mMessage;
	while(out.str().length() < NETWORK_BUFFER_SIZE-1) {
		out << char(NEUTRAL_CHAR);
	}
	out << char(END_OF_MESSAGE_CHAR);
	return out.str();
}

void PacketDisassembler::setMessage(string pMessage) {
	mMessage = pMessage;
}

void PacketDisassembler::setOperation(Operation pOperation) {
	mOperation = pOperation;
}

Operation PacketDisassembler::getOperation() const {
	return mOperation;
}

string PacketDisassembler::getMessage() const {
	return mMessage;
}

bool PacketDisassembler::receivePacket(SOCKET pSocket) {
	// We have to handle the case where TCP splits up the sent packet
	char readBuffer[NETWORK_BUFFER_SIZE];

	while(mInputBuffer.length()<NETWORK_BUFFER_SIZE) {
		int wSizeReceived = recv(pSocket, readBuffer, NETWORK_BUFFER_SIZE, 0);

		if(wSizeReceived == SOCKET_ERROR) {
			return false;
		}
		
		for(int i=0; i<wSizeReceived; ++i) {
			if(readBuffer[i] == -52 || readBuffer[i] == END_OF_MESSAGE_CHAR) {
				readBuffer[i] = NEUTRAL_CHAR;
			}
		}


		mInputBuffer += readBuffer;
		mInputBuffer.resize(mInputBuffer.length()-strlen(readBuffer)+wSizeReceived, '~');

		
		
	}
	

	decodePacket(mInputBuffer.substr(0, NETWORK_BUFFER_SIZE));
	mInputBuffer = mInputBuffer.substr(NETWORK_BUFFER_SIZE, mInputBuffer.length());
	return true;

}

bool PacketDisassembler::sendPacket(SOCKET pSocket) const {
	// We need to force the send more than the receive because the receive can just ignore the packet
	int wNbTries = 0,wReturnValue = 0;
	bool wSent = false;
	string wBuiltPacket = buildPacket();
	wBuiltPacket.resize(NETWORK_BUFFER_SIZE, NEUTRAL_CHAR);
	while(!wSent && wNbTries<NB_RETRIES) {
		// Try sending it
		wReturnValue = send(pSocket, wBuiltPacket.c_str(), NETWORK_BUFFER_SIZE, 0);
		if(wReturnValue == NETWORK_BUFFER_SIZE) {
			wSent = true;
		}
		else if(wReturnValue == SOCKET_ERROR) {
			return false;
		}
		else {
			wNbTries++;
			cout << "Trying..." << wNbTries << endl;
			Sleep(10); // Sleep to try later
		}
	}
	return wSent;
}


bool PacketDisassembler::sendPacket(SOCKET pSocket, Operation pOperation, string pMessage) {
	setOperation(pOperation);
	setMessage(pMessage);
	return sendPacket(pSocket);
}