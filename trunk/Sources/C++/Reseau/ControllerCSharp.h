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

typedef int (__stdcall *MessageReceivedCallBack)( char* pUsername, char* pMessage );
typedef int (__stdcall *EventReceivedCallBack)( int pId, char* pMessage );


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
    virtual void handleEvent(int pEventCode,  va_list pListeElems);
    inline void setMessageReceivedCallBack(MessageReceivedCallBack pVal) { mMessageReceivedCallBack = pVal; }
    inline void setEventReceivedCallBack(EventReceivedCallBack pVal) { mEventReceivedCallback = pVal; }
private:
    MessageReceivedCallBack mMessageReceivedCallBack;
    EventReceivedCallBack   mEventReceivedCallback;

    

};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

