#include "PaquetChatUserStatus.h"


PaquetChatUserStatus::PaquetChatUserStatus(  )
	:PaquetBase("ChatUserStatus")
{
    mConnectionState = CONNECTED;
	mUserName = "";
}


PaquetChatUserStatus::~PaquetChatUserStatus()
{
	
}
