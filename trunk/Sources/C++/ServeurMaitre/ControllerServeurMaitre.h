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
#include "Utilitaire.h"
#include <iostream>
#include "../Reseau/ControllerInterface.h"
#include "../Reseau/Socket.h"
#include <hash_map>
#include <map>

class PartieServeurs;

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
    ControllerServeurMaitre();
    virtual void handleEvent(EventCodes pEventCode,  va_list pListeElems);

    virtual void handleDisconnectDetection(SPSocket pSocket);

    virtual bool requireAuthentification() const {return true;}

    virtual std::string authenticate( const std::string& pUsername, const std::string& pPassword );

    virtual void getPlayersInGame( int pGameId, std::vector<std::string>& pPlayerList );
};




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

