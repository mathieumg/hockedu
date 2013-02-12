#include "ControllerCSharp.h"
#include "GestionnaireReseau.h"
#include "PaquetRunnable.h"


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
    mPaquetRunnables[0] = PaquetRunnable::RunnableEvent;



    int RunnableEvent(Paquet* pPaquet);

    int RunnableLoginInfoServer(Paquet* pPaquet);

    int RunnableConnAutomatiqueClient(Paquet* pPaquet);
    int RunnableConnAutomatiqueServer(Paquet* pPaquet);

    int RunnableUserStatusClient(Paquet* pPaquet);
    int RunnableUserStatusServer(Paquet* pPaquet);

    int RunnableChatMessageServer(Paquet* pPaquet);
    int RunnableChatMessageClient(Paquet* pPaquet);

    int RunnableTest(Paquet* pPaquet);

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
                wEventHandled = mEventReceivedCallback(pEventCode,(char*)message.c_str());
            }
        }
    }
    if(!wEventHandled)
    {
        std::cerr << "UnHandled event\n";
    }
}
