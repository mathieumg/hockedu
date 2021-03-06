//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseau.h
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "ExceptionsReseau/ExceptionReseau.h"
#include "FacadePortability.h"
#include "Singleton.h"
#include <map>
#include "Socket.h"
#include "CommunicateurReseau.h"
#include <fstream>
#include <set>
#include "ControllerInterface.h"
#include "NetworkEnums.h"

#ifdef LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#endif

#define NETWORK_LOG(...) GestionnaireReseau::obtenirInstance()->sendMessageToLog(__FILE__, __LINE__, ##__VA_ARGS__);

#if !SHIPPING
#define PACKETS_SEND_LOG(content,l) GestionnaireReseau::obtenirInstance()->PacketSendToLog(content,l);
#define PACKETS_RECEIVE_LOG(content,l) GestionnaireReseau::obtenirInstance()->PacketReceivedToLog(content,l);
#else
#define PACKETS_SEND_LOG(content,l)
#define PACKETS_RECEIVE_LOG(content,l)
#endif

class PacketHandler;
class UsinePaquet;
struct _ENetHost;
// Typedef pour la liste de sockets
typedef std::map<std::pair<std::string, ConnectionType>, SPSocket> ListeSockets;


struct ConnectionStateEvent
{
	std::string mPlayerName;
	ConnectionState mState;
};

typedef void (*SocketConnectionStateCallback)(const ConnectionStateEvent&);

///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireReseau
/// @brief Classe qui contient des fonctions globales et des helpers globaux pour la reseautique
///
/// @author Mathieu Parent
/// @date 2012-12-01
///////////////////////////////////////////////////////////////////////////
class GestionnaireReseau : public Singleton<GestionnaireReseau>
{
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireReseau);
public:
	// Taille des buffers de read et write lors des appels a recv et send
	static const int TAILLE_BUFFER_RECEPTION_ENVOI = 10000;
    //
    static const int TAILLE_BUFFER_HOSTNAME = 256;
	// Taille des paquets qui disent la longeur des 2e paquets en UDP
	static const int TAILLE_PAQUET_CONTROLE_UDP = 10;
	// Longueur max des noms d'operations
	static const int longueurMaxOperationReseau = 20;
	// Port a utiliser pour la communication multicast
	static int multicastPort;
	// Port a utiliser pour les comminications de base
	static int communicationPort;
    // Port a utiliser pour les comminications de base
    static int communicationPortMasterServer;
    // Port a utiliser pour les comminications de base
    static int communicationUDPPortClientLourd;
    // Port a utiliser pour les comminications de base
    static int communicationUDPPortServeurJeu;

    inline const std::string& getUserId() const { return mUserId; }
    inline void setUserId(const std::string& val) { mUserId = val; }

    // Initialise le GestionnaireReseau
    void initClient(const std::string& pUsername = "", const std::string& pPassword = "");
    /// Initialise le GestionnaireReseau avec des fonctionnalites communes des serveurs (listen de ports, etc.)
	/// must be called from initGameServer and  initMasterServer
    void initServer();
    /// Initialise le GestionnaireReseau pour des GameServers
	void initGameServer();
	/// Initialise le GestionnaireReseau pour des MasterServers
	void initMasterServer();

    void setUser(const std::string& pUsername, const std::string& pPassword);

    // Demarre la connection d'un socket TCP dans un thread separe
    void demarrerConnectionThread(SPSocket pSocket);

    // Methode pour supprimer un socket dans la liste a ecouter selon son pointeur
    void supprimerEcouteSocket(SPSocket pSocket);

    // Methode a appeler dans le client quand on veut se deconnecter
    void disconnectClient(const std::string& pPlayerName, ConnectionType pConnectionType = TCP);

    // Methode a appeler du serveur pour se deconnecter d'un client
    //void disconnectOtherPlayer(const std::string& pPlayerName, ConnectionType pConnectionType = TCP);

	// Get the server socket observer
	void setSocketConnectionStateCallback(SocketConnectionStateCallback val) { mSocketStateCallback = val; }
	void socketConnectionStateEvent(SPSocket pSocket, ConnectionStateEvent& pEvent);

    // Methode pour configurer le Controlleur a associer avec la Modele
    inline void setController(ControllerInterface* pController) {mControlleur = pController;}
    inline ControllerInterface* getController() const {return mControlleur;}

	// Methode pour obtenir l'adresse IP locale qui est dans le meme subnet que l'adresse IP passee en parametre
	std::string getAdresseIPLocaleAssociee(const std::string& pDestinationIP);
    /// unsigned important, all 32 bits used
    std::string getAdresseIPLocaleAssociee( unsigned int minAdress, unsigned int maxAdress );

	// Methode pour ajouter une operation reseau (ex: deplacerNoeud). Le handler doit etre fourni ainsi que l'usine pour creer les noeuds


	// Verifie le type de socket et utilise le bon communicateurReseau afin de transmettre la message
    void envoyerPaquet(SPSocket pSocketAUtiliser, Paquet* pPaquet);
	void envoyerPaquet(const std::string& pPlayerName, Paquet* pPaquet, ConnectionType pConnectionType);

	/*// Demarre la reception de paquets dans un thread separe afin que le communicateur reseau du meme type que le socket ecoute et gere la reception sur ce socket
	Paquet* recevoirPaquets(Socket* pSocketAUtiliser);*/

	void envoyerPaquetBroadcast(Paquet* pPaquet);

	// True si valide
	bool validerOperation(const PacketTypes pOperation) const;

    // True si valide
    bool validerUsername(const std::string& pUsername) const;


	// Methode pour sauvegarder un socket en fonction d'un nom de joueur
	void saveSocket(const std::string& pNomJoueur, SPSocket pSocket);

    // Methode pour obtenir le socket associe a un nom de joueur
    SPSocket getSocket(const std::string& pNomJoueur, ConnectionType pConnectionType);

    // Methode pour enlever le Socket de la liste de sockets valides
    void removeSocket(const std::string& pNomJoueur, ConnectionType pConnectionType);
    void removeSocket(SPSocket pSocket); // Pas optimal, mais necessaire dans certains cas


    // Methode pour retourner l'id d'une connexion
    std::string getConnectionId(SPSocket pSocket);


    // Methode pour retourner le nom du player (ne devrait pas etre utilisee par le serveur)
    std::string getPlayerName();

    std::string getSocketIdentifier(SPSocket wSocket);

    void getEveryoneConnected(std::vector<const std::string*>& pList);

    // Methode pour retourner le mot de passe du joueur
    std::string getPlayerPassword();

    // Retourne la liste des joueurs connectes
    std::set<std::string> getConnectionIdList(ConnectionType pConnectionType) const;

    // Retourne la liste des adresses IP disponibles sur la machine courante
    void getListeAdressesIPLocales(std::list<std::string>& pOut) const;

	// Methode pour creer une nouvelle connection (un nouveau Socket) et le sauvegarder a la liste des Sockets actifs
	void demarrerNouvelleConnection(const std::string& pConnectionId, const std::string& pIP, ConnectionType pConnectionType);
    /// Permet d'arreter d'essayer de connecter un socket
    void cancelNewConnection(const std::string& pPlayerName, ConnectionType pConnectionType = TCP);

    // Methode pour envoyer des events au Controlleur
    void transmitEvent(EventCodes pMessageCode, ...) const;

	void gererExceptionReseau(ExceptionReseau* pException) const;

    //Determines native byte order of a system.
	static void determineNativeByteOrder();
	//Returns system native byte order
	static ByteOrder& getNativeByteOrder();


	// Methode pour ecrire une erreur dans le log de reseautique
    static void sendMessageToLog(const std::string& pMessage);
    static void sendMessageToLog(const char* File, int Line, const char* Format="", ... );
    static void PacketSendToLog(const char* pMessage, int length);
    static void PacketReceivedToLog(const char* pMessage, int length);


    // Methode pour throw une exception du bon type
    void throwExceptionReseau(const std::string& pMessagePrefix = "") const;


    inline bool requireAuthentification() const {return mControlleur->requireAuthentification();}

    inline std::string authenticate(const std::string& pUsername, const std::string& pPassword)
    {
        mUserId = mControlleur->authenticate(pUsername, pPassword);
        if(mUserId != "-1" && pUsername!= "GameServer")
        {
            return pUsername;
        }
        else if(pUsername == "GameServer")
        {
            return mUserId;
        }
        else
        {
            return "";
        }
    }

    bool isMasterServerConnected() const;

    bool isAGameServerConnected() const;


private:

    // Methode d'initialisation (initialise notamment Winsock)
    void init();

    // Methode de nettoyage
	void supprimerPacketHandlersEtUsines();

    // Userinfo
    std::string mUsername;
    std::string mPassword;
    std::string mUserId;

    // Controlleur associe au modele
    ControllerInterface* mControlleur;

    std::map<int, ExceptionTypes> mMapExceptions;

	// Communicateur Reseau a utiliser
	CommunicateurReseau* mCommunicateurReseau;

	// Liste des Sockets associes par un key:(string et un type de socket)
	ListeSockets mListeSockets;

    // Mutex pour la gestion de la liste des sockets (doit etre acquis si on modifie la liste de sockets)
    HANDLE_MUTEX mMutexListeSockets;

	_ENetHost * mServerHost, *mClientHost;

	SocketConnectionStateCallback mSocketStateCallback;

protected:
    /// Constructeur par défaut.
    GestionnaireReseau();
    /// Destructeur.
    ~GestionnaireReseau();
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

