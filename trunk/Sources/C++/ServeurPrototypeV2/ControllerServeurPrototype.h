//////////////////////////////////////////////////////////////////////////////
/// @file ControllerServeurPrototype.h
/// @author Mathieu Parent
/// @date 2013-01-30
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Reseau\ControllerInterface.h"
#include "Utilitaire.h"
#include <iostream>
#include "Reseau\GestionnaireReseau.h"
#include "Reseau\ExceptionsReseau\ExceptionReseauSocketDeconnecte.h"


///////////////////////////////////////////////////////////////////////////
/// @class ControllerServeurPrototype
/// @brief Controlleur du serveur du prototype
///
/// @author Mathieu Parent
/// @date 2013-01-30
///////////////////////////////////////////////////////////////////////////
class ControllerServeurPrototype : public ControllerInterface
{
public:
    virtual void handleEvent(int pEventCode,  va_list pListeElems);

private:
    
};



void ControllerServeurPrototype::handleEvent( int pEventCode, va_list pListeElems )
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
    default:
        break;
    };

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

