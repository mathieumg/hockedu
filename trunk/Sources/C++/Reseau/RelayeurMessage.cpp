///////////////////////////////////////////////////////////////////////////////
/// @file RelayeurMessage.cpp
/// @author Mathieu Parent
/// @date 2013-01-26
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "RelayeurMessage.h"


// Initialisations automatiques
SINGLETON_DECLARATION_CPP(RelayeurMessage);

////////////////////////////////////////////////////////////////////////
///
/// @fn  RelayeurMessage::RelayeurMessage(  )
///
/// Constructeur
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
RelayeurMessage::RelayeurMessage()
{
    

}


////////////////////////////////////////////////////////////////////////
///
/// @fn  RelayeurMessage::~RelayeurMessage(  )
///
/// Destructeur 
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
RelayeurMessage::~RelayeurMessage()
{
    
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void RelayeurMessage::relayerPaquetGlobalement(Paquet* pPaquet, ConnectionType pConnectionType /* = TCP */)
///
/// Methode pour relayer un paquet a tout le monde qui est connecte
///
/// @param[in] Paquet* pPaquet                  : Paquet a relayer
/// @param[in] ConnectionType pConnectionType   : Type de connection a utiliser
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RelayeurMessage::relayerPaquetGlobalement( Paquet* pPaquet, const std::set<std::string>* pListeNomsAIgnorer /*= NULL*/, ConnectionType pConnectionType /*= TCP */ )
{
    // On trouve la liste des personnes connectees et on envoie le Paquet a tous
    std::set<std::string> wListe = GestionnaireReseau::obtenirInstance()->getConnectionIdList(pConnectionType);
    pPaquet->setNbAssociatedQueries((int) (wListe.size()));
    for(std::set<std::string>::const_iterator it = wListe.begin(); it!=wListe.end(); ++it)
    {
        bool wNomFound = false;
        if(pListeNomsAIgnorer)
        {
            for(auto itListe = pListeNomsAIgnorer->begin(); itListe != pListeNomsAIgnorer->end(); itListe++)
            {
                size_t wIndexFound = (*it).find(*itListe);
                if(wIndexFound == 0) // Si contient le nom a ignorer au debut de son nom
                {
                    wNomFound = true;
                    break;
                }
            }
        }
        if(!wNomFound)
        {
            relayerPaquet((*it), pPaquet, TCP);
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void RelayeurMessage::relayerPaquet( const std::string& pPlayerName, Paquet* pPaquet, ConnectionType pConnectionType /* = TCP */ )
///
/// Methode pour relayer un Paquet a une autre personne en particulier
///
/// @param[in] const std::string& pPlayerName   : Nom du joueur a qui on veut envoyer le paquet
/// @param[in] Paquet* pPaquet                  : Paquet a relayer
/// @param[in] ConnectionType pConnectionType   : Type de connection
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RelayeurMessage::relayerPaquet( const std::string& pPlayerName, Paquet* pPaquet, ConnectionType pConnectionType /* = TCP */ )
{
  
    // On envoie le paquet a une personne en particulier
    GestionnaireReseau* wGestionnaire = GestionnaireReseau::obtenirInstance();
    try
    {
        wGestionnaire->envoyerPaquet(wGestionnaire->getSocket(pPlayerName, pConnectionType), pPaquet);
    }
    catch(ExceptionReseau&)
    {
        // Si on ne reussit pas a relayer le message, on reduit le nb de Queries associees a
        // ce paquet pour eviter les memoey leaks
        pPaquet->removeAssociatedQuery();
    }
    
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void RelayeurMessage::relayerPaquetGame( int pGameId, Paquet* pPaquet, ConnectionType /*= TCP*/ )
///
/// Methode pour relayer un Paquet a tous les clients connectes a une certaine partie
///
/// @param[in] int                              : Id de la partie ou le paquet doit etre relaye
/// @param[in] Paquet* pPaquet                  : Paquet a relayer
/// @param[in] ConnectionType pConnectionType   : Type de connection
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RelayeurMessage::relayerPaquetGame( int pGameId, Paquet* pPaquet, ConnectionType pConnectionType /*= TCP*/ )
{
    // Va chercher la liste des joueurs
    std::vector<std::string> wPlayersList;
    GestionnaireReseau::obtenirInstance()->getController()->getPlayersInGame(pGameId, wPlayersList);
    
    
    pPaquet->setNbAssociatedQueries((int)wPlayersList.size());

    for(auto it = wPlayersList.begin(); it != wPlayersList.end(); ++it)
    {
        relayerPaquet((*it), pPaquet, pConnectionType);
    }
    

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////