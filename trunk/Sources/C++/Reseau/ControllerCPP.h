//////////////////////////////////////////////////////////////////////////////
/// @file ControllerCPP.h
/// @author Mathieu Parent
/// @date 2013-01-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ControllerInterface.h"
#include "Utilitaire.h"
#include <iostream>
#include "Enum_Declarations.cs"


///////////////////////////////////////////////////////////////////////////
/// @class ControllerCPP
/// @brief Controlleur du client CPP en Command line
///
/// @author Mathieu Parent
/// @date 2013-01-29
///////////////////////////////////////////////////////////////////////////
class ControllerCPP : public ControllerInterface
{
public:
    virtual void handleEvent(EventCodes pEventCode,  va_list pListeElems);

private:
    
};



void ControllerCPP::handleEvent( EventCodes pEventCode, va_list pListeElems )
{
    std::stringstream wSS;
    wSS << ":::EVENT:::";
    int wBaseSize = (int)wSS.str().length();

    switch(pEventCode) {
        case USER_ALREADY_CONNECTED               :
            wSS << "User deja connecte" << std::endl;
            break;
        case USER_DID_NOT_SEND_NAME_ON_CONNECTION :
            wSS << "Le client n'a pas envoye son nom lors de la connection" << std::endl;
            break;
        case USER_CONNECTED                       :
            wSS << "Connection reussie" << std::endl;
            break;
        case USER_DISCONNECTED                    :
            wSS << "Connection interronpue" << std::endl;
            break;
        case RECONNECTION_TIMEOUT                 :
            wSS << "Delais de reconnection depasse" << std::endl;
            break;
        case RECONNECTION_IN_PROGRESS             :
            wSS << "Tentative de reconnection" << std::endl;
            break;
        case WRONG_PASSWORD                       :
            wSS << "Mot de passe invalide" << std::endl;
            break;
        case SERVER_USER_CONNECTED:
            wSS << va_arg(pListeElems,std::string) << " Connected" << std::endl;
            break;
        case SERVER_USER_DISCONNECTED:
            wSS << va_arg(pListeElems,std::string) << " Disconnected" << std::endl;
            break;
        default:
            break;
    };

    std::string wOutput = wSS.str();
    if(wOutput.length() > wBaseSize)
    {
        std::cout << wOutput;
    }

    //     char buffer[1024];
    //     vsprintf_s(buffer,"%s",pListeElems);
    //     std::cout << buffer << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

