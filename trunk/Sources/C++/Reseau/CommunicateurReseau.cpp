//////////////////////////////////////////////////////////////////////////////
/// @file CommunicateurReseau.cpp
/// @author Mathieu Parent
/// @date 2013-01-08
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "CommunicateurReseau.h"
#include "ExceptionsReseau/ExceptionReseau.h"
#include <iostream>
#include "GestionnaireReseau.h"
#include "Paquets/Paquet.h"
#include "ExceptionsReseau/ExceptionReseauSocketDeconnecte.h"
#include "ExceptionsReseau/ExceptionReseauTimeout.h"
#include "ExceptionsReseau/ExceptionReseauParametreInvalide.h"
#include "SocketTCPServeur.h"
#include "PacketBuilder.h"
#include "PaquetHandlers/PacketHandler.h"
#include "Paquets/Paquet.h"
#include "PacketReader.h"
#include "GestionnaireReseau.h"
#include "Utilitaire.h"
#include <sstream>
#include <time.h>
#include "../ServeurMaitre/ControllerServeurMaitre.h"

// Taille maximale des buffers pour l<envoie et la reception
unsigned int CommunicateurReseau::maxBufferSize = 5000;

struct ArgsConnectionThread {
	CommunicateurReseau* communicateurReseau;
	SPSocket socketAConnecter;
};

void CommunicateurReseau::envoyerPaquetSync( Paquet* wPaquetAEnvoyer, SPSocket wSocket )
{
    PacketBuilder wPacketBuilder;

    PacketHandler* wPacketHandler = GestionnaireReseau::obtenirInstance()->getPacketHandler(wPaquetAEnvoyer->getOperation());

    // On utilise le handler pour convertir le paquet en suite de char et l'envoyer
    wPacketHandler->handlePacketPreparation(wPaquetAEnvoyer, wPacketBuilder);


    // On essaie d'envoyer la chaine, envoi bloquant
    uint32_t wTaille = wSocket->send( wPacketBuilder.getPacketString(),  wPacketBuilder.getPacketLength(), true);

}


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

    FacadePortability::createMutex(mMutexListeSocketsEcoute);
    FacadePortability::createMutex(mMutexListeSocketsConnection);
    FacadePortability::createSemaphore(mHandleSemaphoreContentSend, 0, maxBufferSize);
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

    for(auto it = mHandlesThreadConnectionTCPServeur.begin(); it!=mHandlesThreadConnectionTCPServeur.end(); ++it)
    {
        // Quitter les threads
        FacadePortability::terminateThread(*it);
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
	FacadePortability::releaseSemaphore(mHandleSemaphoreContentSend);
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
	FacadePortability::createThread(mHandleThreadEnvoie, sendingThreadRoutine, this);
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
    FacadePortability::createThread(mHandleThreadReception, receivingThreadRoutine, this);
	if(mHandleThreadReception==NULL)
	{
		throw ExceptionReseau("Erreur lors de la creation du thread de reception", NULL);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::demarrerThreadsConnectionServeur()
///
/// Methode pour demarrer les threads de connection TCP cote serveur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::demarrerThreadsConnectionServeur()
{
    std::list<std::string> wIPList;
    GestionnaireReseau::obtenirInstance()->getListeAdressesIPLocales(wIPList);

    for(std::list<std::string>::iterator it = wIPList.begin(); it!=wIPList.end(); ++it)
    {
        ArgsConnectionThread* wArgs = new ArgsConnectionThread;
        wArgs->communicateurReseau = this;
        wArgs->socketAConnecter = SPSocketTCPServer(new SocketTCPServeur((*it), GestionnaireReseau::communicationPort, TCP));

        HANDLE_THREAD wThread;
        FacadePortability::createThread(wThread, connectionTCPServeurThreadRoutine, wArgs);
        if(wThread==NULL)
        {
            throw ExceptionReseau("Erreur lors de la creation du thread de connection TCP serveur", NULL);
        }
        else
        {
            // On ajoute le handle a la liste
            mHandlesThreadConnectionTCPServeur.push_back(wThread);
        }
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::demarrerThreadsReceptionUDPClientLourd()
///
/// Methode qui demarre 1 thread pour la reception des messages UDP du client lourd
///
/// @return void*
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::demarrerThreadsReceptionUDPClientLourd()
{

    ArgsConnectionThread* wArgs = new ArgsConnectionThread;
    wArgs->communicateurReseau = this;
    wArgs->socketAConnecter = SPSocket(new Socket("0.0.0.0", GestionnaireReseau::communicationUDPPortClientLourd, UDP));

    HANDLE_THREAD wThread;
    FacadePortability::createThread(wThread, receivingUDPThreadRoutine, wArgs);
    if(wThread==NULL)
    {
        throw ExceptionReseau("Erreur lors de la creation du thread de reception UDP", NULL);
    }
    else
    {
        // On ajoute le handle a la liste
        mHandleThreadReceptionUDP = wThread;
    }

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void CommunicateurReseau::demarrerThreadsReceptionUDPServeurJeu()
///
/// Methode qui demarre 1 thread pour la reception des messages UDP du serveur jeu
///
/// @return void*
///
////////////////////////////////////////////////////////////////////////
void CommunicateurReseau::demarrerThreadsReceptionUDPServeurJeu()
{

    ArgsConnectionThread* wArgs = new ArgsConnectionThread;
    wArgs->communicateurReseau = this;
    wArgs->socketAConnecter = SPSocket(new Socket("0.0.0.0", GestionnaireReseau::communicationUDPPortServeurJeu, UDP));

    HANDLE_THREAD wThread;
    FacadePortability::createThread(wThread, receivingUDPThreadRoutine, wArgs);
    if(wThread==NULL)
    {
        throw ExceptionReseau("Erreur lors de la creation du thread de reception UDP", NULL);
    }
    else
    {
        // On ajoute le handle a la liste
        mHandleThreadReceptionUDP = wThread;
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
    FacadePortability::takeMutex(mMutexListeSocketsEcoute);
	mListeSocketsEcoute.push_back(pSocket);
	FacadePortability::releaseMutex(mMutexListeSocketsEcoute);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn std::list<Socket*>::iterator CommunicateurReseau::getFirstSocketEcoute() const
///
/// Methode pour obtenir un iterateur sur la liste de Sockets a ecouter
/// Elle prend egalement un mutex sur cette liste.
/// Il faut donc absolument appeler la methode terminerIterationListeSocketEcoute() pour liberer le mutex
///
/// @return std::list<Socket*>::iterator  : Iterateur sur le debut de la liste de sockets a ecouter
///
////////////////////////////////////////////////////////////////////////
std::list<SPSocket>::iterator CommunicateurReseau::getFirstSocketEcoute()
{
	FacadePortability::takeMutex(mMutexListeSocketsEcoute);
	return mListeSocketsEcoute.begin();
}



////////////////////////////////////////////////////////////////////////
///
/// @fn std::list<Socket*>::iterator CommunicateurReseau::getEndSocketEcoute() const
///
/// Methode pour obtenir un iterateur sur le dernier element de la liste de sockets a ecouter
///
/// @return std::list<Socket*>::iterator  : Iterateur sur le dernier element de la liste de sockets a ecouter
///
////////////////////////////////////////////////////////////////////////
std::list<SPSocket>::iterator CommunicateurReseau::getEndSocketEcoute()
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
	FacadePortability::releaseMutex(mMutexListeSocketsEcoute);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn std::list<Socket*>::iterator CommunicateurReseau::supprimerEcouteSocket( const std::list<Socket*>::iterator& pIterateur )
///
/// Methode pour supprimer un socket a ecouter
///
/// @return std::list<Socket*>::iterator  : Nouvel iterateur pour continuer la boucle si necessaire
///
////////////////////////////////////////////////////////////////////////
std::list<SPSocket>::iterator CommunicateurReseau::supprimerEcouteSocket( const std::list<SPSocket>::iterator& pIterateur )
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
    FacadePortability::takeMutex(mMutexListeSocketsEcoute);
    for(std::list<SPSocket>::iterator it = mListeSocketsEcoute.begin(); it!=mListeSocketsEcoute.end(); ++it)
    {
        if((*it) == pSocket)
        {
            supprimerEcouteSocket(it);
            break;
        }
    }
    FacadePortability::releaseMutex(mMutexListeSocketsEcoute);
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
		HANDLE_THREAD wConnectionThread;
		FacadePortability::createThread(wConnectionThread, connectionThreadRoutine, wArgs);
		if(wConnectionThread==NULL)
		{
			throw ExceptionReseau("Erreur lors de la creation du thread de connection du socket pour l'adresse " + pSocket->getAdresseDestination(), NULL);
		}
		FacadePortability::takeMutex(mMutexListeSocketsConnection);
		mHandlesThreadConnection[pSocket] = wConnectionThread;
		FacadePortability::createMutex(mMutexListeSocketsConnection);
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
    FacadePortability::takeMutex(mMutexListeSocketsConnection);
	mHandlesThreadConnection.erase(mHandlesThreadConnection.find(pSocket));
    FacadePortability::releaseMutex(mMutexListeSocketsConnection);
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
    int nbBytesEnvoye = 0;
	PaquetAEnvoyer* wPaquetAEnvoyer = NULL;
	while (true)
	{
        FacadePortability::takeSemaphore(wCommunicateurReseau->mHandleSemaphoreContentSend);
        if(!listeAEnvoyer->empty())
		{
			 // Queue, alors ordre ok, pop enleve le premier element et le retourne
			// Aller chercher le Paquet et l'envoyer
			if(listeAEnvoyer->pop(wPaquetAEnvoyer) && wPaquetAEnvoyer)
			{
                Paquet* wPaquet = wPaquetAEnvoyer->paquet;

                // If permet d'eviter ce code 99% du temps
                if(wPaquet->getLatency()!=0)
                {
                    clock_t wElapsed = clock()-wPaquet->getLastTick();
                    clock_t wTotalLatency = wPaquet->getLatency();
                    if(wElapsed < wTotalLatency)
                    {
                        // Latence pas encore atteinte
                        wPaquet->setLatency(wTotalLatency-wElapsed);
                        wPaquet->setLastTick(clock());
                        // On remet dans la queue
                        if(listeAEnvoyer->push(wPaquetAEnvoyer))
                        {
                            FacadePortability::releaseSemaphore(wCommunicateurReseau->mHandleSemaphoreContentSend);
                        }
                        continue;
                    }
                    // Sinon on continue car le temps est atteint et on envoie le paquet
                }

				SPSocket wSocket = wPaquetAEnvoyer->socket;

                // Fix crash
                if(!wSocket)
                {
                    continue; // Si le socket est invalide, on drop le paquet
                }
				ConnectionState wConnectionState = wSocket->getConnectionState();
				if(wConnectionState == CONNECTING || wConnectionState == NOT_CONNECTED)
                {
                    if (wConnectionState == CONNECTING)
                    {
					    // Socket probablement en attente de se faire connecter (on ne fait rien et on le met a la fin de la queue)
					    if(listeAEnvoyer->push(wPaquetAEnvoyer))
                        {
                            FacadePortability::releaseSemaphore(wCommunicateurReseau->mHandleSemaphoreContentSend);
                        }
                    }
                    continue;
				}

				try
				{
					PacketHandler* wPacketHandler = GestionnaireReseau::obtenirInstance()->getPacketHandler(wPaquetAEnvoyer->paquet->getOperation());

					// On utilise le handler pour convertir le paquet en suite de char et l'envoyer
					wPacketBuilder.clearBuffer(); // On s'assure que le buffer est vide
					wPacketHandler->handlePacketPreparation(wPaquet, wPacketBuilder);


					// On essaie d'envoyer la chaine, envoi bloquant
                    wSocket->send( wPacketBuilder.getPacketString(),  wPacketBuilder.getPacketLength(), true);

				}
				catch(ExceptionReseauSocketDeconnecte& )
				{
					// Pas capable d'envoyer le paquet car le Socket est brise
					// On drop le paquet et on envoie le Socket se faire connecter
					wSocket->disconnect();
                    GestionnaireReseau::obtenirInstance()->getController()->handleDisconnectDetection(wSocket);
                    break;
				}
				catch(ExceptionReseauTimeout&)
				{
					// Pas capable d'envoyer a cause de Timeout
					// On fait la meme chose que la deconnection pour l'instant

					// Pas capable d'envoyer le paquet car le Socket est brise
					// On drop le paquet et on envoie le Socket se faire connecter
					wSocket->disconnect();
                    GestionnaireReseau::obtenirInstance()->getController()->handleDisconnectDetection(wSocket);
                    break;
				}
				catch(ExceptionReseau&)
				{
					// Probleme, on ne sait pas c'est quoi, on drop le paquet
				}

				wPaquetAEnvoyer->paquet->removeAssociatedQuery();
				delete wPaquetAEnvoyer;
			}
		}
	}

	// On termine le thread
	return 0;
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
		auto it = wCommunicateurReseau->getFirstSocketEcoute();
		auto end = wCommunicateurReseau->getEndSocketEcoute();
		for(;it!=end; )
		{
            bool breakLoop = false;
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
                            //std::cout << "readUDP: " << (*it)->getAdresseDestination() << std::endl;
                            // On lit ensuite de la taille contenue dans le paquet de controle
                            // TODO: implementer truc qui envoie un paquet de controle a chaque 1 sec or so pour valider que la connection fonctionne (une fois que l'envoie va etre correctement implemente)

                            // TODO: Implementer lecture



                            break;
                        }
                    case TCP:
                        {
                            int wReceivedBytes = -1;
                            while(wReceivedBytes!=0) // On lit tant que le buffer contient quelque chose et qu'on a pas atteint la taille du paquet a recevoir
                            {
                                wReceivedBytes = wSocket->recv(readBuffer, getPacketHeaderSize()); // Pas besoin de lui passer d'arguments
                                if (wReceivedBytes != 0)
                                {

                                    wPacketReader.setArrayStart(readBuffer, wReceivedBytes);
                                    HeaderPaquet wPacketHeader = PacketHandler::handlePacketHeaderReception(wPacketReader);
                                    wPacketReader.setSize(wReceivedBytes);
                                    bool wTaillePaquetValide = wPacketHeader.taillePaquet > wReceivedBytes;
                                    // Ne devrait pas planter, mais laisser une trace dans le log
                                    //checkf(wTaillePaquetValide, "Probleme avec lecture du header du paquet"); // Si trigger, probleme avec lecture du header du paquet (big trouble)
                                    if( !wTaillePaquetValide )
                                    {
                                        throw ExceptionReseauParametreInvalide("La taille du paquet reçu était inférieure à la taille du header des paquets");
                                    }
                                    int wTailleRestanteALire = (int) (wPacketHeader.taillePaquet - wReceivedBytes);
                                    int wNbTries = 0;
                                    while (wTailleRestanteALire>0)
                                    {
                                        wReceivedBytes += wSocket->recv(readBuffer, wTailleRestanteALire);
                                        wPacketReader.append(wReceivedBytes, readBuffer);
                                        wTailleRestanteALire = (int) (wPacketHeader.taillePaquet - wReceivedBytes);
                                        ++wNbTries;
                                        if(wNbTries > 100) // Prevent infinite loop
                                        {
                                            throw ExceptionReseau("Erreur de reception du message. Loop infinie");
                                        }
                                    }
                                    PacketHandler* wPacketHandler = GestionnaireReseau::obtenirInstance()->getPacketHandler(wPacketHeader.type);
                                    wPacketHandler->handlePacketReceptionSpecific( wPacketReader , GestionnaireReseau::obtenirInstance()->getController()->getRunnable(wPacketHeader.type) );
                                }
                                wPacketReader.clearBuffer();
                            }
                            break;
                        }
                    }
                }
                catch(ExceptionReseauSocketDeconnecte&)
                {
                    wSocket->disconnect();
                    wCommunicateurReseau->terminerIterationListeSocketEcoute();
                    GestionnaireReseau::obtenirInstance()->getController()->handleDisconnectDetection(wSocket);
                    breakLoop = true;
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
            else if(wState == NOT_CONNECTED)
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

            if(breakLoop)
            {
                break;
            }
            ++it;
        }

        wCommunicateurReseau->terminerIterationListeSocketEcoute();
        FacadePortability::sleep(1);
	}


	// On termine le thread
	return 0;
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
    GestionnaireReseau::obtenirInstance()->transmitEvent(RECONNECTION_IN_PROGRESS);

    static const int TOTAL_TENTATIVE = 10;
    bool connectionSuccessful = false;
    bool tryConnection = true;
    wSocket->setConnectionState(CONNECTING);

    while(tryConnection)
    {
	    // Essayer de connecter le socket
        switch(wSocket->initClient())
        {
        case CONNECTED:
            tryConnection = false;
            connectionSuccessful = true;
            break;
        case CONNECTING:
            ++wNbTentatives;
            // on tente de ce reconnecter s'il reste des tentatives
            tryConnection = wNbTentatives < TOTAL_TENTATIVE;
            if(tryConnection)
            {
                FacadePortability::sleep(1000); // Pas capable de connecter, on essaye encore dans 1 sec
            }
            break;
        case NOT_CONNECTED:
        default:
            // probleme de logique de connection alors on arrete
            tryConnection = false;
            break;
        }
    }

    wSocket->setConnectionState(connectionSuccessful ? CONNECTED : NOT_CONNECTED);

    // cas ou il n'y a plus de tentative possible
    if(wNbTentatives == TOTAL_TENTATIVE)
    {
        GestionnaireReseau::obtenirInstance()->transmitEvent(RECONNECTION_TIMEOUT);
    }
    else if(connectionSuccessful)
    {
        wSocket->removeCancelFlag();
        GestionnaireReseau::obtenirInstance()->transmitEvent(USER_CONNECTED);
    }
    // sinon on n'envoi pas d'event, car init s'en ait charger dans le cas d'une erreur de logique

    wCommunicateurReseau->enleverConnectionThread(wSocket, connectionSuccessful);

	// On termine le thread
	return 0;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void* CommunicateurReseau::connectionServeurThreadRoutine( void *arg )
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
        FacadePortability::exitThread(EXIT_FAILURE);
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
        FacadePortability::exitThread(EXIT_FAILURE);
    }


    while(true)
    {
        sockaddr_in sinRemote;
        uint32_t nAddrSize = sizeof(sinRemote);
        SPSocket wNewSocket = NULL;
        int wMessageConfirmation = 0;
        std::stringstream ss;
        try
        {
            wNewSocket = wSocket->accept((sockaddr*)&sinRemote, &nAddrSize);

            // On recoit le premier paquet venant de la personne qui se connecte (contient son nom) ne pas bloquer trop longtemps
            char wPlayerName[50];
            if(!wNewSocket->attendreSocket(5)) // On donne 5 sec au client pour envoyer son nom, sinon on le rejette
            {
                throw ExceptionReseauTimeout("Blocage prevenu a la reception du PlayerName lors de la connection");
            }
            wNewSocket->recv((uint8_t*) &wPlayerName, 50, true);

            // On verifie que le nom ne contient pas d'espaces et n'est pas vide
            if(strlen(wPlayerName) == 0 || GestionnaireReseau::obtenirInstance()->getPlayerName() == wPlayerName || !GestionnaireReseau::obtenirInstance()->validerUsername(std::string(wPlayerName)))
            {
                // On envoit un message de confirmation pour dire que la conenction est refusee
                // Send connection state message
                char ssAuth[4];
                uint32_t len = sprintf_s(ssAuth,"%d",INVALID_USERNAME);
                wNewSocket->send((uint8_t*)ssAuth, len, true);

                wNewSocket->disconnect();
                wNewSocket = 0;
            }
            else
            {
                // On verifie que le user n'est pas deja connecte
                if(GestionnaireReseau::obtenirInstance()->getSocket(wPlayerName, TCP) == NULL)
                {
                    std::string pSocketIdentifier = wPlayerName;
                    // On verifie finalement l'authentification
                    if(GestionnaireReseau::obtenirInstance()->requireAuthentification())
                    {
                        // Si il y a une authentification a faire, il faut demander au client d'envoyer le mot de passe
                        char wPassword[50];
                        char ssAuth[4];
                        uint32_t len = sprintf_s(ssAuth,"%d",SEND_PASSWORD_REQUEST);
                        wNewSocket->send((uint8_t*)ssAuth, len, true);

                        if(!wNewSocket->attendreSocket(5)) // On donne 5 sec au client pour envoyer son password, sinon on le rejette
                        {
                            throw ExceptionReseauTimeout("Blocage prevenu a la reception du Password lors de la connection");
                        }
                        wNewSocket->recv((uint8_t*) &wPassword, 50, true);

                        if((pSocketIdentifier = GestionnaireReseau::obtenirInstance()->authenticate(wPlayerName, wPassword)) == "")
                        {
                            // Connexion rejetee
                            wMessageConfirmation = WRONG_PASSWORD;
                            ss.str(std::string());
                            ss.clear();

                            ss << wMessageConfirmation;
                            wNewSocket->send((uint8_t*)ss.str().c_str(), 3, true);

                            wNewSocket->disconnect();
                            wNewSocket = 0;
                            continue; // On ne veut pas retourner un status connected alors on skip le reste de la boucle

                        }

                    }



                    char wMessageConfirmation[4];
                    uint32_t len = sprintf_s(wMessageConfirmation,"%d",USER_CONNECTED);
                    wNewSocket->send((uint8_t*)wMessageConfirmation, len, true);

                    // On ajoute le nouveau socket au gestionnaire reseau (avec son nom obtenu dans le paquet)
                    GestionnaireReseau::obtenirInstance()->saveSocket(pSocketIdentifier, wNewSocket);
                    //GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection(pSocketIdentifier, inet_ntoa(sinRemote.sin_addr), UDP);
                    wNewSocket->setConnectionState(CONNECTED);

                    std::cout << pSocketIdentifier << " Connected" << std::endl;
                    // On envoit un message de confirmation pour dire que la conenction est acceptee
                }
                else
                {
                    // Sinon on ne connecte pas et on delete le socket
                    // On envoit un message de confirmation pour dire que la conenction est refusee
                    char wMessageConfirmation[4];
                    uint32_t len = sprintf_s(wMessageConfirmation,"%d",USER_ALREADY_CONNECTED);
                    wNewSocket->send((uint8_t*)wMessageConfirmation, len, true);

                    wNewSocket->disconnect();
                    wNewSocket = 0;
                }
            }
        }
        catch(ExceptionReseauTimeout&)
        {
            wMessageConfirmation = USER_DID_NOT_SEND_NAME_ON_CONNECTION;
            // Send connection state message
            try
            {
                ss << wMessageConfirmation;
                wNewSocket->send((uint8_t*)ss.str().c_str(), 3, true);
            }
            catch(ExceptionReseau&) {}
            wNewSocket->disconnect();
            wNewSocket = 0;
        }
        catch(ExceptionReseau&)
        {
            wMessageConfirmation = USER_DISCONNECTED;
            // Send connection state message
            try
            {
                ss << wMessageConfirmation;
                wNewSocket->send((uint8_t*)ss.str().c_str(), 3, true);
            }
            catch(ExceptionReseau&) {}
            wNewSocket->disconnect();
            wNewSocket = 0;
        }





    }


    // On termine le thread
    return 0;
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void* CommunicateurReseau::receivingUDPThreadRoutine( void *arg )
///
/// Routine d'execution du thread de de reception UDP
/// 1 Thread pour 1 socket UDP qui est bind a une interface reseau
///
/// @param[in] void *arg  : arguments a passer au thread
///
/// @return void*
///
////////////////////////////////////////////////////////////////////////
void * CommunicateurReseau::receivingUDPThreadRoutine( void *arg )
{
    ArgsConnectionThread* wArgs = (ArgsConnectionThread*) arg;

    SPSocket wSocket = (SPSocket)wArgs->socketAConnecter;
    delete wArgs;

#ifndef __linux__

    uint8_t readBuffer[GestionnaireReseau::TAILLE_BUFFER_RECEPTION_ENVOI];
    PacketReader wPacketReader;

    std::hash_map<std::string, int> wMapNoSequence;



    try
    {
        wSocket->bind();
    }
    catch(ExceptionReseau&)
    {
        GestionnaireReseau::obtenirInstance()->throwExceptionReseau("Bind du socket de reception UDP impossible");
    }

    while(true)
    {

        try
        {
            wPacketReader.clearBuffer();


            sockaddr_in wInAddr;
            int wReceivedBytes = wSocket->recvfrom((uint8_t*)readBuffer, GestionnaireReseau::TAILLE_BUFFER_RECEPTION_ENVOI/*getPacketHeaderSize()*/, (sockaddr*)&wInAddr, true);

            if(wReceivedBytes <= getPacketHeaderSize())
            {
                throw ExceptionReseau("Taille UDP recu inferieure a la taille du header");
            }

            wPacketReader.setArrayStart(readBuffer, getPacketHeaderSize());
            HeaderPaquet wPacketHeader = PacketHandler::handlePacketHeaderReception(wPacketReader);
            wPacketReader.setSize(getPacketHeaderSize());

            std::string wIP = inet_ntoa(wInAddr.sin_addr);
            if(wPacketHeader.numeroPaquet > wMapNoSequence[wIP])
            {
                wPacketReader.append(wReceivedBytes, readBuffer+(sizeof(uint8_t)*getPacketHeaderSize()));

                PacketHandler* wPacketHandler = GestionnaireReseau::obtenirInstance()->getPacketHandler(wPacketHeader.type);
                wPacketHandler->handlePacketReceptionSpecific( wPacketReader , GestionnaireReseau::obtenirInstance()->getController()->getRunnable(wPacketHeader.type) );

                wMapNoSequence[wIP] = wPacketHeader.numeroPaquet;
            }





        }
        catch(ExceptionReseauTimeout&)
        {
            // Timeout
            // Just wait and skip this iteration (should not happend here)
            // Va etre utile pour la detection de connection en UDP
            GestionnaireReseau::sendMessageToLog("Timeout survenue a la reception UDP ");
        }
        catch(ExceptionReseau&)
        {
            // Erreur inconnue
            GestionnaireReseau::sendMessageToLog("Erreur inconnue survenue a la reception UDP");

        }
    }
#endif

    return 0;
}









///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

