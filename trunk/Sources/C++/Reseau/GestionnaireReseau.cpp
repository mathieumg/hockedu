//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseau.cpp
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifdef WINDOWS
#include <WinSock2.h>
#endif
#include "NetworkEnums.h"
#include "GestionnaireReseau.h"
#include <strstream>
#include <algorithm>
#include <exception>
#include <map>
#include <iostream>
#include <ctime>
#include <sstream>
#include "ExceptionsReseau/ExceptionReseau.h"
#include "ExceptionsReseau/ExceptionReseauSocketDeconnecte.h"
#include "ExceptionsReseau/ExceptionReseauTimeout.h"
#include "ExceptionsReseau/ExceptionReseauParametreInvalide.h"
#include "ExceptionsReseau/ExceptionReseauGlobale.h"
#include <stdexcept>
#include "CommunicateurReseau.h"
#include "PaquetHandlers/PacketHandler.h"
#include <utility>
#include "Paquets/Paquet.h"
#include "Paquets/PaquetEvent.h"

///Enet not yet used, only for initialisation of winsock so far.
#include "enet/enet.h"

#ifdef WINDOWS
// lien avec la librairie winsock2
#pragma comment( lib, "ws2_32.lib" )
#endif


// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireReseau);


// Port utilise pour la connexion automatique (Multicast en UDP)
int GestionnaireReseau::multicastPort = 1001;

// Port a utiliser pour les communications de base
int GestionnaireReseau::communicationPort = /*5010;//*/25565;

// Port a utiliser pour les communications de base
int GestionnaireReseau::communicationPortMasterServer = /*5011;//*/25566;

// Port a utiliser pour les communications UDP sur le client Lourd
int GestionnaireReseau::communicationUDPPortClientLourd = GestionnaireReseau::communicationPort;

// Port a utiliser pour les communications UDP sur le serveur jeu
int GestionnaireReseau::communicationUDPPortServeurJeu = GestionnaireReseau::communicationPortMasterServer;

std::ofstream errorLogHandle;
std::ofstream PacketSentLogHandle;
std::ofstream PacketRecvLogHandle;
bool bLogCreated = false;
bool bPacketLogCreated = false;
bool bPacketRecvLogCreated = false;

/// ne pas rename, utis/ comme extern ailleur
std::string NETWORK_LOG_FILE_NAME = "GAME_LOG_";
std::string NETWORK_PACKET_SENT_FILE_NAME = "GAME_PACKET_SENT_LOG_";
std::string NETWORK_PACKET_RECV_FILE_NAME = "GAME_PACKET_RECV_LOG_";

// Network Log setup
// Methode pour creer le fichier de log
void logSetup()
{
    if(!bLogCreated)
    {
        FacadePortability::createDirectory("logs");
        time_t wTime = time(0);
        struct tm wTimeNow;
        localtime_s( &wTimeNow, &wTime );
        std::stringstream wFilename;
        wFilename << "logs/";
        wFilename <<NETWORK_LOG_FILE_NAME<< wTimeNow.tm_mon << "_" << wTimeNow.tm_mday << "_" << wTimeNow.tm_hour << "_" << wTimeNow.tm_min << "_" << wTimeNow.tm_sec << ".txt";
        errorLogHandle.open(wFilename.str(), std::fstream::out);
        bLogCreated = true;
    }
}

void PacketSentlogSetup()
{
    if(!bPacketLogCreated)
    {
        FacadePortability::createDirectory("logs");
        time_t wTime = time(0);
        struct tm wTimeNow;
        localtime_s( &wTimeNow, &wTime );
        std::stringstream wFilename;
        wFilename << "logs/"<< NETWORK_PACKET_SENT_FILE_NAME << wTimeNow.tm_mon << "_" << wTimeNow.tm_mday << "_" << wTimeNow.tm_hour << "_" << wTimeNow.tm_min << "_" << wTimeNow.tm_sec;
        PacketSentLogHandle.open(wFilename.str(), std::ios::binary|std::ios::out);
        bPacketLogCreated = true;
    }
}

void PacketRecvlogSetup()
{
    if(!bPacketRecvLogCreated)
    {
        FacadePortability::createDirectory("logs");
        time_t wTime = time(0);
        struct tm wTimeNow;
        localtime_s( &wTimeNow, &wTime );
        std::stringstream wFilename;
        wFilename << "logs/"<< NETWORK_PACKET_RECV_FILE_NAME << wTimeNow.tm_mon << "_" << wTimeNow.tm_mday << "_" << wTimeNow.tm_hour << "_" << wTimeNow.tm_min << "_" << wTimeNow.tm_sec;
        PacketRecvLogHandle.open(wFilename.str(), std::ios::binary|std::ios::out);
        bPacketRecvLogCreated = true;
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireReseau::GestionnaireReseau()
///
/// Constructeur prive
///
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseau::GestionnaireReseau(): 
	mSocketStateCallback(NULL), mControlleur(NULL),
	mServerHost(NULL),mClientHost(NULL)
{
    mCommunicateurReseau = new CommunicateurReseau();
    determineNativeByteOrder();

	FacadePortability::createMutex(mMutexListeSockets);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn GestionnaireReseau::~GestionnaireReseau( )
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
GestionnaireReseau::~GestionnaireReseau()
{
	supprimerPacketHandlersEtUsines();
	if(bLogCreated && !errorLogHandle.fail())
	{
        bLogCreated = false;
		errorLogHandle.close();
	}


    delete mCommunicateurReseau;

// #ifdef WINDOWS
//     WSACleanup();
// #endif
	if(mClientHost)
	{
		enet_host_destroy(mClientHost);
	}
	if(mServerHost)
	{
		enet_host_destroy(mServerHost);
	}
	mServerHost = NULL;
	mClientHost = NULL;
	enet_deinitialize();

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::init() (private)
///
/// Initialisaiton des parametres systeme
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::init()
{
    // Setup de la map pour les types d'exceptions
    mMapExceptions[RAZORGAME_ENOTCONN]         = SOCKET_DECONNECTE;    // Socket not connected
    mMapExceptions[RAZORGAME_ECONNRESET]       = SOCKET_DECONNECTE;    // Connection reset
    mMapExceptions[RAZORGAME_ECONNABORTED]     = SOCKET_DECONNECTE;    // Connection aborted (ex: il y a eu un problemen de protocole)
    mMapExceptions[RAZORGAME_ESHUTDOWN]        = SOCKET_DECONNECTE;    // Le socket a ete shutdown
    mMapExceptions[RAZORGAME_ECONNREFUSED]     = SOCKET_DECONNECTE;    // Connection refusee par le host
    mMapExceptions[RAZORGAME_ENOTSOCK]         = SOCKET_DECONNECTE;    // Handle de socket invalide, throw un socket Deconnecte pour dire qu'il faut reconnecter/reconstruire le socket

    mMapExceptions[RAZORGAME_ETIMEDOUT]        = TIMEOUT;  // Timeout
    mMapExceptions[RAZORGAME_EHOSTDOWN]        = TIMEOUT;  // Host is down
    mMapExceptions[RAZORGAME_EHOSTUNREACH]     = TIMEOUT;  // Host is unreacheable

    mMapExceptions[RAZORGAME_EBADF]                = PARAMETRE_INVALIDE;   // File handle invalide
    mMapExceptions[RAZORGAME_EFAULT]               = PARAMETRE_INVALIDE;   // Bad address, ex: if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr)
    mMapExceptions[RAZORGAME_EINVAL]               = PARAMETRE_INVALIDE;   // Invalid arguments and/or calls (ex: calling accept() on a non-listening socket)
    mMapExceptions[RAZORGAME_EDESTADDRREQ]         = PARAMETRE_INVALIDE;   // Pas de variable d'adresse
    mMapExceptions[RAZORGAME_EPROTOTYPE]           = PARAMETRE_INVALIDE;   // Protocol invalide (ex: set SOCK_STREAM on a UDP socket)
    mMapExceptions[RAZORGAME_ENOPROTOOPT]          = PARAMETRE_INVALIDE;   // Bad protocol option
    mMapExceptions[RAZORGAME_EPROTONOSUPPORT]      = PARAMETRE_INVALIDE;   // Protocole pas supporte
    mMapExceptions[RAZORGAME_ESOCKTNOSUPPORT]      = PARAMETRE_INVALIDE;   // Type de socket pas supporte
    mMapExceptions[RAZORGAME_EOPNOTSUPP]           = PARAMETRE_INVALIDE;   // Operation pas supportee
    mMapExceptions[RAZORGAME_EPFNOSUPPORT]         = PARAMETRE_INVALIDE;   // Famille de protocole pas supportee
    mMapExceptions[RAZORGAME_EAFNOSUPPORT]         = PARAMETRE_INVALIDE;   //Famille d'adresse pas supportee
    mMapExceptions[RAZORGAME_EADDRNOTAVAIL]        = PARAMETRE_INVALIDE;   // Contexte invalide pour cette adresse
    mMapExceptions[RAZORGAME_EACCES]               = PARAMETRE_INVALIDE;   // Access refuse (ex: Utilisation d'un socket pour une adresse de broadcast sans lui avoir donner l'option pour les broadcast)
    mMapExceptions[RAZORGAME_EADDRINUSE]           = PARAMETRE_INVALIDE;   // Adresse deja utilisee (ex: Ne peut pas rebind)
    mMapExceptions[RAZORGAME_ENETUNREACH]          = PARAMETRE_INVALIDE;   // Pas de route connue pour cette adresse
    mMapExceptions[RAZORGAME_ELOOP]                = PARAMETRE_INVALIDE;   // Pas capable de traduire le nom
    mMapExceptions[RAZORGAME_ENAMETOOLONG]         = PARAMETRE_INVALIDE;   // Nom trop long

    mMapExceptions[RAZORGAME_NOT_ENOUGH_MEMORY]   = GLOBALE;  // Memoire insuffisante
    mMapExceptions[RAZORGAME_EMFILE]               = GLOBALE;  // Trop de sockets ouverts
    mMapExceptions[RAZORGAME_ENETDOWN]             = GLOBALE;  // Network is down
    mMapExceptions[RAZORGAME_EUSERS]               = GLOBALE;  // Use quota exceeded
    mMapExceptions[RAZORGAME_EDQUOT]               = GLOBALE;  // Disk quota exceeded

#ifdef WINDOWS
    mMapExceptions[WSA_INVALID_HANDLE]      = PARAMETRE_INVALIDE;   // Utilisation d'un handle invalide
    mMapExceptions[WSA_INVALID_PARAMETER]   = PARAMETRE_INVALIDE;   // Parametre invalide
    mMapExceptions[RAZORGAME_EPROCLIM]             = GLOBALE;  // Trop de process
    mMapExceptions[RAZORGAME_SYSNOTREADY]          = GLOBALE;  // Erreur a l'initialisation de Winsock
    mMapExceptions[RAZORGAME_VERNOTSUPPORTED]      = GLOBALE;  // Version de Winsock pas supportee
    mMapExceptions[RAZORGAME_NOTINITIALISED]       = GLOBALE;  // Winsock pas encore initialise
    mMapExceptions[WSASYSCALLFAILURE]       = GLOBALE;  // System call failure
    mMapExceptions[WSASERVICE_NOT_FOUND]    = GLOBALE;  // Service not found
    mMapExceptions[WSATYPE_NOT_FOUND]       = GLOBALE;  // Class type not found
#endif

    // Ajout des classes PacketHandler et UsinePaquet de base



	// Init Winsock2
	// --> The WSAStartup function initiates use of the Winsock DLL by a process.
// #ifdef WINDOWS
// 	WSADATA wsaData;
// 	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
// #endif
	int iResult = enet_initialize();



#ifdef WINDOWS
    if(iResult != NO_ERROR)
    {
        throw ExceptionReseau("Erreur init de Gestionnaire Reseau", NULL);
    }
#endif
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::getAdresseIPLocaleAssociee( const std::string& pDestinationIP )
///
/// Permet d'obtenir l'adresse IP locale qui correspond a une adresse IP externe.
/// Ex:     Externe: 192.168.9.45
///         -> L'adresse retournee par la fonction sera celle sur l'adapteur reseau qui debute par 192.XXX.XXX.XXX
///
/// @param[in] const std::string& pDestinationIP    : Adresse IP externe a laquelle on veut se connecter
///
/// @return std::string : Adresse IP locale sous forme de string
///
////////////////////////////////////////////////////////////////////////
std::string GestionnaireReseau::getAdresseIPLocaleAssociee( const std::string& pDestinationIP )
{
	std::string wIpLocaleFinale = "";
    std::list<std::string> wIPs;
    FacadePortability::getLocalIPAddresses(wIPs);

	for(auto it = wIPs.begin(); it != wIPs.end(); ++it)
	{
		std::string& wIPAddr = *it;
		if(wIPAddr.substr(0, wIPAddr.find_first_of(".")) == pDestinationIP.substr(0, pDestinationIP.find_first_of(".")))
		{
			// Good network interface
			wIpLocaleFinale = *it;
			break;
		}
	}
	// IP not found in the spcified subnet
	if(wIpLocaleFinale.length() == 0)
	{
		throw ExceptionReseau("Impossible de trouver l'adresse locale associee a " + pDestinationIP);
	}
	// IP found
	else
	{
		return wIpLocaleFinale;
	}

}
std::string GestionnaireReseau::getAdresseIPLocaleAssociee( unsigned int minAdress, unsigned int maxAdress )
{
    std::string wIpLocaleFinale = "";
    std::list<std::string> wIPs;
    FacadePortability::getLocalIPAddresses(wIPs);

    for(auto it = wIPs.begin(); it != wIPs.end(); ++it)
    {
        std::string& wIPAddr = *it;
        unsigned long addInverse = inet_addr(wIPAddr.c_str());
        ///addInverse = ABC.DEF.GHI.JKL
        ///add = JKL.GHI.DEF.ABC

        /// on renverse le tout
        unsigned int add = ((addInverse&255)<<24) | (((addInverse>>8)&255)<<16) | (((addInverse>>16)&255)<<8) | (((addInverse>>24)&8));
        if(add-minAdress <= maxAdress-minAdress)
        {
            // Good network interface
            wIpLocaleFinale = *it;
            break;
        }
    }
    // IP not found in the spcified subnet
    if(wIpLocaleFinale.length() == 0)
    {
        throw ExceptionReseau("Impossible de trouver l'adresse locale associee dans le range");
    }
    // IP found
    else
    {
        return wIpLocaleFinale;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::supprimerPacketHandlersEtUsines()
///
/// (Private)
/// Methode pour nettoyer la liste des PaquetHandler et UsinePaquet (les libere de la memoire egalement)
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::supprimerPacketHandlersEtUsines()
{
    // Vider la liste de Sockets et les detruire
    std::map<std::pair<std::string, ConnectionType>, SPSocket>::iterator iter;
    FacadePortability::takeMutex(mMutexListeSockets);
    for (iter = mListeSockets.begin(); iter != mListeSockets.end(); ++iter)
    {
        (*iter).second->flagToDelete();
        (*iter).second->flagToCancel();
    }
    mListeSockets.clear();
    FacadePortability::releaseMutex(mMutexListeSockets);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::envoyerPaquet( Socket* pSocketAUtiliser, Paquet* pPaquet )
///
/// Methode pour envoyer un paquet sur un socket particulier
///
/// @param[in] Socket* pSocketAUtiliser     : Socket a utiliser pour l'envoie du paquet
/// @param[in] Paquet* pPaquet              : Paquet a envoyer (sera delete par le CommunicateurReseau une fois que le paquet sera envoye)
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::envoyerPaquet( SPSocket pSocketAUtiliser, Paquet* pPaquet )
{
    if(!mCommunicateurReseau->ajouterPaquetEnvoie(pSocketAUtiliser, pPaquet))
    {
        // Le paquet ne peut pas etre ajouter
        pPaquet->removeAssociatedQuery();
        checkf(0); // Si ca arrive, qqch est louche, la file est plein au damn
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::envoyerPaquet( const std::string& pPlayerName, Paquet* pPaquet )
///
/// Methode pour envoyer un paquet par un joueur en particulier
///
/// @param[in] const std::string & pConnectionId    : Id de la connexion
/// @param[in] Paquet* pPaquet                      : Paquet a envoyer (sera delete par le CommunicateurReseau une fois que le paquet sera envoye)
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::envoyerPaquet( const std::string& pConnectionId, Paquet* pPaquet, ConnectionType pConnectionType )
{
    SPSocket pSocket = getSocket(pConnectionId,pConnectionType);
    if(pSocket)
    {
        envoyerPaquet(pSocket,pPaquet);
    }
    else
    {

        pPaquet->removeAssociatedQuery();
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::validerOperation( const std::string& pOperation ) const
///
/// Methode pour valider si un type d'operation est valide (s'il a ete ajoute auparavant)
///
/// @param[in] const std::string& pOperation     : Nom de l'operation a valider
///
/// @return bool    : True si valide
///
////////////////////////////////////////////////////////////////////////
bool GestionnaireReseau::validerOperation( const PacketTypes pOperation ) const
{
    /// TODO:: ajouter une validation supp pour que les controlleur indiquent si on peut ou non envoyer ce type de paquet
	return true;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::demarrerNouvelleConnection(const std::string& pIP, ConnectionType pConnectionType)
///
/// // Methode pour creer une nouvelle connection (un nouveau Socket) et le sauvegarder a la liste des Sockets actifs
///
/// @param[in] const std::string& pConnectionId : Id de la connexion
/// @param[in] const std::string& pIP			: Adresse IP du Socket a creer
/// @param[in] ConnectionType pConnectionType	: Type de connectiona  demarrer (TCP ou UDP)
///
/// @return     : Socket*	: Pointeur vers le socket qui vient d'être créer
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::demarrerNouvelleConnection(const std::string& pConnectionId, const std::string& pIP, ConnectionType pConnectionType)
{
    if(pConnectionId != "")
    {
        SPSocket wNewSocket;
        if(pConnectionId == "MasterServer")
        {
            wNewSocket = SPSocket(new Socket(pIP, GestionnaireReseau::communicationPortMasterServer, pConnectionType));
        }
        else if(pConnectionId == "GameServer" && pConnectionType == UDP)
        {
            wNewSocket = SPSocket(new Socket(pIP, GestionnaireReseau::communicationUDPPortServeurJeu, pConnectionType));
        }
        else if(pConnectionType == UDP)
        {
            wNewSocket = SPSocket(new Socket(pIP, GestionnaireReseau::communicationUDPPortClientLourd, pConnectionType));
        }
        else
        {
            wNewSocket = SPSocket(new Socket(pIP, GestionnaireReseau::communicationPort, pConnectionType));
        }
        saveSocket(pConnectionId, wNewSocket);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::cancelNewConnection( const std::string& pPlayerName )
///
/// Permet d'arreter d'essayer de connecter un socket
///
/// @param[in] const std::string & pPlayerName
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::cancelNewConnection(const std::string& pPlayerName, ConnectionType pConnectionType)
{
    SPSocket wSocket = getSocket(pPlayerName,pConnectionType);
    if(wSocket)
    {
        wSocket->cancelConnection();
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::transmitEvent( int pMessageCode, const std::string& pMessageContent ) const
///
/// Methode pour envoyer des events au Controlleur
///
/// @param[in] int pMessageCode : Code de message a envoyer au controlleur
/// @param[in] ...              : Autres parametres a envoyer au controlleur (varient selon le MessageCode)
///
/// @return     void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::transmitEvent( EventCodes pMessageCode, ... ) const
{
    if(mControlleur)
    {
        va_list args;
        va_start(args, pMessageCode);
        mControlleur->handleEvent(pMessageCode, args);
        va_end(args);
    }
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::saveSocket( const std::string& pNomJoueur, Socket* pSocket )
///
/// Methode pour sauvegarder un Socket. Le socket est donc associe a un identifiant de nom de joueur unique.
/// Le Socket est ensuite envoye dans la liste de Socket a ecouter afin de recevoir des communications.
/// On peut sauvegarder un Socket pour chaque joueur et chaque type de socket (ex: 1 pour UDP et 1 pour TCP)
///
/// @param[in] const std::string& pNomJoueur    : Nom du joueur (doit etre unique)
/// @param[in] Socket* pSocket                  : Socket a sauvegarder
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::saveSocket( const std::string& pNomJoueur, SPSocket pSocket )
{
    if(pNomJoueur == mUsername)
    {
        return;
    }

    FacadePortability::takeMutex(mMutexListeSockets);
    // On compte le nb de connexions qui ne sont pas des connexions de gestion (ex: serveurJeu-serveurMaitre est une connexion de gestion)
    int compte = 0;
    std::set<std::string> wListeUsers;
    for(auto it = mListeSockets.begin(); it != mListeSockets.end(); ++it)
    {
        if((*it).first.first != "GameServer" && (*it).first.first != "MasterServer")
        {
            wListeUsers.insert((*it).first.first);
        }
    }


    if(getController()->getNbConnectionMax() == wListeUsers.size() && wListeUsers.find(pNomJoueur) == wListeUsers.end())
    {
        NETWORK_LOG("Too many player connected dropping : %s", pNomJoueur.c_str());
        FacadePortability::releaseMutex(mMutexListeSockets);
        throw ExceptionReseau("Trop de users connectes");
    }



	// On ecrase le dernier socket qui etait associe a ce nom de joueur et a ce type de socket
	mListeSockets[std::pair<std::string, ConnectionType>(pNomJoueur, pSocket->getConnectionType())] = pSocket;
    pSocket->setId(pNomJoueur);
    if(pSocket->getConnectionType() == TCP)
    {
	    mCommunicateurReseau->ajouterSocketEcoute(pSocket);
    }
    FacadePortability::releaseMutex(mMutexListeSockets);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::getSocket( const std::string& pNomJoueur, ConnectionType pConnectionType )
///
/// Methode pour obtenir un Socket sauvegarde precedemment selon le nom du joueur et le type de Socket.
///
/// @param[in] const std::string& pNomJoueur    : Nom du joueur (doit etre unique)
/// @param[in] ConnectionType pConnectionType   : Type de Socket
///
/// @return Socket* : Pointeur vers le socket associe a ce joueur et a ce type ou NULL si pas dans la liste
///
////////////////////////////////////////////////////////////////////////
SPSocket GestionnaireReseau::getSocket( const std::string& pNomJoueur, ConnectionType pConnectionType )
{
    FacadePortability::takeMutex(mMutexListeSockets);
	ListeSockets::iterator iterateurRecherche = mListeSockets.find(std::pair<std::string, ConnectionType>(pNomJoueur, pConnectionType));
	if(mListeSockets.end() == iterateurRecherche)
	{
        FacadePortability::releaseMutex(mMutexListeSockets);
		return NULL;
	}
	else
	{
        FacadePortability::releaseMutex(mMutexListeSockets);
		return (*iterateurRecherche).second;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::removeSocket( const std::string& pNomJoueur, ConnectionType pConnectionType )
///
/// Methode pour retirer un Socket de la liste de sockets sauvegardes.
/// ATTENTION, INVALIDE LE POINTEUR!!!!!!!!!!!!!!!!!!
/// Les autres threads ne le reutilisent pas, mais il ne faut plus utiliser une ancienne reference
///
/// @param[in] const std::string& pNomJoueur    : Nom du joueur (doit etre unique)
/// @param[in] ConnectionType pConnectionType   : Type de Socket
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::removeSocket( const std::string& pNomJoueur, ConnectionType pConnectionType )
{
    FacadePortability::takeMutex(mMutexListeSockets);
	auto itMap = mListeSockets.find(std::pair<std::string, ConnectionType>(pNomJoueur, pConnectionType));
	if(itMap == mListeSockets.end())
	{
        // Le socket n'est pas dans la liste de sockets sauvegardes
        FacadePortability::releaseMutex(mMutexListeSockets);
		return;
	}
	SPSocket wSocketASupprimer = itMap->second;

    // On attend pour ne pas supprimer le socket pendant une operation critique
    HANDLE_MUTEX wMutex = wSocketASupprimer->getMutexActiviteSocket();
    FacadePortability::takeMutex(wMutex);

    // On doit le retirer de la liste de Socket a ecouter avant de le supprimer
    auto it = mCommunicateurReseau->getFirstSocketEcoute(); // On va egalement chercher le mutex vers la liste de Sockets a ecouter
	for(it; it!=mCommunicateurReseau->getEndSocketEcoute(); ++it)
	{
		if((*it)==wSocketASupprimer)
		{
			// Le supprimer de la liste de Socket a ecouter
			mCommunicateurReseau->supprimerEcouteSocket(it);
			break;
		}
	}
    // On relache le mutex
	mCommunicateurReseau->terminerIterationListeSocketEcoute();

    // Changer le connection state (a pour effet d'envoyer un event de deconnection)
    FacadePortability::releaseMutex(mMutexListeSockets);
    wSocketASupprimer->setConnectionState(NOT_CONNECTED);
    FacadePortability::takeMutex(mMutexListeSockets);

	// Le supprimer de la liste dans cette classe
	mListeSockets.erase(itMap);

	// Liberer la memoire et invalider le pointeur
    wSocketASupprimer->flagToDelete();
	wSocketASupprimer = 0;

    FacadePortability::releaseMutex(wMutex); // On relache le mutex sur le socket
    FacadePortability::releaseMutex(mMutexListeSockets);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::removeSocket( const std::string& pNomJoueur, ConnectionType pConnectionType )
///
/// Methode pour retirer un Socket de la liste de sockets sauvegardes.
/// ATTENTION, INVALIDE LE POINTEUR!!!!!!!!!!!!!!!!!!
/// Les autres threads ne le reutilisent pas, mais il ne faut plus utiliser une ancienne reference
///
/// @param[in] Socket* pSocket  : socket a supprimmer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::removeSocket( SPSocket pSocket )
{
    if(pSocket)
    {
        pSocket->flagToCancel();
        pSocket->flagToDelete();
        // Now remove it from here and delete it
        FacadePortability::takeMutex(mMutexListeSockets);
        for (auto it = mListeSockets.begin(); it!=mListeSockets.end(); ++it)
        {
            if((*it).second == pSocket)
            {
                FacadePortability::releaseMutex(mMutexListeSockets);
                removeSocket(it->first.first,it->first.second); // On laisse l'autre methode faire le retrait
                return;
            }
        }
        FacadePortability::releaseMutex(mMutexListeSockets);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder::determineNativeEndianness()
///
/// Determines the system's native endianness.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::determineNativeByteOrder()
{
	union
	{
		uint32_t value;
		char c[4];
	} test;
	test.value = 1;
	if( test.c[3] && !test.c[2] && !test.c[1] && !test.c[0] )
		NATIVE_BYTE_ORDER = BO_BIG_ENDIAN;
	else if( !test.c[3] && !test.c[2] && !test.c[1] && test.c[0] )
		NATIVE_BYTE_ORDER = BO_LITTLE_ENDIAN;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn PacketBuilder::getNativeEndianness()
///
/// Returns system's native endianness.
///
///
/// @return ByteOrder
///
////////////////////////////////////////////////////////////////////////
ByteOrder& GestionnaireReseau::getNativeByteOrder()
{
	if(NATIVE_BYTE_ORDER == UNKNOWN)
		determineNativeByteOrder();
	return NATIVE_BYTE_ORDER;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::envoyerPaquetBroadcast( Paquet* pPaquet )
///
/// Methode pour envoyer un paquet en broadcast (Utilise le socket preconfigure de GestionnaireReseau)
///
/// @param[in] Paquet* pPaquet    : Paquet a envoyer (Sera libere par CommunicateurReseau apres son envoie)
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::envoyerPaquetBroadcast( Paquet* pPaquet )
{
	// TODO: Implementation et ajouter le Socket de broadcast
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::sendMessageToLog( const std::string& pMessage )
///
/// Methode pour envoyer un message dans le log de reseautique
///
/// @param[in] const std::string& pMessage    : Message a envoyer dans le log
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::sendMessageToLog( const std::string& pMessage )
{
    logSetup();

    // On verifie que le fichier d'output a bien pu etre creer au demarrage
	if(!errorLogHandle.fail())
	{
        time_t wTime = time(0);
        struct tm wTimeNow;
        localtime_s( &wTimeNow, &wTime );
        errorLogHandle << "[" << wTimeNow.tm_hour << ":" << wTimeNow.tm_min << ":" << wTimeNow.tm_sec << "] ";
        errorLogHandle << ExceptionReseau::getLastErrorMessage("", FacadePortability::getLastError());
        errorLogHandle << " " << pMessage << std::endl;
		errorLogHandle.flush(); // Pour etre certain d'avoir tout meme si le programme crash
	}
#if !SHIPPING
    std::cout << pMessage << std::endl;
#endif
}

void GestionnaireReseau::sendMessageToLog(const char* File, int Line, const char* Format/*=""*/, ... )
{
    char ErrorMsg[2048];
    GET_VARARGS( ErrorMsg, ARRAY_COUNT(ErrorMsg), ARRAY_COUNT(ErrorMsg)-1, Format, Format );
    sendMessageToLog(ErrorMsg);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::PacketSendToLog( const std::string& pMessage )
///
/// /*Description*/
///
/// @param[in] const std::string & pMessage
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::PacketSendToLog( const char* pMessage , int length )
{
    PacketSentlogSetup();

    // On verifie que le fichier d'output a bien pu etre creer au demarrage
    if(!PacketSentLogHandle.fail())
    {
        unsigned int t = (unsigned int)time(0); // trunk it
        PacketSentLogHandle.write((const char*)&t,4);
        PacketSentLogHandle.put(1);  //sending packet token
        PacketSentLogHandle.write(pMessage,length);
        PacketSentLogHandle.flush(); // Pour etre certain d'avoir tout meme si le programme crash
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::PacketReceivedToLog( const std::string& pMessage )
///
/// /*Description*/
///
/// @param[in] const std::string & pMessage
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::PacketReceivedToLog( const char* pMessage, int length )
{
    PacketRecvlogSetup();

    // On verifie que le fichier d'output a bien pu etre creer au demarrage
    if(!PacketRecvLogHandle.fail())
    {
        unsigned int t = (unsigned int)time(0); // trunk it
        PacketRecvLogHandle.write((const char*)&t,4);
        PacketRecvLogHandle.put(0); //receiving packet token
        PacketRecvLogHandle.write(pMessage,length);
        PacketRecvLogHandle.flush(); // Pour etre certain d'avoir tout meme si le programme crash
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::throwExceptionReseau(const std::string& pMessagePrefix) const
///
/// Methode pour throw une exception du bon type (s'occupe de builder selon la derniere exception de WSAGetLastError())
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::throwExceptionReseau(const std::string& pMessagePrefix /* = "" */) const
{
    ExceptionTypes wType;
    int wErreur = FacadePortability::getLastError();

    try
    {
        wType = mMapExceptions.at(wErreur);
    }
    catch(std::out_of_range&)
    {
        if(wErreur == 0)
        {
            wType = AUCUNE_ERREUR;
        }
        else
        {
            wType = TYPE_NOT_DEFINED;
        }
    }

    switch(wType)
    {
    case SOCKET_DECONNECTE:
        {
            ExceptionReseauSocketDeconnecte wException("Socket Deconnecte - " + pMessagePrefix);
            wException.throwMe();
            break;
        }
    case TIMEOUT:
        {
            ExceptionReseauTimeout wException("Timeout - " + pMessagePrefix);
            wException.throwMe();
            break;
        }
    case PARAMETRE_INVALIDE:
        {
            ExceptionReseauParametreInvalide wException("Parametre invalide - " + pMessagePrefix);
            wException.throwMe();
            break;
        }
    case GLOBALE:
        {
            ExceptionReseauGlobale wException("EXCEPTION GLOBALE - " + pMessagePrefix);
            wException.throwMe();
            break;
        }
    case TYPE_NOT_DEFINED:
        {
            ExceptionReseau wException("Exception inconnue - " + pMessagePrefix);
            wException.throwMe();
            break;
        }
    case AUCUNE_ERREUR:
        {
            // On ne fait rien si pas d'erreur
            break;
        }
    }



}


////////////////////////////////////////////////////////////////////////
///
/// @fn std::string GestionnaireReseau::getPlayerName( Socket* pSocket )
///
/// Methode pour obtenir le nom d'un joueur selon son socket
///
/// @param[in] Socket* pSocket  : Socketdont on veut trouver le nom de joueur
///
/// @return std::string : Nom du joueur
///
////////////////////////////////////////////////////////////////////////
std::string GestionnaireReseau::getPlayerName()
{
    return mUsername;
}

std::string GestionnaireReseau::getPlayerPassword()
{
    return mPassword;
}





std::string GestionnaireReseau::getSocketIdentifier( SPSocket wSocket )
{
    std::string wBuf;
    FacadePortability::takeMutex(mMutexListeSockets);
    for(auto it = mListeSockets.begin(); it!=mListeSockets.end(); ++it)
    {
        if(it->second == wSocket)
        {
            wBuf = it->first.first;
            break;
        }
    }
    FacadePortability::releaseMutex(mMutexListeSockets);
    return wBuf;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn std::string GestionnaireReseau::getConnectionId( Socket* pSocket )
///
/// Methode pour obtenir le nom d'une connexion selon le socket
///
/// @param[in] Socket* pSocket  : Socketdont on veut trouver l'id
///
/// @return std::string : Id de la connexion
///
////////////////////////////////////////////////////////////////////////
std::string GestionnaireReseau::getConnectionId(SPSocket pSocket)
{
    FacadePortability::takeMutex(mMutexListeSockets);
    std::string name;
    for (auto it = mListeSockets.begin(); it!=mListeSockets.end(); ++it)
    {
        if((*it).second == pSocket)
        {
            name = (*it).first.first;
            break;
        }
    }
    FacadePortability::releaseMutex(mMutexListeSockets);
    return name;
}





////////////////////////////////////////////////////////////////////////
///
/// @fn std::set<std::string> GestionnaireReseau::getConnectionIdList() const
///
/// Methode pour obtenir la liste des noms des joueurs connectes
///
/// @return std::set<std::string>   : Liste des noms
///
////////////////////////////////////////////////////////////////////////
std::set<std::string> GestionnaireReseau::getConnectionIdList(ConnectionType pConnectionType) const
{
    std::set<std::string> wReturnSet;
    for (std::map<std::pair<std::string, ConnectionType>, SPSocket>::const_iterator it = mListeSockets.begin(); it!=mListeSockets.end(); ++it)
    {
        if((*it).first.second == pConnectionType)
        {
            wReturnSet.insert((*it).first.first);
        }
    }
    return wReturnSet;
}

void GestionnaireReseau::socketConnectionStateEvent( SPSocket pSocket, ConnectionStateEvent& pEvent )
{
	if(mSocketStateCallback)
	{
		pEvent.mPlayerName = getConnectionId(pSocket);
		mSocketStateCallback(pEvent);
	}
}



void GestionnaireReseau::disconnectClient( const std::string& pConnectionId, ConnectionType pConnectionType /*= TCP*/ )
{
    SPSocket wSocket = getSocket(pConnectionId, pConnectionType);
    PaquetEvent* wPaquet = (PaquetEvent*) UsinePaquet::creerPaquet(EVENT);
    wPaquet->setEventCode(USER_DISCONNECTED);
    wPaquet->setMessage(GestionnaireReseau::obtenirInstance()->getPlayerName());
    CommunicateurReseau::envoyerPaquetSync(wPaquet, wSocket);
}


// void GestionnaireReseau::disconnectOtherPlayer( const std::string& pPlayerName, ConnectionType pConnectionType /*= TCP*/ )
// {
//     // Si erreur c'est qu'il est deja enleve
//     try
//     {
//         SPSocket wSocket = getSocket(pPlayerName, pConnectionType);
//         removeSocket(wSocket);
//     }
//     catch(ExceptionReseau&) {}
// }





void GestionnaireReseau::getListeAdressesIPLocales(std::list<std::string>& pOut) const
{
    FacadePortability::getLocalIPAddresses(pOut);
}



bool GestionnaireReseau::validerUsername( const std::string& pUsername ) const
{
    // Verifier que le nom n'est pas vide
    if(pUsername.length() == 0)
    {
        return false;
    }

    // Verifier que le nom ne contient pas d'espaces
    if(pUsername.find(" ") != std::string::npos)
    {
        return false;
    }


    return true;
}



void GestionnaireReseau::initClient(const std::string& pUsername/* = ""*/, const std::string& pPassword/* = ""*/)
{
    mUsername = pUsername;
    mPassword = pPassword;
    init();

    // Demarre les threads de reception UDP (pour serveur et client)
    mCommunicateurReseau->demarrerThreadsReceptionUDPClientLourd();
}




void GestionnaireReseau::initServer()
{
    init();

    // Demarrer le thread de connexion TCP
    mCommunicateurReseau->demarrerThreadsConnectionServeur();
}

void GestionnaireReseau::initMasterServer()
{
	initServer();

	ENetAddress address;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = communicationPortMasterServer;
	mServerHost = enet_host_create (& address /* the address to bind the server host to */, 
		32      /* allow up to 32 clients and/or outgoing connections */,
		1      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (mServerHost == NULL)
	{
		/// exception not caught because the application has to crash if this fails
		throw ExceptionReseau("An error occurred while trying to create an ENet server host for the master server.");
	}

}

void GestionnaireReseau::initGameServer()
{
    mUsername = "GameServer";
    mPassword = "HockeduSuperProtectedPassword";
    initServer();
    // Demarre les threads de reception UDP (pour serveur et client)
    mCommunicateurReseau->demarrerThreadsReceptionUDPServeurJeu();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// Setup the server as a host to receive connection from game client
	ENetAddress address;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port. */
	address.port = communicationPort;
	mServerHost = enet_host_create (& address /* the address to bind the server host to */, 
		2      /* allow up to 32 clients and/or outgoing connections */,
		1      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (mServerHost == NULL)
	{
		/// exception not caught because the application has to crash if this fails
		throw ExceptionReseau("An error occurred while trying to create an ENet server host for the game server.");
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// Setup the server as a host to receive connection from game client
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	mClientHost = enet_host_create (NULL /* the address to bind the server host to */, 
		1      /* allow up to 32 clients and/or outgoing connections */,
		1      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (mClientHost == NULL)
	{
		/// exception not caught because the application has to crash if this fails
		throw ExceptionReseau("An error occurred while trying to create an ENet client host for the game server.");
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////

}

void GestionnaireReseau::demarrerConnectionThread( SPSocket pSocket )
{
    mCommunicateurReseau->demarrerConnectionThread(pSocket);
}


void GestionnaireReseau::supprimerEcouteSocket( SPSocket pSocket )
{
    mCommunicateurReseau->supprimerEcouteSocket(pSocket);
}




void GestionnaireReseau::setUser( const std::string& pUsername, const std::string& pPassword )
{
    mUsername = pUsername;
    mPassword = pPassword;
}



void GestionnaireReseau::getEveryoneConnected( std::vector<const std::string*>& pList )
{
    FacadePortability::takeMutex(mMutexListeSockets);

    for(auto it = mListeSockets.begin(); it!=mListeSockets.end(); ++it)
    {
        pList.push_back((&(*it).first.first));
    }

    FacadePortability::releaseMutex(mMutexListeSockets);
}



bool GestionnaireReseau::isMasterServerConnected() const
{
    auto it = mListeSockets.find(std::pair<std::string, ConnectionType>("MasterServer", TCP));
    if(it!=mListeSockets.end())
    {
        return (*it).second->getConnectionState() == CONNECTED;
    }
    return false;
}



bool GestionnaireReseau::isAGameServerConnected() const
{
    auto it = mListeSockets.find(std::pair<std::string, ConnectionType>("GameServer", TCP));
    if(it!=mListeSockets.end())
    {
        return (*it).second->getConnectionState() == CONNECTED;
    }
    return false;
}











///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

