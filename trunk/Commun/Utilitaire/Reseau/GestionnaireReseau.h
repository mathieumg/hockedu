//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireReseau.h
/// @author Mathieu Parent
/// @date 2012-12-01
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "Singleton.h"
#include <winerror.h>
#include <map>
#include "Socket.h"
#include "ExceptionsReseau\ExceptionReseau.h"
#include "CommunicateurReseau.h"
#include <fstream>
#include <set>
#include "ControllerInterface.h"

class PacketHandler;
class UsinePaquet;
// Typedef pour la liste de sockets
typedef std::map<std::pair<std::string, ConnectionType>, SPSocket> ListeSockets;

enum NetworkMode {CLIENT, SERVER, NOT_DEFINED};

enum ByteOrder {NATIVE, LITTLE_ENDIAN, BIG_ENDIAN, UNKNOWN};

enum ExceptionTypes {GLOBALE, PARAMETRE_INVALIDE, SOCKET_DECONNECTE, TIMEOUT, TYPE_NOT_DEFINED, AUCUNE_ERREUR};

enum EventCodes {
    USER_ALREADY_CONNECTED, 
    USER_DID_NOT_SEND_NAME_ON_CONNECTION, 
    USER_CONNECTED, 
    USER_DISCONNECTED, 
    CONNECTION_CANCELED,
    RECONNECTION_TIMEOUT, 
    RECONNECTION_IN_PROGRESS, 
    WRONG_PASSWORD, 
    CHAT_MESSAGE_RECEIVED,
    SERVER_USER_CONNECTED,
    SERVER_USER_DISCONNECTED,
    NB_EVENT_CODES // Must be always last !
};


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
	// Taille des paquets qui disent la longeur des 2e paquets en UDP
	static const int TAILLE_PAQUET_CONTROLE_UDP = 10;
	// Longueur max des noms d'operations
	static const int longueurMaxOperationReseau = 20;
	// Port a utiliser pour la communication multicast
	static int multicastPort;
	// Port a utiliser pour les comminications de base
	static int communicationPort;
    // Port a utiliser pour les comminications de base
    static int communicationUDPPort;
    // Port a utiliser pour les comminications de base
    static int connectionUDPPort;



    // Methode d'initialisation (initialise notamment Winsock)
	void init();

    void disconnectClient(const std::string& pPlayerName, ConnectionType pConnectionType = TCP);

	// Set the network mode
	inline static void setNetworkMode(NetworkMode pNetworkMode) {mNetworkMode = pNetworkMode;}

    // Get the network mode
    inline static NetworkMode getNetworkMode() {return mNetworkMode;}

	// Get the server socket observer
	void setSocketConnectionStateCallback(SocketConnectionStateCallback val) { mSocketStateCallback = val; }
	void socketConnectionStateEvent(SPSocket pSocket, ConnectionStateEvent& pEvent);
	
    // Methode pour configurer le Controlleur a associer avec la Modele
    inline void setController(ControllerInterface* pController) {mControlleur = pController;}
    inline ControllerInterface* getController() const {return mControlleur;}

	// Methode pour obtenir l'adresse IP locale qui est dans le meme subnet que l'adresse IP passee en parametre
	std::string getAdresseIPLocaleAssociee(const std::string& pDestinationIP);

	// Methode pour ajouter une operation reseau (ex: deplacerNoeud). Le handler doit etre fourni ainsi que l'usine pour creer les noeuds
	void ajouterOperationReseau(const std::string& pNomOperation, PacketHandler* pPacketHandler, UsinePaquet* pUsine);
	
	// Verifie le type de socket et utilise le bon communicateurReseau afin de transmettre la message
    void envoyerPaquet(SPSocket pSocketAUtiliser, Paquet* pPaquet);
	void envoyerPaquet(const std::string& pPlayerName, Paquet* pPaquet, ConnectionType pConnectionType);

	/*// Demarre la reception de paquets dans un thread separe afin que le communicateur reseau du meme type que le socket ecoute et gere la reception sur ce socket
	Paquet* recevoirPaquets(Socket* pSocketAUtiliser);*/

	void envoyerPaquetBroadcast(Paquet* pPaquet);

	// True si valide
	bool validerOperation(const std::string& pOperation) const;
	PacketHandler* getPacketHandler(const std::string& pOperation);

	// Methode pour sauvegarder un socket en fonction d'un nom de joueur
	void saveSocket(const std::string& pNomJoueur, SPSocket pSocket);

	// Methode pour obtenir le socket associe a un nom de joueur
	SPSocket getSocket(const std::string& pNomJoueur, ConnectionType pConnectionType);
	
	// Methode pour enlever le Socket de la liste de sockets valides
	void removeSocket(const std::string& pNomJoueur, ConnectionType pConnectionType);
    void removeSocket(SPSocket pSocket); // Pas optimal, mais necessaire dans certains cas

    // Methode (PAS OPTIMALE) pour retourner le nom du player associee a un socket
    std::string getPlayerName(SPSocket pSocket);

    // Retourne la liste des joueurs connectes
    std::set<std::string> getPlayerNameList(ConnectionType pConnectionType) const;

    // Retourne la liste des adresses IP disponibles sur la machine courante
    void getListeAdressesIPLocales(std::list<std::string>& pOut) const;

	// Methode qui appelle la Factory de Paquets et qui retourne un Paquet du bon type représenté par le nom d'opération
	Paquet* creerPaquet(const std::string& pOperation);

	// Methode pour creer une nouvelle connection (un nouveau Socket) et le sauvegarder a la liste des Sockets actifs
	void demarrerNouvelleConnection(const std::string& pPlayerName, const std::string& pIP, ConnectionType pConnectionType);
    /// Permet d'arreter d'essayer de connecter un socket
    void cancelNewConnection(const std::string& pPlayerName, ConnectionType pConnectionType = TCP);

    // Methode pour envoyer des events au Controlleur
    void transmitEvent(int pMessageCode, ...) const;

	void gererExceptionReseau(ExceptionReseau* pException) const;
    
    //Determines native byte order of a system.
	static void determineNativeByteOrder();
	//Returns system native byte order
	static ByteOrder& getNativeByteOrder();


	// Methode pour ecrire une erreur dans le log de reseautique
	static void sendMessageToLog(const std::string& pMessage);

    // Methode pour throw une exception du bon type
    void throwExceptionReseau(const std::string& pMessagePrefix = "") const;


    //Contains the operating system's byte order.
	static ByteOrder NATIVE_BYTE_ORDER;
private:
    

    // Methode de nettoyage
	void supprimerPacketHandlersEtUsines();

    // Controlleur associe au modele
    ControllerInterface* mControlleur;
	

	// Liste des handlers pour les paquets
	std::map<std::string, PacketHandler*> mListeHandlers;
	// Liste des usines de paquets avec la clée = nom de l'opération
	std::map<std::string, UsinePaquet*> mListeUsines;

    std::map<int, ExceptionTypes> mMapExceptions;

	// Communicateur Reseau a utiliser
	CommunicateurReseau mCommunicateurReseau;

	// Liste des Sockets associes par un key:(string et un type de socket)
	ListeSockets mListeSockets;

    // Mutex pour la gestion de la liste des sockets (doit etre acquis si on modifie la liste de sockets)
    HANDLE mMutexListeSockets;

    // Network Mode (CLIENT OR SERVER)
	static NetworkMode mNetworkMode;


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

