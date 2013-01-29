//////////////////////////////////////////////////////////////////////////////
/// @file CommunicateurReseau.cpp
/// @author Mathieu Parent
/// @date 2013-01-08
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "CommunicateurReseau.h"
#include "ExceptionsReseau\ExceptionReseau.h"
#include <winbase.h>
#include <iostream>
#include "GestionnaireReseau.h"
#include "Paquets/Paquet.h"
#include "ExceptionsReseau\ExceptionReseauSocketDeconnecte.h"
#include "ExceptionsReseau\ExceptionReseauTimeout.h"
#include "SocketTCPServeur.h"
#include "PacketBuilder.h"
#include "PaquetHandlers\PacketHandler.h"
#include "PaquetHandlers\PacketHandlerBase.h"
#include "Paquets/PaquetBase.h"
#include "PacketReader.h"
#include "GestionnaireReseau.h"
#include "Utilitaire.h"

// Taille maximale des buffers pour l<envoie et la reception
unsigned int CommunicateurReseau::maxBufferSize = 5000;

struct ArgsConnectionThread {
	CommunicateurReseau* communicateurReseau;
	SPSocket socketAConnecter;
};

////////////////////////////////////////////////////////////////////////
///
/// @fn CommunicateurReseau::CommunicateurReseau()
///
/// Constructeur 
///
/// @return
///
////////////////////////////////////////////////////////////////////////
CommunicateurReseau::CommunicateurReseau():mListeEnvoie(maxBufferSize),mListeReception(maxBufferSize)
{
	mMutexListeSocketsEcoute = CreateMutex(NULL, FALSE, NULL);
	mMutexListeSocketsConnection = CreateMutex(NULL, FALSE, NULL);
	demarrerSendingThread();
	demarrerReceivingThread();
    
    
}


////////////////////////////////////////////////////////////////////////
///
/// @fn CommunicateurReseau::~CommunicateurReseau()
///
/// Destructeur
///
/// @return
///
////////////////////////////////////////////////////////////////////////
CommunicateurReseau::~CommunicateurReseau()
{
	// On doit supprimer les Paquets contenus dans les files de paquets. Pas besoin d'etre effectuer souvent alors pas besoin d'etre performant.
	// Ces paquets ne seront jamais envoyes
	while(!mListeEnvoie.empty())
	{
		PaquetAEnvoyer* paquet = NULL;
		mListeEnvoie.pop(paquet);
		if(paquet)
		{
			delete paquet;
		}
	}
	while(!mListeReception.empty())
	{
		Paquet* paquet = NULL;
		mListeReception.pop(paquet);
		if(paquet)
		{
			delete paquet;
		}
	}

    for(std::vector<HANDLE>::iterator it = mHandlesThreadConnectionTCPServeur.begin(); it!=mHandlesThreadConnectionTCPServeur.end(); ++it)
    {
        // Quitter les threads
        TerminateThread((*it), 0);
    }
    mHandlesThreadConnectionTCPServeur.empty();

}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool CommunicateurReseau::ajouterPaquetEnvoie( Socket* pSocket, Paquet* pPaquet )
///
/// Methode pour ajouter un paquet a envoyer avec le socket sur laquel l'envoyer
///
/// @param[in] Socket* pSocket  : socket sur lequel envoyer le paquet
/// @param[in] Paquet* pPaquet  : paquet a ajouter
/// 
/// @return bool    : True si l'ajout a reussi
///
////////////////////////////////////////////////////////////////////////
bool CommunicateurReseau::ajouterPaquetEnvoie( SPSocket pSocket, Paquet* pPaquet )
{
	// On sauvegarde le paquet
	PaquetAEnvoyer* paquetAEnvoyer = new PaquetAEnvoyer;
	paquetAEnvoyer->paquet = pPaquet;
	paquetAEnvoyer->socket = pSocket;
	if(!mListeEnvoie.push(paquetAEnvoyer))
	{
		// On ne peut pas ajouter le paquet a la liste car elle est pleine
		delete paquetAEnvoyer;
		return false;
	}
	return true;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::demarrerSendingThread()
///
/// Methode pour demarrer le thread d'envoie
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::demarrerSendingThread()
{
	mHandleThreadEnvoie = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendingThreadRoutine, this, 0, NULL);
	if(mHandleThreadEnvoie==NULL)
	{
		throw ExceptionReseau("Erreur lors de la creation du thread d'envoie", NULL);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::demarrerReceivingThread()
///
/// Methode pour demarrer le thread de reception
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::demarrerReceivingThread()
{
	mHandleThreadReception = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receivingThreadRoutine, this, 0, NULL);
	if(mHandleThreadReception==NULL)
	{
		throw ExceptionReseau("Erreur lors de la creation du thread de reception", NULL);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::demarrerThreadsConnectionTCPServeur()
///
/// Methode pour demarrer les threads de connection TCP cote serveur
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::demarrerThreadsConnectionTCPServeur()
{
    hostent *hostCourant;
    hostCourant=gethostbyname("");
    if(hostCourant == NULL)
    {
        // Probleme au get des valeurs reseau locales
        throw ExceptionReseau("Erreur lors de la lecture des adresses reseau locales");
        return;
    }
    
    // Ajouter localhost car pas dans la liste
    ArgsConnectionThread* wArgsLocal = new ArgsConnectionThread;
    wArgsLocal->communicateurReseau = this;
	wArgsLocal->socketAConnecter = SPSocketTCPServer(new SocketTCPServeur("127.0.0.1", GestionnaireReseau::communicationPort));
    HANDLE wHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)connectionTCPServeurThreadRoutine, wArgsLocal, 0, NULL);
    if(wHandle==NULL)
    {
        throw ExceptionReseau("Erreur lors de la creation du thread de connection TCP serveur", NULL);
    }
    else
    {
        // On ajoute le handle a la liste
        mHandlesThreadConnectionTCPServeur.push_back(wHandle);
    }
    
    int wNbCount = 0;
    while(hostCourant->h_addr_list[wNbCount])
    {
        ArgsConnectionThread* wArgs = new ArgsConnectionThread;
        wArgs->communicateurReseau = this;
        std::string wIP = inet_ntoa(*(struct in_addr *)hostCourant->h_addr_list[wNbCount]);
        wArgs->socketAConnecter = SPSocketTCPServer(new SocketTCPServeur(wIP, GestionnaireReseau::communicationPort));
        HANDLE wHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)connectionTCPServeurThreadRoutine, wArgs, 0, NULL);
        if(wHandle==NULL)
        {
            throw ExceptionReseau("Erreur lors de la creation du thread de connection TCP serveur", NULL);
        }
        else
        {
            // On ajoute le handle a la liste
            mHandlesThreadConnectionTCPServeur.push_back(wHandle);
        }
        ++wNbCount;



        /*
        ArgsConnectionThread* wArgs = new ArgsConnectionThread;
        wArgs->communicateurReseau = this;
        std::string wIP = "127.0.0.1";//inet_ntoa(*(struct in_addr *)hostCourant->h_addr_list[wNbCount]);
        wArgs->socketAConnecter = new SocketTCPServeur(wIP, GestionnaireReseau::communicationPort);
        HANDLE wHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)connectionTCPServeurThreadRoutine, &wArgs, 0, NULL);
        if(wHandle==NULL)
        {
            throw ExceptionReseau("Erreur lors de la creation du thread de connection TCP serveur", NULL);
        }
        else
        {
            // On ajoute le handle a la liste
            mHandlesThreadConnectionTCPServeur.push_back(wHandle);
        }
        ++wNbCount;*/
    }


}



////////////////////////////////////////////////////////////////////////
///
/// @fn QueueThreadSafe<PaquetAEnvoyer>* CommunicateurReseau::getSendingList()
///
/// Methode pour obtenir la liste de Paquets a envoyer
/// 
/// @return QueueThreadSafe<PaquetAEnvoyer>*    : Queue d'envoie
///
////////////////////////////////////////////////////////////////////////
QueueThreadSafe<PaquetAEnvoyer*>* CommunicateurReseau::getSendingList()
{
	return &mListeEnvoie;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn QueueThreadSafe<Paquet*>* CommunicateurReseau::getReceivedList()
///
/// Methode pour obtenir la liste de Paquets recus
/// 
/// @return QueueThreadSafe<Paquet*>*   : Queue qui contient les Paquets recus et qui n'ont pas ete consommes par le thread principal
///
////////////////////////////////////////////////////////////////////////
QueueThreadSafe<Paquet*>* CommunicateurReseau::getReceivedList()
{
	return &mListeReception;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::ajouterSocketEcoute( Socket* pSocket )
///
/// Methode pour ajouter un Socket a ecouter (A envoyer au thread d'envoie)
///
/// @param[in] Socket* pSocket  : Socket a ajouter
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::ajouterSocketEcoute( SPSocket pSocket )
{
	WaitForSingleObject(mMutexListeSocketsEcoute,INFINITE);
	mListeSocketsEcoute.push_back(pSocket);
	ReleaseMutex(mMutexListeSocketsEcoute);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn std::list<Socket*>::const_iterator CommunicateurReseau::getFirstSocketEcoute() const
///
/// Methode pour obtenir un iterateur sur la liste de Sockets a ecouter
/// Elle prend egalement un mutex sur cette liste.
/// Il faut donc absolument appeler la methode terminerIterationListeSocketEcoute() pour liberer le mutex
/// 
/// @return std::list<Socket*>::const_iterator  : Iterateur sur le debut de la liste de sockets a ecouter
///
////////////////////////////////////////////////////////////////////////
std::list<SPSocket>::const_iterator CommunicateurReseau::getFirstSocketEcoute() const
{
	DWORD wTest = WaitForSingleObject(mMutexListeSocketsEcoute,INFINITE);
	return mListeSocketsEcoute.begin();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn std::list<Socket*>::const_iterator CommunicateurReseau::getEndSocketEcoute() const
///
/// Methode pour obtenir un iterateur sur le dernier element de la liste de sockets a ecouter
/// 
/// @return std::list<Socket*>::const_iterator  : Iterateur sur le dernier element de la liste de sockets a ecouter
///
////////////////////////////////////////////////////////////////////////
std::list<SPSocket>::const_iterator CommunicateurReseau::getEndSocketEcoute() const
{
	return mListeSocketsEcoute.end();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::terminerIterationListeSocketEcoute()
///
/// Methode pour relacher le mutex de la liste de sockets a ecouter
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::terminerIterationListeSocketEcoute()
{
	ReleaseMutex(mMutexListeSocketsEcoute);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn std::list<Socket*>::const_iterator CommunicateurReseau::supprimerEcouteSocket( const std::list<Socket*>::const_iterator& pIterateur )
///
/// Methode pour supprimer un socket a ecouter
/// 
/// @return std::list<Socket*>::const_iterator  : Nouvel iterateur pour continuer la boucle si necessaire
///
////////////////////////////////////////////////////////////////////////
std::list<SPSocket>::const_iterator CommunicateurReseau::supprimerEcouteSocket( const std::list<SPSocket>::const_iterator& pIterateur )
{
	return mListeSocketsEcoute.erase(pIterateur);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::supprimerEcouteSocket( Socket* pSocket )
///
/// Methode pour supprimer un socket a ecouter selon le pointeur de socket
/// 
/// @return void cause we like it
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::supprimerEcouteSocket( SPSocket pSocket )
{
    WaitForSingleObject(mMutexListeSocketsEcoute,INFINITE);
    for(std::list<SPSocket>::iterator it = mListeSocketsEcoute.begin(); it!=mListeSocketsEcoute.end(); ++it)
    {
        if((*it) == pSocket)
        {
            supprimerEcouteSocket(it);
            break;
        }
    }
    ReleaseMutex(mMutexListeSocketsEcoute);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::demarrerConnectionThread( Socket* pSocket )
///
/// Methode pour demarrer le thread de connection
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::demarrerConnectionThread( SPSocket pSocket )
{
	if(mHandlesThreadConnection.find(pSocket) == mHandlesThreadConnection.end())
	{
		// Le socket ne possede pas deja un thread de connection
		ArgsConnectionThread* wArgs = new ArgsConnectionThread;
		wArgs->communicateurReseau = this;
		wArgs->socketAConnecter = pSocket;

		// Demarrer le thread
		HANDLE wHandleConnectionThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)connectionThreadRoutine, wArgs, 0, NULL);
		if(wHandleConnectionThread==NULL)
		{
			throw ExceptionReseau("Erreur lors de la creation du thread de connection du socket pour l'adresse " + pSocket->getAdresseDestination(), NULL);
		}
		WaitForSingleObject(mMutexListeSocketsConnection,INFINITE);
		mHandlesThreadConnection[pSocket] = wHandleConnectionThread;
		ReleaseMutex(mMutexListeSocketsConnection);
	}

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::enleverConnectionThread( Socket* pSocket, bool pSuccess )
///
/// Methode pour terminer la reconnection dans un thread de connection.
/// Si pSuccess est True, on remet le socket dans la liste a ecouter
///
/// @param[in] Socket* pSocket  : Socket qui vient de terminer la connection
/// @param[in] bool pSuccess    : Bool pour dire si la reconnectiona  reussie
/// 
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::enleverConnectionThread( SPSocket pSocket, bool pSuccess )
{
	WaitForSingleObject(mMutexListeSocketsConnection,INFINITE);
	mHandlesThreadConnection.erase(mHandlesThreadConnection.find(pSocket));
	ReleaseMutex(mMutexListeSocketsConnection);
    if(!pSuccess)
    {
        GestionnaireReseau::obtenirInstance()->removeSocket(pSocket);
    }
    else
    {
        // Si la reconnection a reussie, on le remet dans la liste des sockets a ecouter
        ajouterSocketEcoute(pSocket);
    }
}



///////////////////////////////////////////////////
// ROUTINES DE THREADS
///////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
///
/// @fn void* CommunicateurReseau::sendingThreadRoutine( void *arg )
///
/// Routine d'execution du thread d'envoie
///
/// @param[in] void *arg  : arguments a passer au thread
/// 
/// @return void*
///
////////////////////////////////////////////////////////////////////////
void* CommunicateurReseau::sendingThreadRoutine( void *arg )
{
	CommunicateurReseau* wCommunicateurReseau = (CommunicateurReseau*) arg;
	QueueThreadSafe<PaquetAEnvoyer*>* listeAEnvoyer = wCommunicateurReseau->getSendingList();
	PacketBuilder wPacketBuilder;

	PaquetAEnvoyer* wPaquetAEnvoyer = NULL;
	while (true)
	{
		if(!listeAEnvoyer->empty())
		{
			 // Queue, alors ordre ok, pop enleve le premier element et le retourne
			// Aller chercher le Paquet et l'envoyer
			if(listeAEnvoyer->pop(wPaquetAEnvoyer) && wPaquetAEnvoyer)
			{
				SPSocket wSocket = wPaquetAEnvoyer->socket;

				ConnectionState wConnectionState = wSocket->getConnectionState();
				if(wConnectionState == CONNECTING || wConnectionState == NOT_CONNECTED)
				{
					// Socket probablement en attente de se faire connecter (on ne fait rien et on le met a la fin de la queue)
					listeAEnvoyer->push(wPaquetAEnvoyer);
					continue;
				}

				try
				{
					PacketHandler* wPacketHandler = GestionnaireReseau::obtenirInstance()->getPacketHandler(wPaquetAEnvoyer->paquet->getOperation());

					// On utilise le handler pour convertir le paquet en suite de char et l'envoyer
					wPacketBuilder.clearBuffer(); // On s'assure que le buffer est vide
					wPacketHandler->handlePacketPreparation(wPaquetAEnvoyer->paquet, wPacketBuilder);


					// On essaie d'envoyer la chaine, envoi bloquant
					wSocket->send( wPacketBuilder.getPacketString(),  wPacketBuilder.getPacketLength(), true);

				}
				catch(ExceptionReseauSocketDeconnecte& )
				{
					// Pas capable d'envoyer le paquet car le Socket est brise
					// On drop le paquet et on envoie le Socket se faire connecter
					wSocket->disconnect();
					if(GestionnaireReseau::getNetworkMode() == CLIENT)
					{
						// Si c'est un client, on essaie de reconnecter
						wCommunicateurReseau->demarrerConnectionThread(wSocket);
						wCommunicateurReseau->supprimerEcouteSocket(wSocket);
					}
					else
					{
						// Si c'est un serveur, on ne veut pas reconnecter le socket. Le client va se reconnecter et le serveur creera un nouveau socket
						GestionnaireReseau::obtenirInstance()->removeSocket(wSocket);
						break; // On doit enlever le socket globalement, impossible de recuperer de ca
					}

				}
				catch(ExceptionReseauTimeout&)
				{
					// Pas capable d'envoyer a cause de Timeout
					// On fait la meme chose que la deconnection pour l'instant

					// Pas capable d'envoyer le paquet car le Socket est brise
					// On drop le paquet et on envoie le Socket se faire connecter
					wSocket->disconnect();
					if(GestionnaireReseau::getNetworkMode() == CLIENT)
					{
						// Si c'est un client, on essaie de reconnecter
						wCommunicateurReseau->demarrerConnectionThread(wSocket);
						wCommunicateurReseau->supprimerEcouteSocket(wSocket);
					}
					else
					{
						// Si c'est un serveur, on ne veut pas reconnecter le socket. Le client va se reconnecter et le serveur creera un nouveau socket
						GestionnaireReseau::obtenirInstance()->removeSocket(wSocket);
						break; // On doit enlever le socket globalement, impossible de recuperer de ca
					}

				}
				catch(ExceptionReseau&)
				{
					// Probleme, on ne sait pas c'est quoi, on drop le paquet
				}

				wPaquetAEnvoyer->paquet->removeAssociatedQuery();
				delete wPaquetAEnvoyer;
			}

			Sleep(10); // A reduire pour le release
		}

	}
	
	// On termine le thread
	ExitThread(EXIT_SUCCESS);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void* CommunicateurReseau::receivingThreadRoutine( void *arg )
///
/// Routine d'execution du thread de reception
///
/// @param[in] void *arg  : arguments a passer au thread
/// 
/// @return void*
///
////////////////////////////////////////////////////////////////////////
void* CommunicateurReseau::receivingThreadRoutine( void *arg )
{
	CommunicateurReseau* wCommunicateurReseau = (CommunicateurReseau*) arg;
	uint8_t readBuffer[GestionnaireReseau::TAILLE_BUFFER_RECEPTION_ENVOI];
    PacketReader wPacketReader;
	while(true)
	{
		
		std::list<SPSocket>::const_iterator it = wCommunicateurReseau->getFirstSocketEcoute();
		std::list<SPSocket>::const_iterator end = wCommunicateurReseau->getEndSocketEcoute();
		for(;it!=end; )
		{
			SPSocket wSocket = (*it);
			if(wSocket == 0) 
			{
				// Bad socket, on le supprime et on doit recommencer la boucle car l'iterateur n'est plus valide
				it = wCommunicateurReseau->supprimerEcouteSocket(it);
                continue;
			}

			// Verifier si le socket est connecte
            ConnectionState wState = wSocket->getConnectionState();
            if(wState == CONNECTED)
            {
                // Read the socket and convert to paquet

                int client_length=(int)sizeof(struct sockaddr_in);
                sockaddr_in sinRemote;
                memset(readBuffer, 0, GestionnaireReseau::TAILLE_BUFFER_RECEPTION_ENVOI);
                int wErrorId = 0;
                try
                {
                    switch(wSocket->getConnectionType())
                    {
                    case UDP:
                        {
                            // On recoit le paquet de controle de taille fixe qui nous donne la taille du paquet suivant
                            wSocket->recvfrom(readBuffer, GestionnaireReseau::TAILLE_PAQUET_CONTROLE_UDP, (struct sockaddr*)&sinRemote, false);
                            std::cout << "readUDP: " << (*it)->getAdresseDestination() << std::endl;
                            // On lit ensuite de la taille contenue dans le paquet de controle
                            // TODO: implementer truc qui envoie un paquet de controle a chaque 1 sec or so pour valider que la connection fonctionne (une fois que l'envoie va etre correctement implemente)

                            // TODO: Implementer lecture



                            break;
                        }
                    case TCP:
                        {
                            size_t wReceivedBytes = -1;
                            while(wReceivedBytes!=0) // On lit tant que le buffer contient quelque chose et qu'on a pas atteint la taille du paquet a recevoir
                            {
                                wReceivedBytes = wSocket->recv(readBuffer, 44);
                                if (wReceivedBytes != 0)
                                {
                                    PacketHandler* wPacketHandler = GestionnaireReseau::obtenirInstance()->getPacketHandler("Base");
                                    wPacketReader.setArrayStart(readBuffer, wReceivedBytes);
                                    HeaderPaquet wPacketHeader = wPacketHandler->handlePacketHeaderReception(wPacketReader);
                                    wPacketReader.setSize(wReceivedBytes);
                                    checkf(wPacketHeader.taillePaquet > wReceivedBytes); // Si trigger, probleme avec lecture du header du paquet (big trouble)
                                    int wTailleRestanteALire = (int) (wPacketHeader.taillePaquet - wReceivedBytes);
                                    while (wTailleRestanteALire>0)
                                    {
                                        wReceivedBytes += wSocket->recv(readBuffer, wTailleRestanteALire);
                                        wPacketReader.append(wReceivedBytes, readBuffer);
                                        wTailleRestanteALire = (int) (wPacketHeader.taillePaquet - wReceivedBytes);
                                    }
                                    wPacketHandler = GestionnaireReseau::obtenirInstance()->getPacketHandler(wPacketHeader.type);
                                    wPacketHandler->handlePacketReceptionSpecific( wPacketReader );
                                }
                                wPacketReader.clearBuffer();
                            }

                            
                            //std::cout << "readTCP: " << (*it)->getAdresseDestination() << std::endl;
                            break;
                        }
                    }
                    
                    // Test
                    
                }
                catch(ExceptionReseauSocketDeconnecte&)
                {
                    wSocket->disconnect();
                    if(GestionnaireReseau::getNetworkMode() == CLIENT)
                    {
                        // Si c'est un client, on essaie de reconnecter
                        wCommunicateurReseau->demarrerConnectionThread(wSocket);
                        it = wCommunicateurReseau->supprimerEcouteSocket(it);
                    }
                    else
                    {
                        // Si c'est un serveur, on ne veut pas reconnecter le socket. Le client va se reconnecter et le serveur creera un nouveau socket
                        GestionnaireReseau::obtenirInstance()->removeSocket(wSocket);
                        break; // On doit enlever le socket globalement, impossible de recuperer de ca
                    }
                    
                    continue;
                }
                catch(ExceptionReseauTimeout&)
                {
                    // Timeout
                    // Just wait and skip this iteration (should not happend here)
                    // Va etre utile pour la detection de connection en UDP
                }
                catch(ExceptionReseau&)
                {
                    // Erreur inconnue
                    GestionnaireReseau::sendMessageToLog("Erreur inconnue survenue a la reception ");

                }

            }
            else if(wState = NOT_CONNECTED)
            {
                // On essaye de demarrer un thread de connection et on le retire de la liste de sockets a ecouter
                wCommunicateurReseau->demarrerConnectionThread(wSocket);
                it = wCommunicateurReseau->supprimerEcouteSocket(it);
                continue;
            }
            else 
            {
                // Don't do anything
            }

			

			++it;
		}
		//std::cout << "loop" << std::endl;

		wCommunicateurReseau->terminerIterationListeSocketEcoute();
		Sleep(10);
	}


	// On termine le thread
	ExitThread(EXIT_SUCCESS);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void* CommunicateurReseau::connectionThreadRoutine( void *arg )
///
/// Routine d'execution du thread de connection
///
/// @param[in] void *arg  : arguments a passer au thread
/// 
/// @return void*
///
////////////////////////////////////////////////////////////////////////
void* CommunicateurReseau::connectionThreadRoutine( void *arg )
{
	ArgsConnectionThread* wArgs = (ArgsConnectionThread*) arg;

	CommunicateurReseau* wCommunicateurReseau = wArgs->communicateurReseau;
	SPSocket wSocket = wArgs->socketAConnecter;

    delete wArgs; // Plus besoin des arguments

    int wNbTentatives = 0;
	
    while(wNbTentatives<200) // 200 tentatives max = 40 sec de connection max
    {
	    // Essayer de connecter le socket
        std::cout << "Tentative de connection a " << wSocket->getAdresseDestination() << std::endl;
        wSocket->init();

        if(wSocket->getConnectionState() == CONNECTED)
        {
            break;
        }
        else
        {
            Sleep(2000); // Pas capable de connecter, on essaye encore dans 2 sec
            ++wNbTentatives;
        }
    }

    wCommunicateurReseau->enleverConnectionThread(wSocket, wNbTentatives != 20);
    

	// On termine le thread
	ExitThread(EXIT_SUCCESS);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void* CommunicateurReseau::connectionTCPServeurThreadRoutine( void *arg )
///
/// Routine d'execution du thread de connection des sockets TCP cote serveur
///
/// @param[in] void *arg  : arguments a passer au thread
/// 
/// @return void*
///
////////////////////////////////////////////////////////////////////////
void * CommunicateurReseau::connectionTCPServeurThreadRoutine( void *arg )
{
    
    // Le thread principal s'occupe du delete sur les sockets
    ArgsConnectionThread* wArgs = (ArgsConnectionThread*) arg;

    CommunicateurReseau* wCommunicateurReseau = wArgs->communicateurReseau;
    SPSocketTCPServer wSocket = (SPSocketTCPServer&)wArgs->socketAConnecter;

    delete wArgs; // Plus besoin des arguments
    

    // call bind + listen on the Socket
    try
    {
        wSocket->bind();
    }
    catch(ExceptionReseau&)
    {
        wSocket->setConnectionState(NOT_CONNECTED);
        throw ExceptionReseau("Appel a bind() impossible. Type: TCP SERVEUR.");
        ExitThread(EXIT_FAILURE);
    }

    // listen()
    try
    {
        wSocket->listen(50);
    }
    catch(ExceptionReseau&)
    {
        wSocket->setConnectionState(NOT_CONNECTED);
        throw ExceptionReseau("Appel a listen() impossible. Type: TCP SERVEUR.");
        ExitThread(EXIT_FAILURE);
    }


    while(true)
    {
        sockaddr_in sinRemote;
        uint32_t nAddrSize = sizeof(sinRemote);
        SPSocket wNewSocket = NULL;
        try
        {
            wNewSocket = wSocket->accept((sockaddr*)&sinRemote, &nAddrSize);

            // On recoit le premier paquet venant de la personne qui se connecte (contient son nom) ne pas bloquer trop longtemps
            char wPlayerName[50];
            if(!wNewSocket->attendreSocket(5)) // On donne 5 sec au client pour envoyer son nom, sinon on le rejette
            {
                throw ExceptionReseau("Blocage prevenu a la reception du PlayerName lors de la connection");
            }
            wNewSocket->recv((uint8_t*) &wPlayerName, 50, true);
            // On verifie que le user n'est pas deja connecte
            if(GestionnaireReseau::obtenirInstance()->getSocket(wPlayerName, TCP) == NULL)
            {
                // On envoit un message de confirmation pour dire que la conenction est acceptee
                char wMessageAcceptation[2] = "1";
                wNewSocket->send((uint8_t*) &wMessageAcceptation, 2, true);

                // On ajoute le nouveau socket au gestionnaire reseau (avec son nom obtenu dans le paquet)
                GestionnaireReseau::obtenirInstance()->saveSocket(std::string(wPlayerName), wNewSocket);
				wNewSocket->setConnectionState(CONNECTED);

				// Ne pas mettre avant de save() sinon il le connait pas le nom su player
				//wNewSocket->attach(GestionnaireReseau::getObserverSocketServer());
            }
            else
            {
                // Sinon on ne connecte pas et on delete le socket
                // On envoit un message de confirmation pour dire que la conenction est acceptee
                char wMessageRefus[2] = "0";
                wNewSocket->send((uint8_t*)&wMessageRefus, 2, true);

                wNewSocket->disconnect();
                wNewSocket = 0;
            }

        }
        catch(ExceptionReseau&)
        {
            try // On essaie d'envoyer un message de refus au client (try pcq il peut etre deconnecte et le send va fail)
            {
                char wMessageRefus[2] = "0";
                wNewSocket->send((uint8_t*) &wMessageRefus, 2, true);
            }
            catch(ExceptionReseau&){}
            wNewSocket->disconnect();
            wNewSocket = 0;
        }

        
        
    }


    // On termine le thread
    ExitThread(EXIT_SUCCESS);
}












///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

