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
#include "GestionnaireReseau.h"
#include <strstream>
#include <algorithm>
#include <exception>
#include <map>
#include "UsinePaquets/UsinePaquet.h"
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
#include "UsinePaquets/UsinePaquetConnAutomatique.h"
#include "UsinePaquets/UsinePaquetTest.h"
#include <utility>
#include "UsinePaquets/UsinePaquetEvent.h"

#ifdef WINDOWS
// lien avec la librairie winsock2
#pragma comment( lib, "ws2_32.lib" )
#endif

ByteOrder GestionnaireReseau::NATIVE_BYTE_ORDER = UNKNOWN;

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireReseau);


// Port utilise pour la connexion automatique (Multicast en UDP)
int GestionnaireReseau::multicastPort = 1001;

// Port a utiliser pour les communications de base
int GestionnaireReseau::communicationPort = 25565;

// Port a utiliser pour les communications de base
int GestionnaireReseau::communicationPortMasterServer = 25566;

// Port a utiliser pour les communications de base
int GestionnaireReseau::communicationUDPPort = 5011;

// Port a utiliser pour les communications de base
int GestionnaireReseau::connectionUDPPort = 5012;

std::ofstream errorLogHandle;
bool bLogCreated = false;


// Network Log setup
// Methode pour creer le fichier de log
void logSetup()
{
    if(!bLogCreated)
    {
        time_t wTime = time(0);
        struct tm wTimeNow;
#ifdef WINDOWS
        localtime_s( &wTimeNow, &wTime );
#elif defined(LINUX)
        time(&wTime);
        wTimeNow = *localtime(&wTime);
#endif
        std::stringstream wFilename;
        wFilename << "NETWORK_LOG_" << wTimeNow.tm_mon << "_" << wTimeNow.tm_mday << "_" << wTimeNow.tm_hour << "_" << wTimeNow.tm_min << "_" << wTimeNow.tm_sec << ".txt";

        errorLogHandle.open(wFilename.str(), std::fstream::out);
        bLogCreated = true;
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
GestionnaireReseau::GestionnaireReseau(): mSocketStateCallback(NULL), mControlleur(NULL)
{
    getNativeByteOrder();

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
#ifdef WINDOWS
    WSACleanup();
#endif
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
#ifdef WINDOWS
    mMapExceptions[WSAENOTCONN]         = SOCKET_DECONNECTE;    // Socket not connected
    mMapExceptions[WSAECONNRESET]       = SOCKET_DECONNECTE;    // Connection reset
    mMapExceptions[WSAECONNABORTED]     = SOCKET_DECONNECTE;    // Connection aborted (ex: il y a eu un problemen de protocole)
    mMapExceptions[WSAESHUTDOWN]        = SOCKET_DECONNECTE;    // Le socket a ete shutdown
    mMapExceptions[WSAECONNREFUSED]     = SOCKET_DECONNECTE;    // Connection refusee par le host
    mMapExceptions[WSAENOTSOCK]         = SOCKET_DECONNECTE;    // Handle de socket invalide, throw un socket Deconnecte pour dire qu'il faut reconnecter/reconstruire le socket

    mMapExceptions[WSAETIMEDOUT]        = TIMEOUT;  // Timeout
    mMapExceptions[WSAEHOSTDOWN]        = TIMEOUT;  // Host is down
    mMapExceptions[WSAEHOSTUNREACH]     = TIMEOUT;  // Host is unreacheable

    mMapExceptions[WSA_INVALID_HANDLE]      = PARAMETRE_INVALIDE;   // Utilisation d'un handle invalide
    mMapExceptions[WSA_INVALID_PARAMETER]   = PARAMETRE_INVALIDE;   // Parametre invalide
    mMapExceptions[WSAEBADF]                = PARAMETRE_INVALIDE;   // File handle invalide
    mMapExceptions[WSAEFAULT]               = PARAMETRE_INVALIDE;   // Bad address, ex: if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr)
    mMapExceptions[WSAEINVAL]               = PARAMETRE_INVALIDE;   // Invalid arguments and/or calls (ex: calling accept() on a non-listening socket)
    mMapExceptions[WSAEDESTADDRREQ]         = PARAMETRE_INVALIDE;   // Pas de variable d'adresse
    mMapExceptions[WSAEPROTOTYPE]           = PARAMETRE_INVALIDE;   // Protocol invalide (ex: set SOCK_STREAM on a UDP socket)
    mMapExceptions[WSAENOPROTOOPT]          = PARAMETRE_INVALIDE;   // Bad protocol option
    mMapExceptions[WSAEPROTONOSUPPORT]      = PARAMETRE_INVALIDE;   // Protocole pas supporte
    mMapExceptions[WSAESOCKTNOSUPPORT]      = PARAMETRE_INVALIDE;   // Type de socket pas supporte
    mMapExceptions[WSAEOPNOTSUPP]           = PARAMETRE_INVALIDE;   // Operation pas supportee
    mMapExceptions[WSAEPFNOSUPPORT]         = PARAMETRE_INVALIDE;   // Famille de protocole pas supportee
    mMapExceptions[WSAEAFNOSUPPORT]         = PARAMETRE_INVALIDE;   //Famille d'adresse pas supportee
    mMapExceptions[WSAEADDRNOTAVAIL]        = PARAMETRE_INVALIDE;   // Contexte invalide pour cette adresse
    mMapExceptions[WSAEACCES]               = PARAMETRE_INVALIDE;   // Access refuse (ex: Utilisation d'un socket pour une adresse de broadcast sans lui avoir donner l'option pour les broadcast)
    mMapExceptions[WSAEADDRINUSE]           = PARAMETRE_INVALIDE;   // Adresse deja utilisee (ex: Ne peut pas rebind)
    mMapExceptions[WSAENETUNREACH]          = PARAMETRE_INVALIDE;   // Pas de route connue pour cette adresse
    mMapExceptions[WSAELOOP]                = PARAMETRE_INVALIDE;   // Pas capable de traduire le nom
    mMapExceptions[WSAENAMETOOLONG]         = PARAMETRE_INVALIDE;   // Nom trop long

    mMapExceptions[WSA_NOT_ENOUGH_MEMORY]   = GLOBALE;  // Memoire insuffisante
    mMapExceptions[WSAEMFILE]               = GLOBALE;  // Trop de sockets ouverts
    mMapExceptions[WSAENETDOWN]             = GLOBALE;  // Network is down
    mMapExceptions[WSAEPROCLIM]             = GLOBALE;  // Trop de process
    mMapExceptions[WSAEUSERS]               = GLOBALE;  // Use quota exceeded
    mMapExceptions[WSAEDQUOT]               = GLOBALE;  // Disk quota exceeded
    mMapExceptions[WSASYSNOTREADY]          = GLOBALE;  // Erreur a l'initialisation de Winsock
    mMapExceptions[WSAVERNOTSUPPORTED]      = GLOBALE;  // Version de Winsock pas supportee
    mMapExceptions[WSANOTINITIALISED]       = GLOBALE;  // Winsock pas encore initialise
    mMapExceptions[WSASYSCALLFAILURE]       = GLOBALE;  // System call failure
    mMapExceptions[WSASERVICE_NOT_FOUND]    = GLOBALE;  // Service not found
    mMapExceptions[WSATYPE_NOT_FOUND]       = GLOBALE;  // Class type not found
#endif

    // Ajout des classes PacketHandler et UsinePaquet de base
    ajouterOperationReseau(CONN_AUTOMATIQUE, new PacketHandlerConnAutomatique(), new UsinePaquetConnAutomatique());
    ajouterOperationReseau(TEST, new PacketHandlerTest(), new UsinePaquetTest());
    ajouterOperationReseau(EVENT, new PacketHandlerEvent(), new UsinePaquetEvent());

	// Init Winsock2
	// --> The WSAStartup function initiates use of the Winsock DLL by a process.
#ifdef WINDOWS
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
#endif

    // Demarre les threads de reception UDP (pour serveur et client)
    mCommunicateurReseau.demarrerThreadsReceptionUDP();


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


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::ajouterOperationReseau(const std::string& pNomOperation, PacketHandler* pPacketHandler, UsinePaquet* pUsine)
///
/// (Private)
/// Methode pour ajouter une operation reseau (ex: deplacerNoeud). Le handler doit etre fourni ainsi que l'usine pour creer les noeuds
///
/// @param[in] const std::string& pNomOperation     : Nom de l'operation a ajouter
/// @param[in] PacketHandler* pPacketHandler        : Pointeur vers un PaquetHandler a associer a ce type de Paquet
/// @param[in] UsinePaquet* pUsine                  : Pointeur vers une UsinePaquet a associer a ce type de Paquet
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::ajouterOperationReseau(const PacketTypes pNomOperation, PacketHandler* pPacketHandler, UsinePaquet* pUsine)
{
	// Ajouter le handler de paquet
	mListeHandlers[pNomOperation] = pPacketHandler;

	// Ajouter le Factory (Usine) pour le type de paquet
	mListeUsines[pNomOperation] = pUsine;

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
	// Vide liste Handlers
	auto itHandlers = mListeHandlers.begin();
	for(itHandlers; itHandlers != this->mListeHandlers.end(); ++itHandlers)
	{
		delete (*itHandlers).second;
	}
	mListeHandlers.clear();

	// Vide liste Usines
	auto itUsines = mListeUsines.begin();
	for(itUsines; itUsines!=this->mListeUsines.end(); ++itUsines)
	{
		delete (*itUsines).second;
	}
	mListeUsines.clear();

    // Vider la liste de Sockets et les detruire
    std::map<std::pair<std::string, ConnectionType>, SPSocket>::iterator iter;
    FacadePortability::takeMutex(mMutexListeSockets);
//     for (iter = mListeSockets.begin(); iter != mListeSockets.end(); ++iter)
//     {
//         delete (*iter).second;
//     }
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
    if(this->validerOperation(pPaquet->getOperation()))
    {
        if(!mCommunicateurReseau.ajouterPaquetEnvoie(pSocketAUtiliser, pPaquet))
        {
            // Le paquet ne peut pas etre ajouter
            pPaquet->removeAssociatedQuery();
            throw ExceptionReseau("Buffer d'envoie plein");
        }
    }
    else
    {
        throw ExceptionReseau("Operation invalide a l'envoie d'un Paquet dans GestionnaireReseau.");
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
	return mListeHandlers.find(pOperation)!=mListeHandlers.end();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::getPacketHandler( const std::string& pOperation )
///
/// Methode pour obtenir le PaquetHandler associe a un type d'operation qui a ete ajoute auparavant
///
/// @param[in] const std::string& pOperation     : Nom de l'operation dont on veut le PaquetHandler
///
/// @return     : Pointeur sur le PaquetHandler (Ne pas appeler delete sur ce pointeur. GestionnaireReseau va s'en occuper.)
///
////////////////////////////////////////////////////////////////////////
PacketHandler* GestionnaireReseau::getPacketHandler( const PacketTypes pOperation )
{
    auto it = mListeHandlers.find(pOperation);
    if(it == mListeHandlers.end())
    {
        throw ExceptionReseau("Operation invalide lors de l'appel a getPacketHandler");
    }
    return (*it).second;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::creerPaquet( const std::string& pOperation )
///
/// Methode pour obtenir un paquet du bon type de paquet selon le nom d'operation (Appelle la factory)
///
/// @param[in] const std::string& pOperation     : Nom de l'operation dont on veut creer un paquet
///
/// @return     : Pointeur sur un Paquet du bon type
///
////////////////////////////////////////////////////////////////////////
Paquet* GestionnaireReseau::creerPaquet( const PacketTypes pOperation )
{
    auto wIterateur = mListeUsines.find(pOperation);

    // Valider l'operation
    if (wIterateur == mListeUsines.end())
    {
        throw ExceptionReseau("Operation invalide a la creation d'un paquet. L'usine n'existe pas.");
    }

	// Appeler la bonne usine et retourner le resultat
	return (*wIterateur).second->creerPaquet();
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
    for(auto it = mListeSockets.begin(); it != mListeSockets.end(); ++it)
    {
        if((*it).first.first != "GameServer" && (*it).first.first != "MasterServer")
        {
            ++compte;
        }
    }


    if(getController()->getNbConnectionMax() == compte)
    {
        FacadePortability::releaseMutex(mMutexListeSockets);
        throw ExceptionReseau("Trop de users connectes");
    }



	// On ecrase le dernier socket qui etait associe a ce nom de joueur et a ce type de socket
	mListeSockets[std::pair<std::string, ConnectionType>(pNomJoueur, pSocket->getConnectionType())] = pSocket;
    if(pSocket->getConnectionType() == TCP)
    {
	    mCommunicateurReseau.ajouterSocketEcoute(pSocket);
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
    auto it = mCommunicateurReseau.getFirstSocketEcoute(); // On va egalement chercher le mutex vers la liste de Sockets a ecouter
	for(it; it!=mCommunicateurReseau.getEndSocketEcoute(); ++it)
	{
		if((*it)==wSocketASupprimer)
		{
			// Le supprimer de la liste de Socket a ecouter
			mCommunicateurReseau.supprimerEcouteSocket(it);
			break;
		}
	}
    // On relache le mutex
	mCommunicateurReseau.terminerIterationListeSocketEcoute();

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
#ifdef WINDOWS
        localtime_s( &wTimeNow, &wTime );
#elif defined(LINUX)
        time(&wTime);
        wTimeNow = *localtime(&wTime);
#endif
        errorLogHandle << "[" << wTimeNow.tm_hour << ":" << wTimeNow.tm_min << ":" << wTimeNow.tm_sec << "] ";
#ifdef WINDOWS
        errorLogHandle << ExceptionReseau::getLastErrorMessage("", WSAGetLastError());
#endif
        errorLogHandle << " " << pMessage << std::endl;
		errorLogHandle.flush(); // Pour etre certain d'avoir tout meme si le programme crash
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
#ifdef WINDOWS
    int wErreur = WSAGetLastError();
#elif defined(LINUX)
    int wErreur = -1;
#endif

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
    PaquetEvent* wPaquet = (PaquetEvent*) GestionnaireReseau::obtenirInstance()->creerPaquet(EVENT);
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
}




void GestionnaireReseau::initServer()
{
    init();

    // Demarrer le thread de connexion TCP
    mCommunicateurReseau.demarrerThreadsConnectionServeur();

    

    // Demarrer Thread connexion automatique
}

void GestionnaireReseau::initGameServer()
{
    mUsername = "GameServer";
    mPassword = "HockeduSuperProtectedPassword";
    initServer();
}

void GestionnaireReseau::demarrerConnectionThread( SPSocket pSocket )
{
    mCommunicateurReseau.demarrerConnectionThread(pSocket);
}


void GestionnaireReseau::supprimerEcouteSocket( SPSocket pSocket )
{
    mCommunicateurReseau.supprimerEcouteSocket(pSocket);
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

