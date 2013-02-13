//////////////////////////////////////////////////////////////////////////////
/// @file ControllerServeurJeu.h
/// @author Mathieu Parent
/// @date 2013-02-10
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include "Utilitaire.h"
#include "..\Reseau\ControllerInterface.h"


///////////////////////////////////////////////////////////////////////////
/// @class ControllerServeurJeu
/// @brief Controlleur du serveur de jeu
///
/// @author Mathieu Parent
/// @date 2013-02-10
///////////////////////////////////////////////////////////////////////////
class ControllerServeurJeu : public ControllerInterface
{
public:
    virtual void handleEvent(int pEventCode,  va_list pListeElems);

private:
    
};



void ControllerServeurJeu::handleEvent( int pEventCode, va_list pListeElems )
{
    switch(pEventCode) {
    default:
        std::cout << "EventCode: " << pEventCode << std::endl;
        break;
    };

    

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

