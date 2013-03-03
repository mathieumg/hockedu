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
        if(!pListeNomsAIgnorer || pListeNomsAIgnorer->find(*it) == pListeNomsAIgnorer->end())
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


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////