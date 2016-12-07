//////////////////////////////////////////////////////////////////////////////
/// @file ControllerCSharp.h
/// @author Michael Ferris
/// @date 2013-01-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Utilitaire.h"
#include "ControllerInterface.h"

typedef int (__stdcall *MessageReceivedCallBack)( char* pUsername, char* pMessage, char* pGroupName );
typedef int (__stdcall *EventReceivedCallBack)( EventCodes pId, char* pMessage );
typedef int (*EventTypeHandler)(class ControllerCSharp*, EventCodes, va_list);


///////////////////////////////////////////////////////////////////////////
/// @class ControllerCSharp
/// @brief Controlleur du client CPP en Command line
///
/// @author Michael Ferris
/// @date 2013-01-29
///////////////////////////////////////////////////////////////////////////
class ControllerCSharp : public ControllerInterface
{
public:
    ControllerCSharp();
    virtual void handleEvent(EventCodes pEventCode,  va_list pListeElems);
    inline void setMessageReceivedCallBack(MessageReceivedCallBack pVal) { mMessageReceivedCallBack = pVal; }
    inline void setEventReceivedCallBack(EventReceivedCallBack pVal) { mEventReceivedCallback = pVal; }

    virtual void handleDisconnectDetection( SPSocket pSocket );

private:
    MessageReceivedCallBack mMessageReceivedCallBack;
    EventReceivedCallBack   mEventReceivedCallback;

    static int HandleEvent(ControllerCSharp*, EventCodes,  va_list pListeElems);
    static int HandleMessage(ControllerCSharp*, EventCodes,  va_list pListeElems);

    virtual void getPlayersInGame( int pGameId, std::vector<std::string>& pPlayerList );

    std::unordered_map<EventCodes, EventTypeHandler> mEventTypeHandlers;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

