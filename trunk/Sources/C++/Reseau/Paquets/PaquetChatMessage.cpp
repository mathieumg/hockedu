#include "PaquetChatMessage.h"


PaquetChatMessage::PaquetChatMessage(  )
	:PaquetBase("ChatMessage")
{
    mMessage = "";
    mOrigin = "";
    mIsTargetGroup = true;
    mTimestamp = 0;
    mGroupName = "";
}




PaquetChatMessage::~PaquetChatMessage()
{
	
}
