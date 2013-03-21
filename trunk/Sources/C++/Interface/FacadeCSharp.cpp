#include "FacadeCSharp.h"

#include <iostream>
#include "ExceptionJeu.h"
#include "Terrain.h"
#include "Tournoi.h"
#include "GestionnaireHUD.h"
#include "..\reseau\UsinePaquets\UsinePaquetMaillet.h"
#include "..\reseau\PaquetHandlers\PacketHandler.h"
#include "..\Reseau\Paquets\PaquetGameCreation.h"
#include "..\Reseau\UsinePaquets\UsinePaquetGameCreation.h"
#include "..\reseau\Paquets\PaquetGameConnection.h"
#include "..\reseau\UsinePaquets\UsinePaquetGameConnection.h"
#include "..\reseau\UsinePaquets\UsinePaquetRondelle.h"
#include "VisitorGatherProperties.h"

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
    bool reussite = BancTests::obtenirInstance()->executer();
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
    wGestionnaireReseau->ajouterOperationReseau(MAILLET, new PacketHandlerMaillet, new UsinePaquetMaillet);
    wGestionnaireReseau->ajouterOperationReseau(GAME_CREATION_REQUEST, new PacketHandlerGameCreation, new UsinePaquetGameCreation);
    wGestionnaireReseau->ajouterOperationReseau(GAME_CONNECTION, new PacketHandlerGameConnection, new UsinePaquetGameConnection);
    wGestionnaireReseau->ajouterOperationReseau(RONDELLE, new PacketHandlerRondelle, new UsinePaquetRondelle);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void RequestLogin( char* pUsername, char* pPassword, char* pIpAdress )
///
/// Envoi une demande pour ce connecter
///
/// @param[in] char * pUsername
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void RequestLogin( char* pUsername, char* pPassword, char* pIpAdress )
{
    GestionnaireReseauClientLourd::obtenirInstance();
    GestionnaireReseau::obtenirInstance()->setUser(pUsername, pPassword);
    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("MasterServer",pIpAdress,TCP);
//#if MAT_DEBUG_
    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("GameServer",pIpAdress,TCP);
    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("GameServer",pIpAdress,UDP);
//#endif
}

void SendMessageDLL(char * pConnectionId, char* pUsername, char * pMessage)
{
    PaquetChatMessage* wPaquet = (PaquetChatMessage*) GestionnaireReseau::obtenirInstance()->creerPaquet(CHAT_MESSAGE);
    wPaquet->setMessage(pMessage);
    wPaquet->setIsTargetGroup(true);
    wPaquet->setGroupName("groupe");
    wPaquet->setTimestamp(time(0));
    wPaquet->setOrigin(pUsername);

    try
    {
        GestionnaireReseau::obtenirInstance()->envoyerPaquet(pConnectionId, wPaquet,TCP);
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
void DisconnectUser( char* pConnectionId )
{
    try
    {
        GestionnaireReseau::obtenirInstance()->disconnectClient(pConnectionId);
    }
    catch(...)
    {
        // nothing to do, it most likely failed because the server isnt responsive
    }
}

void CancelConnection( char* pConnectionId )
{
    GestionnaireReseau::obtenirInstance()->cancelNewConnection(pConnectionId);
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
    
}


void FreeApplicationMemory(  )
{
    FacadeModele::getInstance()->libererOpenGL();

    // Désinitialisation de la façade.  Le fait de le faire après la
    // désinitialisation du contexte OpenGL aura pour conséquence que la
    // libération des listes d'affichages, par exemple, sera faite une fois que
    // le contexte n'existera plus, et sera donc sans effet.
    FacadeModele::libererInstance();
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
    GestionnaireHUD::obtenirInstance()->setRatio(largeur/(float)hauteur);
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
    GestionnaireEvenements::toucheEnfoncee(EvenementClavier(key));
}


void OnKeyReleased(int key)
{
    GestionnaireEvenements::toucheRelachee(EvenementClavier(key));
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
    GestionnaireEvenements::sourisEnfoncee(EvenementSouris(Vecteur2i(x,y),type));
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
    GestionnaireEvenements::sourisRelachee(EvenementSouris(Vecteur2i(x,y), type));
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
    GestionnaireEvenements::sourisDeplacee(EvenementSouris(Vecteur2i(x,y), type));
}

void OnMouseWheelMoved( int deltaRotation )
{
    GestionnaireEvenements::rouletteSouris(EvenementRouletteSouris(-deltaRotation));
}

bool ActionPerformed( ActionType action )
{
    if(action == ACTION_SUPPRIMER || action == ACTION_EDITEUR_NOUVEAU || action == ACTION_REINITIALISER_PARTIE)
    {
        // Si on est dans le cas de suppression et qu'il n'y a pas de sélection.
        if(action == ACTION_SUPPRIMER  && !FacadeModele::getInstance()->possedeSelection())
            return false;

        //checkf(0,"Enleve le check pour les cas une fois que la fenetre de validation est géré par le C#");
    }

    return RepartiteurActions::obtenirInstance()->appelerMethodeAction(action);
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
/// permet de creer un terrain par defaut pour le mode edition
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void GenerateDefaultField()
{
    FacadeModele::getInstance()->creerTerrainParDefaut();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool ValidateField()
///
/// Perform field validation
///
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool ValidateField()
{
    return FacadeModele::getInstance()->getEditionField()->verifierValidite(true,false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ResetCamera()
///
/// Position the camera at table center
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ResetCamera()
{
    FacadeModele::getInstance()->obtenirVue()->centrerCamera(FacadeModele::getInstance()->getTableWidth());
}

void initNetwork( ControllerInterface* pController )
{
    GestionnaireReseau::obtenirInstance()->setController(pController);
}

void connectServerGame( char* pServerIP )
{
    // Temporary
    GestionnaireReseau::obtenirInstance()->setUser("bob2", "");

    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("GameServer", pServerIP, TCP);
    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("GameServer", pServerIP, UDP);



}

void connectPartieServerGame( int pGameId )
{
    PaquetGameConnection* wPaquet = (PaquetGameConnection*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_CONNECTION);
    wPaquet->setGameId(pGameId);
    GestionnaireReseau::obtenirInstance()->envoyerPaquet("GameServer", wPaquet, TCP);


}

void requestGameCreationServerGame( char* pGameName )
{
    PaquetGameCreation* wPaquet = (PaquetGameCreation*) GestionnaireReseau::obtenirInstance()->creerPaquet(GAME_CREATION_REQUEST);
    wPaquet->setGameName(pGameName);
    GestionnaireReseau::obtenirInstance()->envoyerPaquet("GameServer", wPaquet, TCP);
}

void SaveMap(char* pFileName)
{
    RazerGameUtilities::SaveFieldToFile(std::string(pFileName), *FacadeModele::getInstance()->getEditionField());
}

void LoadMap(char* pFileName)
{
    RazerGameUtilities::LoadFieldFromFile(std::string(pFileName), *FacadeModele::getInstance()->getEditionField());
}

void SetPlayMap(char* pFileName)
{
    FacadeModele::getInstance()->setCurrentMap(std::string(pFileName));
}

void SetSecondPlayer(bool pIsHuman, char* pName)
{
    SPJoueurAbstrait player;

    if(pIsHuman)
    {
        player = SPJoueurHumain(new JoueurHumain);

        // Test seulement
        //player = SPJoueurNetwork(new JoueurNetwork);
    }
    else
    {
        player = FacadeModele::getInstance()->obtenirJoueur(std::string(pName));
    }


    GameManager::obtenirInstance()->setAdversaire(player);
}

void AddPlayer(char* pName, int pSpeed, int pFailProb)
{
    SPJoueurAbstrait joueurVirtuel(new JoueurVirtuel(pName, pSpeed, pFailProb));
    FacadeModele::getInstance()->ajouterJoueur(joueurVirtuel);
}

void RemovePlayer(char* pName)
{
    FacadeModele::getInstance()->supprimerJoueur(std::string(pName));
}

int GetNbrPlayers()
{
    return (int)FacadeModele::getInstance()->obtenirListeNomsJoueurs().size();
}

void GetPlayers(AIProfile* pProfiles, int pNbrProfiles)
{
    ConteneurJoueursTries listeJoueursTries;

    listeJoueursTries = FacadeModele::getInstance()->obtenirListeNomsJoueurs();

    SPJoueurAbstrait joueur;

    int i = 0;


    for(auto iter = listeJoueursTries.begin(); iter != listeJoueursTries.end() && i < pNbrProfiles; ++iter)
	{
        joueur = FacadeModele::getInstance()->obtenirJoueur(*iter);

        if(joueur->obtenirType()==JOUEUR_VIRTUEL)
	    {
		    SPJoueurVirtuel joueurVirtuel = std::dynamic_pointer_cast<JoueurVirtuel>(joueur);

            strcpy_s(pProfiles[i].Name, 255, joueurVirtuel->obtenirNom().c_str());

            pProfiles[i].Speed = joueurVirtuel->obtenirVitesse();
            pProfiles[i].FailProb = joueurVirtuel->obtenirProbabiliteEchec();
            ++i;
	    }
	}
}

void GetKeyboardControl(int* pControls)
{
    pControls[0] = ConfigScene::obtenirInstance()->obtenirToucheHaut();
	pControls[1] = ConfigScene::obtenirInstance()->obtenirToucheBas();
	pControls[2] = ConfigScene::obtenirInstance()->obtenirToucheGauche();
	pControls[3] = ConfigScene::obtenirInstance()->obtenirToucheDroite();
}

void SetKeyboardControl(int* pControls)
{
    ConfigScene::obtenirInstance()->modifierToucheHaut(pControls[0]);
	ConfigScene::obtenirInstance()->modifierToucheBas(pControls[1]);
	ConfigScene::obtenirInstance()->modifierToucheGauche(pControls[2]);
	ConfigScene::obtenirInstance()->modifierToucheDroite(pControls[3]);
}

void PlayRadioSong()
{
    if(SoundFMOD::obtenirInstance()->estEnPause())
	{
		SoundFMOD::obtenirInstance()->togglePlaying();
	}
	else
	{
		SoundFMOD::obtenirInstance()->restartSong();
	}
}

void PauseRadioSong()
{
	SoundFMOD::obtenirInstance()->togglePlaying();
}

void StopRadioSong()
{
	SoundFMOD::obtenirInstance()->stop();
}

void NextRadioSong()
{
	SoundFMOD::obtenirInstance()->next_Song();
}

void PreviousRadioSong()
{
	SoundFMOD::obtenirInstance()->previous_Song();
}

void SetRadioVolume(int pVolume)
{
	SoundFMOD::obtenirInstance()->setPlaylistVolume(pVolume/100.0f);
}

void SetCurrentRadioPlaylist(char* pPlaylist)
{
	SoundFMOD::obtenirInstance()->modifierPlaylistActuelle(std::string(pPlaylist));
}

void GetCurrentRadioPlaylist(char* pPlaylist)
{
    strcpy_s(pPlaylist, 255, SoundFMOD::obtenirInstance()->obtenirNomCanalCourant().c_str());
}



int GetNbrPlaylists()
{
    std::vector<std::string> nomsCanaux =  ConfigScene::obtenirInstance()->obtenirListeCanaux();

    return (int)nomsCanaux.size();
}

void GetRadioPlaylists(char** pPlaylists, int pNbrPlaylists)
{
	std::vector<std::string> nomsCanaux =  ConfigScene::obtenirInstance()->obtenirListeCanaux();

    int i = 0;
	for(auto iter = nomsCanaux.begin(); iter != nomsCanaux.end() && i < pNbrPlaylists; ++iter)
	{
        strcpy_s(pPlaylists[i++], 255, (*iter).c_str());
	}
}

int GetNbrSongs(char* pPlaylist)
{
    NomsPlaylists canal = ConfigScene::obtenirInstance()->obtenirCanal(std::string(pPlaylist));

    return (int)canal.size();
}

void GetPlaylistSongs(char* pPlaylist, char** pSongs, int pNbrSongs)
{
	NomsPlaylists canal = ConfigScene::obtenirInstance()->obtenirCanal(std::string(pPlaylist));

    int i = 0;
    for(auto iter = canal.begin(); iter != canal.end() && i < pNbrSongs; iter++)
	{
        strcpy_s(pSongs[i++], 255, (*iter).c_str());
	}
}

void RemoveRadioPlaylist(char* pPlaylist)
{
	ConfigScene::obtenirInstance()->supprimerCanal(std::string(pPlaylist));
}

void AddRadioPlaylist(char* pPlaylist, char** pSongs, int pNbrSongs)
{
    std::string nomPlaylist = std::string(pPlaylist);

	ConfigScene::obtenirInstance()->ajouterCanal(nomPlaylist);

    for(int i = 0; i < pNbrSongs; ++i)
    {
        ConfigScene::obtenirInstance()->ajouterChanson(nomPlaylist, std::string(pSongs[i]));
    }
}

bool TerrainHasDeletable()
{
    return FacadeModele::getInstance()->getEditionField()->CanSelectedNodeBeDeleted();
}


void BeginNewTournament(char* pTournamentName, char* pMapName, char** pPlayerNames, int pNbrPlayers)
{
	// Players list
	JoueursParticipant players;

	// Fill the players list
	for(int i = 0; i < pNbrPlayers; ++i)
	{
		// Empty name means human player
		if(strlen(pPlayerNames[i]) == 0)
        {
            players.push_back(SPJoueurAbstrait(new JoueurHumain()));
        }
		else // AI player
		{	
			SPJoueurAbstrait jv = FacadeModele::getInstance()->obtenirJoueur(std::string(pPlayerNames[i]));
			if(jv)
			{
				players.push_back(jv);
			}
			else
            {
				players.push_back(SPJoueurAbstrait(new JoueurVirtuel(pPlayerNames[i], 1, 0)));
            }
		}
	}

	// Init tournament
	FacadeModele::getInstance()->initialiserTournoi(players, std::string(pMapName));
	Tournoi* tournoiCpp = FacadeModele::getInstance()->obtenirTournoi();
	tournoiCpp->modifierNom(std::string(pTournamentName));

	// Enregistrement du tournoi
	FacadeModele::getInstance()->enregistrerTournoi(tournoiCpp);
}

void ContinueExistingTournament(char* pTournamentName)
{
    FacadeModele::getInstance()->chargerTournoi(std::string(pTournamentName));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn RazerKey GetSelectedNodeUniqueKey()
///
/// checks if selected nodes are the same type and returns that type
/// if not, return NODE_KEY_NONE
///
///
/// @return RazerKey
///
////////////////////////////////////////////////////////////////////////
RazerKey GetSelectedNodeUniqueKey()
{
    return FacadeModele::getInstance()->getEditionField()->getSelectedNodeUniqueKey();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool GetFieldProperties( FullProperties* fullProperties )
///
/// /*Description*/
///
/// @param[in] FullProperties * fullProperties
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
int GetFieldProperties( FullProperties* fullProperties )
{
    return FacadeModele::getInstance()->getEditionField()->gatherSelectedNodeProperties(fullProperties);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool SendFieldProperties( FullProperties* fullProperties )
///
/// /*Description*/
///
/// @param[in] FullProperties * fullProperties
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
int SendFieldProperties( FullProperties* fullProperties )
{
    return 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool SendTest( BonusProperties* fullProperties )
///
/// /*Description*/
///
/// @param[in] BonusProperties * fullProperties
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
int SendTest( BonusProperties* fullProperties )
{
    return 0;
}




void testConnexionUDPCSharp()
{
    GestionnaireReseau::obtenirInstance()->demarrerNouvelleConnection("Test", "127.0.0.1", UDP);

    FacadePortability::sleep(1000);

    PaquetRondelle* wPaquet = (PaquetRondelle*) GestionnaireReseau::obtenirInstance()->creerPaquet(RONDELLE);
    wPaquet->setGameId(9000);
    wPaquet->setPosition(Vecteur3(0.0f,1.0f,2.0f));
    wPaquet->setVelocite(Vecteur3(3.0f,4.0f,5.0f));
    wPaquet->setVitesseRotation(6.5556f);
    GestionnaireReseau::obtenirInstance()->envoyerPaquet("Test", wPaquet, UDP);

}
