#include "PaquetUserStatus.h"


PaquetUserStatus::PaquetUserStatus(  )
	:PaquetBase("UserStatus")
{
    mConnectionState = CONNECTED;
	mUserName = "";
}


PaquetUserStatus::~PaquetUserStatus()
{
	
}
