//////////////////////////////////////////////////////////////////////////////
/// @file ControllerServeurMaitre.h
/// @author Mathieu Parent
/// @date 2013-02-09
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ControllerInterface.h"
#include "Utilitaire.h"
#include <iostream>
#include "GestionnaireReseau.h"
#include "ExceptionsReseau\ExceptionReseauSocketDeconnecte.h"


///////////////////////////////////////////////////////////////////////////
/// @class ControllerServeurMaitre
/// @brief Controlleur du serveur maitre
///
/// @author Mathieu Parent
/// @date 2013-02-09
///////////////////////////////////////////////////////////////////////////
class ControllerServeurMaitre : public ControllerInterface
{
public:
    virtual void handleEvent(int pEventCode,  va_list pListeElems);

private:
    
};



void ControllerServeurMaitre::handleEvent( int pEventCode, va_list pListeElems )
{
    switch(pEventCode) {
    case USER_DISCONNECTED:
        {
            std::string wPlayerName = va_arg(pListeElems,std::string);
            if(wPlayerName.length() != 0 )
            {
                throw ExceptionReseauSocketDeconnecte("Message de deconnection recu par le client " + wPlayerName);
            }
            break;
        }
    case SERVER_USER_CONNECTING:
        {
            std::cout << "Event: Tentative de connexion de " << va_arg(pListeElems,std::string) << std::endl;
            break;
        }
    default:
        std::cout << "EventCode: " << pEventCode << std::endl;
        break;
    };

    

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

