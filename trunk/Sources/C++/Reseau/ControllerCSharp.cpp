#include "ControllerCSharp.h"
#include "Reseau\GestionnaireReseau.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn void ControllerCSharp::handleEvent( int pEventCode, va_list pListeElems )
///
/// Reception d'un événement provenant du réseau
///
/// @param[in] int pEventCode
/// @param[in] va_list pListeElems
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControllerCSharp::handleEvent( int pEventCode, va_list pListeElems )
{
    int wEventHandled = false;
    if(pEventCode >= 0 && pEventCode < NB_EVENT_CODES)
    {
        std::string message;
        switch(EventCodes(pEventCode))
        {
        case CHAT_MESSAGE_RECEIVED:
            if(mMessageReceivedCallBack)
            {
                std::string username = va_arg(pListeElems,std::string);
                std::string message = va_arg(pListeElems,std::string);
                wEventHandled = mMessageReceivedCallBack((char*)username.c_str(),(char*)message.c_str());
            }
            break;
        case SERVER_USER_CONNECTED:
        case SERVER_USER_DISCONNECTED:
            message = va_arg(pListeElems,std::string);
        default:
            if(mEventReceivedCallback)
            {
                wEventHandled = mEventReceivedCallback(pEventCode,"");
            }
        }
    }
    if(!wEventHandled)
    {
        std::cerr << "UnHandled event\n";
    }
}

