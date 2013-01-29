//////////////////////////////////////////////////////////////////////////////
/// @file ControllerCPP.h
/// @author Mathieu Parent
/// @date 2013-01-29
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Reseau\ControllerInterface.h"
#include "Utilitaire.h"
#include <iostream>


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
    virtual void handleEvent(int pEventCode,  va_list pListeElems);

private:
    
};



void ControllerCPP::handleEvent( int pEventCode, va_list pListeElems )
{
    std::cout << ":::EVENT:::";
    switch(pEventCode) {
        case USER_ALREADY_CONNECTED               :
            std::cout << "User deja connecte" << std::endl;
            break;
        case USER_DID_NOT_SEND_NAME_ON_CONNECTION :
            std::cout << "Le client n'a pas envoye son nom lors de la connection" << std::endl;
            break;
        case USER_CONNECTED                       :
            std::cout << "Connection reussie" << std::endl;
            break;
        case USER_DISCONNECTED                    :
            std::cout << "Connection interronpue" << std::endl;
            break;
        case RECONNECTION_TIMEOUT                 :
            std::cout << "Delais de reconnection depasse" << std::endl;
            break;
        case RECONNECTION_IN_PROGRESS             :
            std::cout << "Tentative de reconnection" << std::endl;
            break;
        case WRONG_PASSWORD                       :
            std::cout << "Mot de passe invalide" << std::endl;
            break;
        default:
            break;
    };

    //     char buffer[1024];
    //     vsprintf_s(buffer,"%s",pListeElems);
    //     std::cout << buffer << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

