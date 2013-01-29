#include "UsineNoeudMaillet.h"
#include "UsineNoeudRondelle.h"
#include "BancTests.h"
#include "..\Reseau\GestionnaireReseauClientLourd.h"
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include "Reseau\Paquets\PaquetTest.h"
#include "QueueThreadSafe.h"
#include <ctime>
#include "FacadeCSharp.h"


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
    // Visual Studio interpr�te le code de retour 0 comme une r�ussite et le code
    // de retour 1 comme un �chec. Nous transmettons le code de retour � Java
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
    
    SPSocket wSocket = GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("bob", "127.0.0.1", TCP);

    PaquetTest* wPaquet = (PaquetTest*) GestionnaireReseau::obtenirInstance()->creerPaquet("Test");
    wPaquet->setMessage("SUP C#");
    wPaquet->setInt(666);
    wPaquet->setFloat(666.666f);

    GestionnaireReseau::obtenirInstance()->envoyerPaquet(wSocket, wPaquet);

}


void InitDLL()
{
    // Creer le controlleur C# dans le gestionnaireReseau
    
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void RequestLogin( char* pUsername, char* pIpAdress )
///
/// Envoi une demande pour ce connecter
///
/// @param[in] char * pUsername
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RequestLogin( char* pUsername, char* pIpAdress )
{
    GestionnaireReseauClientLourd::obtenirInstance();
    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection(pUsername,pIpAdress,TCP);
}

void SendMessageDLL(char * pUsername, char * pMessage)
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet("ChatMessage");
    wPaquet->setMessage(pMessage);
    wPaquet->setIsTargetGroup(true);
    wPaquet->setGroupName("groupe");
    wPaquet->setTimestamp(time(0));
    wPaquet->setOrigin(pUsername);

    try
    {
        GestionnaireReseau::obtenirInstance()->envoyerPaquet(pUsername, wPaquet,TCP);
    }
    catch(...)
    {
        wPaquet->removeAssociatedQuery();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SetMessageCallback( MessageReceivedCallBack callback )
///
/// Enregistre la callback pour mettre a jour la vue lors de nouveau message
///
/// @param[in] MessageReceivedCallBack callback
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SetMessageCallback( MessageReceivedCallBack callback )
{
    GestionnaireReseauClientLourd::obtenirInstance()->setMessageReceivedCallBack(callback);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SetEventCallback( EventReceivedCallBack callback )
///
/// Enregistre la callback pour mettre a jour la vue lors d'�v�nement
///
/// @param[in] EventReceivedCallBack callback
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SetEventCallback( EventReceivedCallBack callback )
{

}
