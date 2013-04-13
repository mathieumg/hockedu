//////////////////////////////////////////////////////////////////////////////
/// @file RelayeurMessage.h
/// @author Mathieu Parent
/// @date 2013-01-26
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Singleton.h"
#include <string>
#include "Paquets/Paquet.h"
#include <set>


///////////////////////////////////////////////////////////////////////////
/// @class RelayeurMessage
/// @brief Classe simgleton qui ne sert qu'a relayer les messages (recus et a envoyer) de differentes facons
///
/// @author Mathieu Parent
/// @date 2013-01-26
///////////////////////////////////////////////////////////////////////////
class RelayeurMessage :public Singleton<RelayeurMessage>
{

    SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(RelayeurMessage);
public:

    // Relaie le Paquet a tous les clients connectes
	void relayerPaquetGlobalement(Paquet* pPaquet, const std::set<std::string>* pListeNomsAIgnorer = NULL, ConnectionType pConnectionType = TCP );
    void relayerPaquet(const std::string& pPlayerName, Paquet* pPaquet, ConnectionType pConnectionType = TCP );
    void relayerPaquetGame(int pGameId, Paquet* pPaquet, ConnectionType pConnectionType = TCP);

private:
    /// Constructeur par defaut
    RelayeurMessage();
    /// Destructeur
    ~RelayeurMessage();

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




