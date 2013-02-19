#include "BancTests.h"
#include "QueueThreadSafe.h"
#include <ctime>
#include "FacadeCSharp.h"
#include "FacadeModele.h"
#include "Vue.h"
#include "GestionnaireEvenements.h"
#include "..\Application\RepartiteurActions.h"
#include "..\Jeu\JoueurHumain.h"
#include "..\Reseau\GestionnaireReseauClientLourd.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerChatMessage.h"
#include "..\Reseau\PaquetHandlers\PacketHandlerUserStatus.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetUserStatus.h"
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include "..\Reseau\Paquets\PaquetTest.h"
#include "..\Reseau\ControllerCSharp.h"
#include "UsineNoeud.h"


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

    PaquetTest* wPaquet = (PaquetTest*) GestionnaireReseau::obtenirInstance()->creerPaquet(TEST);
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
    wGestionnaireReseau->initClient();

    // On doit ajouter une nouvelle operation reseau pour que le systeme le connaisse (1 par type de paquet)
    wGestionnaireReseau->ajouterOperationReseau(CHAT_MESSAGE, new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    wGestionnaireReseau->ajouterOperationReseau(USER_STATUS, new PacketHandlerUserStatus, new UsinePaquetUserStatus);
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
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet(CHAT_MESSAGE);
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
    try
    {
        GestionnaireReseau::obtenirInstance()->disconnectClient(pUsername);
    }
    catch(...)
    {
        // nothing to do, it most likely failed because the server isnt responsive
    }
}

void CancelConnection( char* pUsername )
{
    GestionnaireReseau::obtenirInstance()->cancelNewConnection(pUsername);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void initialiserOpenGL( HWND hWnd )
///
/// /*Description*/
///
/// @param[in] HWND hWnd
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void InitOpenGL( HWND hWnd )
{
    FacadeModele::getInstance()->initialiserOpenGL(hWnd);

    SPJoueurAbstrait joueurHumain = SPJoueurAbstrait(new JoueurHumain("Joueur 2"));
    FacadeModele::getInstance()->modifierAdversaire(joueurHumain);
    //FacadeModele::getInstance()->passageModeEdition();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void renderOpenGL()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RenderOpenGL()
{
    FacadeModele::getInstance()->afficher();
}


void WindowResized(int largeur, int hauteur)
{
    FacadeModele::getInstance()->obtenirVue()->redimensionnerFenetre(
        Vecteur2i(0,0),
        Vecteur2i(largeur, hauteur)
        );
    FacadeModele::getInstance()->rafraichirFenetre();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void LogicUpdate( float time )
///
/// /*Description*/
///
/// @param[in] float time
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LogicUpdate( float time )
{
    FacadeModele::getInstance()->animer(time);
}


void OnKeyPressed(int key)
{
    GestionnaireEvenements::obtenirInstance()->toucheEnfoncee(EvenementClavier(key));
}


void OnKeyReleased(int key)
{
    GestionnaireEvenements::obtenirInstance()->toucheRelachee(EvenementClavier(key));
}



void OnMousePressed( int x, int y, MouseButtons button)
{
    BoutonSouris type = AUCUN_BOUTON_SOURIS;
    switch(button)
    {
    case Left  : type = BOUTON_SOURIS_GAUCHE; break;
    case Right : type = BOUTON_SOURIS_DROIT; break;
    case Middle: type = BOUTON_SOURIS_MILIEU; break;
    default:break;
    }
    GestionnaireEvenements::obtenirInstance()->sourisEnfoncee(EvenementSouris(Vecteur2i(x,y),type));
}


void OnMouseReleased( int x, int y, MouseButtons button)
{
    BoutonSouris type = AUCUN_BOUTON_SOURIS;
    switch(button)
    {
    case Left  : type = BOUTON_SOURIS_GAUCHE; break;
    case Right : type = BOUTON_SOURIS_DROIT; break;
    case Middle: type = BOUTON_SOURIS_MILIEU; break;
    default:break;
    }
    GestionnaireEvenements::obtenirInstance()->sourisRelachee(EvenementSouris(Vecteur2i(x,y), type));
}



void OnMouseMoved( int x, int y, MouseButtons button )
{
    BoutonSouris type = AUCUN_BOUTON_SOURIS;
    switch(button)
    {
    case Left  : type = BOUTON_SOURIS_GAUCHE; break;
    case Right : type = BOUTON_SOURIS_DROIT; break;
    case Middle: type = BOUTON_SOURIS_MILIEU; break;
    default:break;
    }
    GestionnaireEvenements::obtenirInstance()->sourisDeplacee(EvenementSouris(Vecteur2i(x,y), type));
}

void OnMouseWheelMoved( int deltaRotation )
{
    GestionnaireEvenements::obtenirInstance()->rouletteSouris(EvenementRouletteSouris(-deltaRotation));
}

bool ActionPerformed( char* action )
{
    std::string actionString = action;
    if(actionString == "SUPPRIMER" || actionString == "EDITEUR_NOUVEAU" || actionString == "REINITIALISER_PARTIE")
    {
        // Si on est dans le cas de suppression et qu'il n'y a pas de sélection.
        if(actionString == "SUPPRIMER"  && !FacadeModele::getInstance()->possedeSelection())
            return false;

        checkf(0,"Enleve le check pour les cas une fois que la fenetre de validation est géré par le C#");
    }

    return RepartiteurActions::obtenirInstance()->appelerMethodeAction(actionString);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool IsGamePaused()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool IsGamePaused()
{
    return FacadeModele::getInstance()->estEnPause();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void PauseGame( bool doPause )
///
/// /*Description*/
///
/// @param[in] bool doPause
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void PauseGame( bool doPause )
{
    FacadeModele::getInstance()->modifierEnPause(doPause);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GenerateDefaultField()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GenerateDefaultField()
{
    FacadeModele::getInstance()->creerTerrainParDefaut(FacadeModele::getInstance()->getTerrainModifier());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ValidateField()
///
/// /*Description*/
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool ValidateField()
{
    return FacadeModele::getInstance()->verifierValiditeMap();
}







void initNetwork( ControllerInterface* pController )
{
    GestionnaireReseau::obtenirInstance()->setController(pController);




}

