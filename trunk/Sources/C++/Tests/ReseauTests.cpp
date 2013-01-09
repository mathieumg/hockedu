////////////////////////////////////////////////////////////////////////////////////
/// @file ConfigSceneTest.cpp
/// @author Michael Ferris
/// @date 2012-02-02
/// @version 2.0
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "ReseauTests.h"
#include "..\Reseau\SocketTCP.h"
#include "..\Reseau\SocketUDP.h"


// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( ReseauTests );

////////////////////////////////////////////////////////////////////////
///
/// @fn void ReseauTests::setUp()
///
/// Effectue l'initialisation préalable à l'exécution de l'ensemble des
/// cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets doivent être construits, il est conseillé de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ReseauTests::setUp()
{
	localIP_ = "127.0.0.1";
	destinationIP_ = "192.168.0.1";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ReseauTests::tearDown()
///
/// Effectue les opérations de finalisation nécessaires suite à l'exécution
/// de l'ensemble des cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets ont été alloués à l'initialisation, ils doivent être
/// désalloués, et il est conseillé de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ReseauTests::tearDown()
{
	

}

////////////////////////////////////////////////////////////////////////
///
/// @fn ReseauTests::testCreationSocketTCP()
///
/// test de la creation d'un socket TCP
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ReseauTests::testCreationSocketTCP()
{
	SocketTCP* socketTCP = new SocketTCP(destinationIP_, 1001);

	// Validate the created socket inside Windows

	SOCKET windowsSocket = socketTCP->getWindowsSocket();
	delete socketTCP;
	// Verify that the handle in Windows is invalid (socket closed)
	char buf[300];
	int returnValue = recv(windowsSocket, buf, 300, 0);
	CPPUNIT_ASSERT (returnValue == WSAENOTSOCK);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn ReseauTests::testCreationSocketUDP()
///
/// test de la creation d'un socket UDP
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ReseauTests::testCreationSocketUDP()
{
	SocketUDP* socketUDP = new SocketUDP(destinationIP_, 1002);
	// Validate the created socket inside Windows



	SOCKET windowsSocket = socketUDP->getWindowsSocket();
	delete socketUDP;
	// Verify that the handle in Windows is invalid (socket closed)
	char buf[300];
	int returnValue = recv(windowsSocket, buf, 300, 0);
	CPPUNIT_ASSERT (returnValue == WSAENOTSOCK);

}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


