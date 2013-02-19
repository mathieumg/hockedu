#include "ControllerCSharp.h"
#include "GestionnaireReseau.h"
#include "PaquetRunnable.h"
#ifdef LINUX
#include <stdarg.h>
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn  ControllerCSharp::ControllerCSharp()
///
/// Constructeur
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
ControllerCSharp::ControllerCSharp():mEventReceivedCallback(NULL),mMessageReceivedCallBack(NULL)
{
    // Ajouter tous les Runnables dependant de ce qui est handled selon le type de controlleur
    mPaquetRunnables[EVENT] = PaquetRunnable::RunnableEvent;
    mPaquetRunnables[CONN_AUTOMATIQUE] = PaquetRunnable::RunnableConnAutomatiqueClient;
    mPaquetRunnables[USER_STATUS] = PaquetRunnable::RunnableUserStatusClient;
    mPaquetRunnables[CHAT_MESSAGE] = PaquetRunnable::RunnableChatMessageClient;
    mPaquetRunnables[TEST] = PaquetRunnable::RunnableTest;

}


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
                std::string username = va_arg(pListeElems,char*);
                std::string message = va_arg(pListeElems,char*);
                wEventHandled = mMessageReceivedCallBack((char*)username.c_str(),(char*)message.c_str());
            }
            break;
        case SERVER_USER_CONNECTED:
        case SERVER_USER_DISCONNECTED:
            message = va_arg(pListeElems,char*);
        default:
            if(mEventReceivedCallback)
            {
                wEventHandled = mEventReceivedCallback(pEventCode,(char*)message.c_str());
            }
        }
    }
    if(!wEventHandled)
    {
        std::cerr << "UnHandled event\n";
    }
}
