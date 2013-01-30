#include "UsineNoeudMaillet.h"
#include "UsineNoeudRondelle.h"
#include "BancTests.h"
#include "..\Reseau\GestionnaireReseauClientLourd.h"
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include "Reseau\Paquets\PaquetTest.h"
#include "QueueThreadSafe.h"
#include <ctime>
#include "FacadeCSharp.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatUserStatus.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerChatUserStatus.h"
#include "..\Reseau\ControllerCSharp.h"


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
    
    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("bob", "127.0.0.1", TCP);

    PaquetTest* wPaquet = (PaquetTest*) GestionnaireReseau::obtenirInstance()->creerPaquet("Test");
    wPaquet->setMessage("SUP C#");
    wPaquet->setInt(666);
    wPaquet->setFloat(666.666f);

    GestionnaireReseau::obtenirInstance()->envoyerPaquet("bob", wPaquet,TCP);

}


void InitDLL()
{
    // Creer le controlleur C# dans le gestionnaireReseau
    // Initialisation du GestionnaireReseau
    GestionnaireReseau* wGestionnaireReseau = GestionnaireReseau::obtenirInstance();

    // ajout du controlleur qui va gèrer les événements du réseau et les retransmettre par callback à la vue
    wGestionnaireReseau->setController(new ControllerCSharp);
    wGestionnaireReseau->init();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    wGestionnaireReseau->ajouterOperationReseau("ChatUserStatus", new PacketHandlerChatUserStatus, new UsinePaquetChatUserStatus);
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
    ControllerCSharp* pControler = (ControllerCSharp*)GestionnaireReseau::obtenirInstance()->getController();
    checkf(pControler, "The controler must exist at all time inside the NetworkManager");
    pControler->setMessageReceivedCallBack(callback);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SetEventCallback( EventReceivedCallBack callback )
///
/// Enregistre la callback pour mettre a jour la vue lors d'événement
///
/// @param[in] EventReceivedCallBack callback
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SetEventCallback( EventReceivedCallBack callback )
{
    ControllerCSharp* pControler = (ControllerCSharp*)GestionnaireReseau::obtenirInstance()->getController();
    checkf(pControler, "The controler must exist at all time inside the NetworkManager");
    pControler->setEventReceivedCallBack(callback);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void DisconnectUser( char* pUsername )
///
/// /*Description*/
///
/// @param[in] char * pUsername
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void DisconnectUser( char* pUsername )
{
    GestionnaireReseau::obtenirInstance()->removeSocket(pUsername,TCP);
}
