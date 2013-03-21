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
#include "../Reseau/ControllerInterface.h"


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
    ControllerServeurJeu();
    virtual void handleEvent(EventCodes pEventCode,  va_list pListeElems);

    virtual void handleDisconnectDetection( SPSocket pSocket );

    virtual int getNbConnectionMax() const {return 4;} // Max de 2 personnes connectees pour le serveur jeu (UDP+TCP)

    virtual void getPlayersInGame( int pGameId, std::vector<const std::string*>& pPlayerList );

    virtual Partie* getGame( int pGameId );

private:
    unsigned int mServerId;
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

