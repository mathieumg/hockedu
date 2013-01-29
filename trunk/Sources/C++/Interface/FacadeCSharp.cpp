#include "FacadeCSharp.h"
#include "UsineNoeudMaillet.h"
#include "UsineNoeudRondelle.h"
#include "BancTests.h"
#include "..\Reseau\GestionnaireReseauClientLourd.h"
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include "Reseau\Paquets\PaquetTest.h"
#include "QueueThreadSafe.h"
#include <ctime>

// Test pour l'appel de la DLL depuis le C#
int TestCSCall(int i)
{
    return i*2;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ExecuteUnitTest()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
int ExecuteUnitTest()
{
    UsineNoeudMaillet::bypassLimitePourTest = true;
    UsineNoeudRondelle::bypassLimitePourTest = true;
    bool reussite = BancTests::obtenirInstance()->executer();

    UsineNoeudMaillet::bypassLimitePourTest = false;
    UsineNoeudRondelle::bypassLimitePourTest = false;
    // Visual Studio interprète le code de retour 0 comme une réussite et le code
    // de retour 1 comme un échec. Nous transmettons le code de retour à Java
    // qui le transmet directement comme code de sortie du programme.
    system("pause");
    return reussite ? 0 : 1;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void TestGestionnaireReseau()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void TestGestionnaireReseau()
{
    GestionnaireReseauClientLourd::obtenirInstance();
    
    Socket* wSocket = new Socket("127.0.0.1",5010, TCP);
    GestionnaireReseau::obtenirInstance()->saveSocket("bob", wSocket);

    PaquetTest* wPaquet = (PaquetTest*) GestionnaireReseau::obtenirInstance()->creerPaquet("Test");
    wPaquet->setMessage("SUP C#");
    wPaquet->setInt(666);
    wPaquet->setFloat(666.666f);

    GestionnaireReseau::obtenirInstance()->envoyerPaquet(wSocket, wPaquet);



}


std::string mName;
void InitDLL(char * pName)
{
    mName = pName;
    GestionnaireReseauClientLourd::obtenirInstance();

    Socket* wSocket = new Socket("127.0.0.1", 5010, TCP);
    GestionnaireReseau::obtenirInstance()->saveSocket(mName, wSocket);

}


void SendMessageDLL(char * pMessage)
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatMessage");
    wPaquet->setMessage(pMessage);
    wPaquet->setIsTargetGroup(true);
    wPaquet->setGroupName("groupe");
    wPaquet->setTimestamp(time(0));
    wPaquet->setOrigin(mName);

    Socket* wSocket = GestionnaireReseau::obtenirInstance()->getSocket(mName,TCP);

    GestionnaireReseau::obtenirInstance()->envoyerPaquet(wSocket, wPaquet);
}

void GetMessageDLL(char * pMessage , int* pBufferSize)
{
    QueueThreadSafe<std::string>& queue = GestionnaireReseauClientLourd::obtenirInstance()->mMessages;
    if(!queue.empty())
	{
		std::string message = queue.pop();
		int i = 0;
		for(i=0; i<(int)message.size(); ++i)
		{
			if(i < *pBufferSize)
			{
				pMessage[i] = message[i];
			}
			else
			{
				--i;
				break;
			}
		}
		pMessage[i] = 0;
	}
}
