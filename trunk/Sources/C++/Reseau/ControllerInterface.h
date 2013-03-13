//////////////////////////////////////////////////////////////////////////////
/// @file ControllerInterface.h
/// @author Mathieu Parent
/// @date 2013-01-29
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#ifdef WINDOWS
#include <hash_map>
#elif defined(LINUX)
#include <ext/hash_map>
namespace std {using namespace __gnu_cxx; }
#endif
#include "Socket.h"
#include "../../C#/UIHeavyClient/UIHeavyClient/Enum_Declarations.cs"

typedef int PaquetTypes;
typedef int (*PaquetRunnableFunc) (class Paquet*);

class Partie;

///////////////////////////////////////////////////////////////////////////
/// @class ControllerInterface
/// @brief Interface a implementer pour un controlleur a associer au modele pour les callbacks
///
/// @author Mathieu Parent
/// @date 2013-01-29
///////////////////////////////////////////////////////////////////////////
class ControllerInterface
{
public:
    PaquetRunnableFunc getRunnable(PaquetTypes pType) const;


    virtual void handleEvent(EventCodes pEventCode,  va_list pListeElems) = 0;
    virtual void handleDisconnectDetection(SPSocket pSocket);

    virtual int getNbConnectionMax() const {return 1000;}

    // Pas d'authentification par defaut, retourne toujours false
    virtual bool requireAuthentification() const {return false;}

    // Pas d'authentification par defaut, retourne toujours true
    virtual bool authenticate(const std::string& pUsername, const std::string& pPassword) {return true;}

    // Retourne la liste des joueurs connectes qui sont assicies a cette partie
    virtual void getPlayersInGame(int pGameId, std::vector<const std::string*>& pPlayerList);

    virtual Partie* getGame(int pGameId);

protected:
    std::hash_map<PaquetTypes, PaquetRunnableFunc> mPaquetRunnables;
};




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

