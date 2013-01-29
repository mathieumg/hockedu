//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseau.cpp
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireReseau.h"
#include <strstream>
#include <algorithm>
#include <WinSock2.h>
#include <exception>
#include <map>
#include "UsinePaquets\UsinePaquet.h"
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
#include "PaquetHandlers/PacketHandlerBase.h"
#include "PaquetHandlers/PacketHandlerConnAutomatique.h"
#include "PaquetHandlers/PacketHandlerString.h"
#include "UsinePaquets/UsinePaquetBase.h"
#include "UsinePaquets/UsinePaquetConnAutomatique.h"
#include "UsinePaquets/UsinePaquetString.h"
#include "UsinePaquets/UsinePaquetTest.h"
#include "PaquetHandlers/PacketHandlerTest.h"
#include <utility>

// lien avec la librairie winsock2
#pragma comment( lib, "ws2_32.lib" )

ByteOrder GestionnaireReseau::NATIVE_BYTE_ORDER = UNKNOWN;

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(GestionnaireReseau);

// Mode du terminal courant (ex: CLIENT OU SERVEUR)
NetworkMode GestionnaireReseau::mNetworkMode = NOT_DEFINED;

// Port utilise pour la connexion automatique (Multicast en UDP)
int GestionnaireReseau::multicastPort = 1001;

// Port a utiliser pour les communications de base
int GestionnaireReseau::communicationPort = 5010;


// Network Log setup
std::ofstream GestionnaireReseau::logSetup()
{
	std::ofstream wFstream;
	time_t wTime = time(0);
	struct tm wTimeNow;
	localtime_s( &wTimeNow, &wTime );
	std::stringstream wFilename;
	wFilename << "NETWORK_LOG_" << wTimeNow.tm_mon << "_" << wTimeNow.tm_mday << "_" << wTimeNow.tm_hour << "_" << wTimeNow.tm_min << "_" << wTimeNow.tm_sec << ".txt";

	wFstream.open(wFilename.str(), std::fstream::out);
	return wFstream;
}

std::ofstream GestionnaireReseau::mLogHandle = GestionnaireReseau::logSetup();




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
GestionnaireReseau::GestionnaireReseau(): mSocketStateCallback(NULL)
{
	if(mNetworkMode == NOT_DEFINED) {
		throw ExceptionReseau("Appel du constructeur de GestionnaireReseau avant GestionnaireReseau::setNetworkMode", NULL);
	}

    
    getNativeByteOrder();

	mMutexListeSockets = CreateMutex(NULL, false, NULL);


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
	if(!mLogHandle.fail())
	{
		mLogHandle.close();
	}
    WSACleanup();
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


    // Ajout des classes PacketHandler et UsinePaquet de base
    ajouterOperationReseau("Base", new PacketHandlerBase(), new UsinePaquetBase());
    ajouterOperationReseau("String", new PacketHandlerString(), new UsinePaquetString());
    ajouterOperationReseau("ConnAutomatique", new PacketHandlerConnAutomatique(), new UsinePaquetConnAutomatique());
    ajouterOperationReseau("Test", new PacketHandlerTest(), new UsinePaquetTest());

	// Init Winsock2
	// --> The WSAStartup function initiates use of the Winsock DLL by a process.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	

	// Demarrer Thread connexion automatique


	

    
    // Demarrer thread conn TCP Serveur
    if(GestionnaireReseau::getNetworkMode() == SERVER)
    {
        mCommunicateurReseau.demarrerThreadsConnectionTCPServeur();
    }
    



    if(iResult != NO_ERROR)
    {
        throw ExceptionReseau("Erreur init de Gestionnaire Reseau", NULL);
    }


}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireReseau::getAdresseIPLocaleAssociee( const std::string& pDestinationIP )
///
/// Permet d'obtenir l'adresse IP locale qui correspond aune adresse IP externe.
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
	hostent *hostCourant;
	hostCourant=gethostbyname("");

	int wNbCount = 0;
	int wNbBadCount = 0;
	while(hostCourant->h_addr_list[wNbCount])
	{
		wIpLocaleFinale = inet_ntoa(*(struct in_addr *)hostCourant->h_addr_list[wNbCount]);
		wNbCount++;
		if(wIpLocaleFinale.substr(0, wIpLocaleFinale.find_first_of(".")) == pDestinationIP.substr(0, pDestinationIP.find_first_of("."))) {
			// Good network interface
			break;
		}
		wNbBadCount++;
	}

	// IP not found in the spcified subnet
	if(wNbCount == wNbBadCount)
	{
        if(pDestinationIP == "127.0.0.1") // For debug (using localhost)
        {
            return pDestinationIP;
        }
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
void GestionnaireReseau::ajouterOperationReseau(const std::string& pNomOperation, PacketHandler* pPacketHandler, UsinePaquet* pUsine)
{
	// Valider le nom
	if(pNomOperation.length() > longueurMaxOperationReseau)
	{
		throw std::exception("Nom d'operation trop long");
	}
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
	std::map<std::string, PacketHandler*>::iterator itHandlers = mListeHandlers.begin();
	for(itHandlers; itHandlers!=this->mListeHandlers.end(); ++itHandlers)
	{
		delete (*itHandlers).second;
	}
	mListeHandlers.clear();

	// Vide liste Usines
	std::map<std::string, UsinePaquet*>::iterator itUsines = mListeUsines.begin();
	for(itUsines; itUsines!=this->mListeUsines.end(); ++itUsines)
	{
		delete (*itUsines).second;
	}
	mListeUsines.clear();

    // Vider la liste de Sockets et les detruire
    std::map<std::pair<std::string, ConnectionType>, SPSocket>::iterator iter;
    WaitForSingleObject(mMutexListeSockets,INFINITE);
//     for (iter = mListeSockets.begin(); iter != mListeSockets.end(); ++iter)
//     {
//         delete (*iter).second;
//     }
    mListeSockets.clear();
    ReleaseMutex(mMutexListeSockets);
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
            delete pPaquet;
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
/// @param[in] const std::string & pPlayerName : Nom du joueur qui désire envoyer le paque
/// @param[in] Paquet* pPaquet                 : Paquet a envoyer (sera delete par le CommunicateurReseau une fois que le paquet sera envoye)
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GestionnaireReseau::envoyerPaquet( const std::string& pPlayerName, Paquet* pPaquet, ConnectionType pConnectionType )
{
    SPSocket pSocket = getSocket(pPlayerName,pConnectionType);
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
bool GestionnaireReseau::validerOperation( const std::string& pOperation ) const
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
PacketHandler* GestionnaireReseau::getPacketHandler( const std::string& pOperation )
{
    std::map<std::string, PacketHandler*>::iterator it = mListeHandlers.find(pOperation);
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
Paquet* GestionnaireReseau::creerPaquet( const std::string& pOperation ) 
{
    std::map<std::string, UsinePaquet*>::iterator wIterateur = mListeUsines.find(pOperation);

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
/// @param[in] const std::string& pPlayerName,	: Nom du joueur (local) qui doit etre associe a cette connection
/// @param[in] const std::string& pIP			: Adresse IP du Socket a creer
/// @param[in] ConnectionType pConnectionType	: Type de connectiona  demarrer (TCP ou UDP)
/// 
/// @return     : Socket*	: Pointeur vers le socket qui vient d'être créer
///
////////////////////////////////////////////////////////////////////////
SPSocket GestionnaireReseau::demarrerNouvelleConnection(const std::string& pPlayerName, const std::string& pIP, ConnectionType pConnectionType)
{
	SPSocket wNewSocket = SPSocket(new Socket(pIP, GestionnaireReseau::communicationPort, pConnectionType));

// 	if(GestionnaireReseau::mObserverSocket != NULL && GestionnaireReseau::getNetworkMode() == SERVER)
// 	{
// 		wNewSocket->attach(mObserverSocket);
// 	}
// 


	saveSocket(pPlayerName, wNewSocket);
	return wNewSocket;
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
    WaitForSingleObject(mMutexListeSockets,INFINITE);
//     try
//     {
//         std::map<std::pair<std::string, ConnectionType>, SPSocket>::iterator it = mListeSockets.find(std::pair<std::string, ConnectionType>(pNomJoueur, pSocket->getConnectionType()));
//         if(it != mListeSockets.end())
//         {
//             delete (*it).second;
//         }
//         
//     }
//     catch(...){} // Pour etre certain de ne pas planter pour rien

	// On ecrase le dernier socket qui etait associe a ce nom de joueur et a ce type de socket
	mListeSockets[std::pair<std::string, ConnectionType>(pNomJoueur, pSocket->getConnectionType())] = pSocket;

	mCommunicateurReseau.ajouterSocketEcoute(pSocket);
    ReleaseMutex(mMutexListeSockets);
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
    WaitForSingleObject(mMutexListeSockets,INFINITE);
	ListeSockets::iterator iterateurRecherche = mListeSockets.find(std::pair<std::string, ConnectionType>(pNomJoueur, pConnectionType));
	if(mListeSockets.end() == iterateurRecherche)
	{
        ReleaseMutex(mMutexListeSockets);
		return NULL;
	}
	else
	{
        ReleaseMutex(mMutexListeSockets);
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
    WaitForSingleObject(mMutexListeSockets,INFINITE);
	ListeSockets::iterator itMap = mListeSockets.find(std::pair<std::string, ConnectionType>(pNomJoueur, pConnectionType));
	if(itMap == mListeSockets.end())
	{
        // Le socket n'est pas dans la liste de sockets sauvegardes
		ReleaseMutex(mMutexListeSockets);
		return;
	}
	SPSocket wSocketASupprimer = itMap->second;

    // On attend pour ne pas supprimer le socket pendant une operation critique
    HANDLE wHandle = wSocketASupprimer->getMutexActiviteSocket();
    WaitForSingleObject(wHandle,INFINITE);

    // On doit le retirer de la liste de Socket a ecouter avant de le supprimer
	std::list<SPSocket>::const_iterator it = mCommunicateurReseau.getFirstSocketEcoute(); // On va egalement chercher le mutex vers la liste de Sockets a ecouter
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
	
	// Le supprimer de la liste dans cette classe
	mListeSockets.erase(itMap);

	// Liberer la memoire et invalider le pointeur
	wSocketASupprimer = 0;

    ReleaseMutex(wHandle); // On relache le mutex sur le socket
    ReleaseMutex(mMutexListeSockets);
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
    WaitForSingleObject(mMutexListeSockets,INFINITE);
    for (std::map<std::pair<std::string, ConnectionType>, SPSocket>::iterator it = mListeSockets.begin(); it!=mListeSockets.end(); ++it)
    {
        if((*it).second == pSocket)
        {
            ReleaseMutex(mMutexListeSockets);
            removeSocket(it->first.first,it->first.second); // On laisse l'autre methode faire le retrait
            break;
        }
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
		NATIVE_BYTE_ORDER = BIG_ENDIAN;
	else if( !test.c[3] && !test.c[2] && !test.c[1] && test.c[0] )
		NATIVE_BYTE_ORDER = LITTLE_ENDIAN;
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
    time_t wTime = time(0);
	struct tm wTimeNow;
	localtime_s( &wTimeNow, &wTime );
	std::stringstream wMessage;
	wMessage << "[" << wTimeNow.tm_hour << ":" << wTimeNow.tm_min << ":" << wTimeNow.tm_sec << "] " << ExceptionReseau::getLastErrorMessage("", WSAGetLastError()) << " " << pMessage << std::endl;
	
    // On verifie que le fichier d'output a bien pu etre creer au demarrage
	if(!mLogHandle.fail())
	{
		// Si on peut ecrire dans le fichier, on le fait
        std::string wMessageString = wMessage.str();
		mLogHandle.write(wMessageString.c_str(), wMessageString.length());
		mLogHandle.flush(); // Pour etre certain d'avoir tout meme si le programme crash
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
    int wErreur = WSAGetLastError();
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
std::string GestionnaireReseau::getPlayerName( SPSocket pSocket )
{
    WaitForSingleObject(mMutexListeSockets,INFINITE);
	std::string name;
    for (std::map<std::pair<std::string, ConnectionType>, SPSocket>::iterator it = mListeSockets.begin(); it!=mListeSockets.end(); ++it)
    {
        if((*it).second == pSocket)
        {
            name = (*it).first.first;
            break;
        }
    }
    ReleaseMutex(mMutexListeSockets);
    return name;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn std::set<std::string> GestionnaireReseau::getPlayerNameList() const
///
/// Methode pour obtenir la liste des noms des joueurs connectes
/// 
/// @return std::set<std::string>   : Liste des noms
///
////////////////////////////////////////////////////////////////////////
std::set<std::string> GestionnaireReseau::getPlayerNameList(ConnectionType pConnectionType) const
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
		pEvent.mPlayerName = getPlayerName(pSocket);
		mSocketStateCallback(pEvent);
	}
}










///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

