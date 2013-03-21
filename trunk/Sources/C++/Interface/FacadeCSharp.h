/// Il ne pose pas de probleme de faire les includes das le .h car uniquement le .cpp inclut ce fichier
/// ce qui facilite les declarations des fonctions.

#include "BancTests.h"
#include "QueueThreadSafe.h"
#include <ctime>
#include "FacadeModele.h"
#include "Vue.h"
#include "GestionnaireEvenements.h"
#include "..\Application\RepartiteurActions.h"
#include "..\Jeu\JoueurHumain.h"
#include "..\Reseau\GestionnaireReseauClientLourd.h"
#include "..\Reseau\PaquetHandlers\PacketHandler.h"
#include "..\Reseau\UsinePaquets\UsinePaquetChatMessage.h"
#include "..\Reseau\UsinePaquets\UsinePaquetUserStatus.h"
#include "..\Reseau\Paquets\PaquetChatMessage.h"
#include "..\Reseau\Paquets\PaquetTest.h"
#include "..\Reseau\ControllerCSharp.h"
#include "UsineNoeud.h"
#include "..\Jeu\JoueurVirtuel.h"
#include "..\Jeu\JoueurNetwork.h"
#include "..\Configuration\ConfigScene.h"
#include "..\Sons\SoundFMOD.h"
#include "GestionnaireModeles.h"


// Summary:
//     Specifies constants that define which mouse button was pressed.
enum MouseButtons
{
    // Summary:
    //     No mouse button was pressed.
    None = 0,
    //
    // Summary:
    //     The left mouse button was pressed.
    Left = 1048576,  //2^20
    //
    // Summary:
    //     The right mouse button was pressed.
    Right = 2097152,
    //
    // Summary:
    //     The middle mouse button was pressed.
    Middle = 4194304,
    //
    // Summary:
    //     The first XButton was pressed.
    XButton1 = 8388608,
    //
    // Summary:
    //     The second XButton was pressed.
    XButton2 = 16777216,
};

// Test pour l'appel de la DLL depuis le C#
extern "C" 
{
    struct AIProfile
    {
        char* Name;
        int Speed;
        int FailProb;
    };

    __declspec(dllexport) void InitOpenGL(HWND hWnd);
    __declspec(dllexport) void FreeApplicationMemory( );
    __declspec(dllexport) void RenderOpenGL();
    __declspec(dllexport) void WindowResized(int largeur, int hauteur);
    __declspec(dllexport) void LogicUpdate(float time);
    __declspec(dllexport) int ExecuteUnitTest();
    __declspec(dllexport) void InitDLL();
    __declspec(dllexport) void RequestLogin( char* pUsername, char* pPassword, char* pIpAdress );
    __declspec(dllexport) void CancelConnection( char* pConnectionId );
    __declspec(dllexport) void SendMessageDLL( char* pConnectionId, char* pUsername, char * pMessage );
    __declspec(dllexport) void SendMessageGameDLL( char * pMessage );
    __declspec(dllexport) void ReloadModels(){GestionnaireModeles::obtenirInstance()->ReloadModels();}

    __declspec(dllexport) bool ActionPerformed( ActionType action );
    __declspec(dllexport) bool IsGamePaused();
    __declspec(dllexport) void PauseGame(bool doPause);
    __declspec(dllexport) void GenerateDefaultField();
    __declspec(dllexport) bool ValidateField();
    __declspec(dllexport) void ResetCamera();


    __declspec(dllexport) bool TerrainHasDeletable();
    /// checks if selected nodes are the same type and returns that type
    /// if not, return NODE_KEY_NONE
    __declspec(dllexport) RazerKey GetSelectedNodeUniqueKey();
    __declspec(dllexport) int GetFieldProperties(class FullProperties* fullProperties);
    __declspec(dllexport) int SendFieldProperties(class FullProperties* fullProperties);
    __declspec(dllexport) int SendTest( class BonusProperties* fullProperties);
	// Load/Save calls
    __declspec(dllexport) void SaveMap(char* pFileName);
    __declspec(dllexport) void LoadMap(char* pFileName);
    __declspec(dllexport) void SetPlayMap(char* pFileName);
    __declspec(dllexport) void SetSecondPlayer(bool pIsHuman, char* pName);

	// AI configuration calls
    __declspec(dllexport) void AddPlayer(char* pName, int pSpeed, int pFailProb);
    __declspec(dllexport) void RemovePlayer(char* pName);
	__declspec(dllexport) int GetNbrPlayers();
	__declspec(dllexport) void GetPlayers(AIProfile* pProfiles, int pNbrProfiles);

	// Control calls
    __declspec(dllexport) void GetKeyboardControl(int* pControls);
    __declspec(dllexport) void SetKeyboardControl(int* pControls);
		

	// Radio calls
    __declspec(dllexport) void PlayRadioSong();
    __declspec(dllexport) void PauseRadioSong();
    __declspec(dllexport) void StopRadioSong();
    __declspec(dllexport) void NextRadioSong();
    __declspec(dllexport) void PreviousRadioSong();
    __declspec(dllexport) void SetRadioVolume(int pVolume);
    __declspec(dllexport) void SetCurrentRadioPlaylist(char* pPlaylist);
    __declspec(dllexport) void GetCurrentRadioPlaylist(char* pPlaylist);

    // Radio configuration calls
    __declspec(dllexport) int GetNbrPlaylists();
    __declspec(dllexport) void GetRadioPlaylists(char** pPlaylists, int pNbrPlaylists);
    __declspec(dllexport) int GetNbrSongs(char* pPlaylist);
    __declspec(dllexport) void GetPlaylistSongs(char* pPlaylist, char** pSongs, int pNbrSongs);
    __declspec(dllexport) void RemoveRadioPlaylist(char* pPlaylist);
    __declspec(dllexport) void AddRadioPlaylist(char* pPlaylist, char** pSongs, int pNbrSongs);


    // Tournaments calls
    __declspec(dllexport) void BeginNewTournament(char* pTournamentName, char* pMapName, char** pPlayerNames, int pNbrPlayers);
    __declspec(dllexport) void ContinueExistingTournament(char* pTournamentName);


    ///////////////////////////////////////////////////////////////////////////////
    // User mouse and keyboard events
    __declspec(dllexport) void OnKeyPressed(int key);
    __declspec(dllexport) void OnKeyReleased(int key);
    __declspec(dllexport) void OnMousePressed( int x, int y, MouseButtons button);
    __declspec(dllexport) void OnMouseReleased( int x, int y, MouseButtons button);
    __declspec(dllexport) void OnMouseMoved( int x, int y, MouseButtons button );
    __declspec(dllexport) void OnMouseWheelMoved( int deltaRotation );
    ///////////////////////////////////////////////////////////////////////////////



    //////////////////////////////////////////////////////////////////////////
    /// Chat functions
    /// declare the callback prototype
    typedef BOOL (__stdcall *MessageReceivedCallBack)( char* pUsername, char* pMessage );
    /// Enregistre la callback pour mettre a jour la vue lors de nouveau message
    __declspec(dllexport) void SetMessageCallback( MessageReceivedCallBack callback );

    /// declare the callback prototype
    typedef BOOL (__stdcall *EventReceivedCallBack)( EventCodes pId, char* pMessage );
    /// Enregistre la callback pour mettre a jour la vue lors d'événement
    __declspec(dllexport) void SetEventCallback( EventReceivedCallBack callback );

    __declspec(dllexport) void DisconnectUser( char* pUsername );
    //////////////////////////////////////////////////////////////////////////




    class ControllerInterface;
    class Paquet;
    //////////////////////////////////////////////////////////////////////////
    /// Network functions
    __declspec(dllexport) void initNetwork( ControllerInterface* pController );
    __declspec(dllexport) void envoyerPaquet( Paquet* pPaquet );
    __declspec(dllexport) void connectServerGame( char* pServerIP );
    __declspec(dllexport) void connectPartieServerGame( int pGameId );
    __declspec(dllexport) void requestGameCreationServerGame( char* pGameName );
    __declspec(dllexport) void testConnexionUDPCSharp();
    //__declspec(dllexport) void recevoirPaquet( Paquet* pPaquet );
    /// 
    //////////////////////////////////////////////////////////////////////////
   
}