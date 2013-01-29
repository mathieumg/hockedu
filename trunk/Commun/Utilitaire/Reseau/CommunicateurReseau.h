//////////////////////////////////////////////////////////////////////////////
/// @file CommunicateurReseau.h
/// @author Mathieu Parent
/// @date 2013-01-08
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <queue>
#include <concrt.h>
#include "Reseau/Socket.h"
#include <list>
#include <map>
#include "QueueThreadSafe.h"

class Paquet;
struct PaquetAEnvoyer {
	Socket* socket;
	Paquet* paquet;
};



///////////////////////////////////////////////////////////////////////////
/// @class CommunicateurReseau
/// @brief Classe qui permet d'envoyer et de recevoir des paquets et qui contient la liste des paquets a envoyer
/// ainsi que les paquets recus qui sont a executer sur le thread principal (basically pour gérer les threads de communication et les echanges de paquets)
///
/// @author Mathieu Parent
/// @date 2013-01-08
///////////////////////////////////////////////////////////////////////////
class CommunicateurReseau 
{
public:
	// Constructeur
	CommunicateurReseau();

	// Destructeur
	~CommunicateurReseau();

	// Methode pour ajouter un paquet a la liste a envoyer
	bool ajouterPaquetEnvoie(Socket* pSocket, Paquet* pPaquet);

	// Methode pour avoir un pointeur sur la queue de paquets a envoyer
	QueueThreadSafe<PaquetAEnvoyer*>* getSendingList();

	// Methode pour avoir un pointeur sur la queue de paquets recus
	QueueThreadSafe<Paquet*>* getReceivedList();

	// Methode pour ajouter un socket a ecouter
	void ajouterSocketEcoute(Socket* pSocket);

	// Methode qui retourne un iterateur sur la liste de sockets a ecouter ET QUI MET UN VERROU.
	std::list<Socket*>::const_iterator getFirstSocketEcoute() const;

	// Methode pour avoir le end de la liste de sockets a ecouter
	std::list<Socket*>::const_iterator getEndSocketEcoute() const;

	// Methode qui relache le mutex de la liste de sockets a ecouter
	void terminerIterationListeSocketEcoute();

	// Methode pour supprimer un socket dans la liste de sockets a ecouter
    std::list<Socket*>::const_iterator supprimerEcouteSocket(const std::list<Socket*>::const_iterator& pIterateur);

    // Methode pour supprimer un socket dans la liste a ecouter selon son pointeur
    void supprimerEcouteSocket(Socket* pSocket);

	// Demarre la connection d'un socket TCP dans un thread separe
	void demarrerConnectionThread(Socket* pSocket);

    // Methode pour demarrer les threads de connection TCP
    void demarrerThreadsConnectionTCPServeur();

	// Enleve le socket et son thread de connection de la liste (surtout appelee par le thread lui meme quand il a terminer son travail)
	void enleverConnectionThread(Socket* pSocket, bool pSuccess);


	static unsigned int maxBufferSize;

private:
	// Methode pour demarrer le thread d'envoie
	void demarrerSendingThread();

	//Methode pour demarrer le thread de lecture
	void demarrerReceivingThread();


	// Code d'execution du thread d'envoie
	static void *sendingThreadRoutine(void *arg);

	// Code d'execution du thread de lecture
	static void *receivingThreadRoutine(void *arg);

	// Code d'execution du thread de connection
	static void *connectionThreadRoutine(void *arg);

    static void *connectionTCPServeurThreadRoutine(void *arg);

	// Methode pour avertir le thread d'envoie que des paquets sont a envoyer (le thread se met en wait si la file est vide)
	void wakeThreadEnvoie() const;


	// Les deux files fifo  qui contiennent les Paquets a echanger entre les threads (paquets a envoyer et paquets recus)
	QueueThreadSafe<PaquetAEnvoyer*> mListeEnvoie;
	QueueThreadSafe<Paquet*> mListeReception;

	// Liste des Socket a "ecouter"
	std::list<Socket*> mListeSocketsEcoute;

	// Liste des sockets a connecter
	std::list<Socket*> mListeSocketsConnection;

	// Mutex pour l'acces a mListeSocketsEcoute
	HANDLE mMutexListeSocketsEcoute;

	// Mutex pour l'acces a mListeSocketsConnection
	HANDLE mMutexListeSocketsConnection;

	// Handle du thread qui s'occupe d'envoyer les paquets
	HANDLE mHandleThreadEnvoie;
	// Handle du thread qui s'occupe de recevoir les paquets
	HANDLE mHandleThreadReception;

    // Liste des HANDLE pour les threads de connection TCP
    std::vector<HANDLE> mHandlesThreadConnectionTCPServeur;

	// Map de handles de thread de connection selon les sockets
	std::map<Socket*, HANDLE> mHandlesThreadConnection;

};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////




