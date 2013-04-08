///////////////////////////////////////////////////////////////////////////////
/// @file MainWindowHandler.xaml.cs
/// @author Vincent Lemire and Micheal Ferris
/// @date 2013-02-26
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using System.Windows.Forms;
using HttpHockeduRequests;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class MainWindowHandler
    /// @brief Static wrapper to access every controls.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    static class MainWindowHandler
    {
        // Members
        private static MainWindow mContext;
        private static string mCurrentMap = "";
        private static OpenFileDialog mOpenFileDialog = new OpenFileDialog();
        private static Microsoft.Win32.SaveFileDialog mSaveFileDialog = new Microsoft.Win32.SaveFileDialog();
        private static int mMapId = -1;

        // Properties
        public static MainWindow Context
        {
            get { return mContext; }
            set { mContext = value; }
        }
        public static string CurrentMap
        {
            get { return mCurrentMap; }
            set { mCurrentMap = value; }
        }
        public static int MapId
        {
            get { return mMapId; }
            set { mMapId = value; }
        }

        public static TaskManager mTaskManager = new TaskManager();

        // Event C++ function
        [DllImport(@"RazerGame.dll")]
        public static extern bool ActionPerformed(ActionType action);

        
        // Save/Load C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SaveMap(string pFileName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void LoadMap(string pFileName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetPlayMap(string pFileName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void LaunchAchievementEvent(AchievementEvent pEvent);

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.InitCallbacks()
        ///
        /// On doit definir les callbacks pour chaque etat defini dans le CallbackManager.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void InitCallbacks()
        {
            CallbackManager.Init();

            // CallbackContainers
            CallbackContainer wContainerEdition = new CallbackContainer();
            wContainerEdition.mEditionEventCallback=Context.EditionModeControl.EventCallBack;

            CallbackContainer wContainerPlay = new CallbackContainer();
            wContainerPlay.mMessageReceivedCallback = PlayModeControl.mMessageCallback;
            wContainerPlay.mEventReceivedCallback=PlayModeControl.mEventCallback;

            CallbackContainer wContainerMainMenu = new CallbackContainer();
            wContainerMainMenu.mEventReceivedCallback = LoginControl.LoginControlEventReceived;

            CallbackContainer wContainerOnlineLobby = new CallbackContainer();
            wContainerOnlineLobby.mEventReceivedCallback = OnlineLobbyControl.CallbackEvent;
            wContainerOnlineLobby.mMessageReceivedCallback = OnlineLobbyControl.CallbackMessage;

            // Create callback lists
            List<CallbackContainer> wEditionStateCallBacks = new List<CallbackContainer>() { wContainerEdition, wContainerOnlineLobby, };
            List<CallbackContainer> wPlayStateCallBacks = new List<CallbackContainer>() { wContainerPlay, wContainerOnlineLobby, };
            List<CallbackContainer> wMainMenuStateCallBacks = new List<CallbackContainer>() { wContainerMainMenu, wContainerOnlineLobby, };
            List<CallbackContainer> wOnlineStateCallBacks = new List<CallbackContainer>() { wContainerOnlineLobby, };

            // Create elements list
            List<UIElement> wEditionStateElements = Context.GetEditionUniqueElements();
            List<UIElement> wPlayStateElements = Context.GetPlayUniqueElements();
            List<UIElement> wMainMenuStateElements = new List<UIElement>() { };
            List<UIElement> wOnlineStateElements = new List<UIElement>() { };

            // Adding states
            CallbackManager.AddState(GameState.GAME_STATE_EDITION, wEditionStateCallBacks, wEditionStateElements);
            CallbackManager.AddState(GameState.GAME_STATE_PLAY, wPlayStateCallBacks, wPlayStateElements);
            CallbackManager.AddState(GameState.GAME_STATE_MAIN_MENU, wMainMenuStateCallBacks, wMainMenuStateElements);
            CallbackManager.AddState(GameState.GAME_STATE_ONLINE_LOBBY, wOnlineStateCallBacks, wOnlineStateElements);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToEditionMode()
        ///
        /// Append OpenGL to edition mode and go there.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToEditionMode(bool pLoadDefaultXML = true)
        {
            
            if(CallbackManager.ChangeGameMode(GameState.GAME_STATE_EDITION))
            {
                bool wSuccess = false;
                if (pLoadDefaultXML)
                {
                    wSuccess = ActionPerformed(ActionType.ACTION_ALLER_MODE_EDITION);
                }
                else
                {
                    wSuccess = ActionPerformed(ActionType.ACTION_ALLER_MODE_EDITION_PAS_DEFAUT_XML);
                }
                if (wSuccess)
                {

                    Context.WindowContentControl.Content=Context.EditionModeControl;
                    Context.PlayModeControl.RemoveOpenGL();
                    Context.EditionModeControl.AppendOpenGL();
                    Context.EditionModeControl.InitButtons();
                    Context.EditionModeControl.mPropertiesGroupBox.DisplayProperties(RazerKey.RAZER_KEY_NONE);
                    Context.OnlineLobbyControl.ClearOnlineUsers();
                    CallbackManager.CommitChanges();
                }
                else
                {
                    CallbackManager.RevertChanges();
                }
            }
            else
            {
                CallbackManager.RevertChanges();
            }

            Context.EditionModeControl.SetGuidanceInstuction("");
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToPlayMode()
        ///
        /// Append OpenGL to play mode and init it according to the type of game.
        /// 
        /// @param[in] ActionType : To specify the type of game.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToPlayMode(ActionType pAction)
        {
            if (pAction != ActionType.ACTION_ALLER_MODE_JEU && pAction != ActionType.ACTION_ALLER_MODE_TOURNOI)
            {
                return;
            }

            
            if(CallbackManager.ChangeGameMode(GameState.GAME_STATE_PLAY))
            {
                if (ActionPerformed(pAction))
                {
                    Context.WindowContentControl.Content=Context.PlayModeControl;
                    Context.EditionModeControl.RemoveOpenGL();
                    Context.PlayModeControl.AppendOpenGL();
                    Context.PlayModeControl.DisplayRadioPlaylists();
                    Context.PlayModeControl.DisplayRadioVolume();
                    Context.OnlineLobbyControl.ClearOnlineUsers();
                    CallbackManager.CommitChanges();
                    Context.RestartGameMenuHandle(false);
                    Context.ReplayMenuHandle(false);
                    Context.PlayModeControl.HandleChatVisibility(false);
                }
                else
                {
                    CallbackManager.RevertChanges();
                }
            }
            else
            {
                CallbackManager.RevertChanges();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToMainMenu()
        ///
        /// Return to main menu.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToMainMenu()
        {
            SynchroniseAchievements();

            if(CallbackManager.ChangeGameMode(GameState.GAME_STATE_MAIN_MENU))
            {
                if (ActionPerformed(ActionType.ACTION_ALLER_MENU_PRINCIPAL))
                {
                    Context.WindowContentControl.Content=Context.MainMenuControl;

                    Context.MainMenuControl.InitOperations();
                    Context.EditionModeControl.RemoveOpenGL();
                    Context.PlayModeControl.RemoveOpenGL();
                    Context.MainMenuControl.DisplayProfileNames();
                    Context.OnlineLobbyControl.ClearOnlineUsers();

                    MainWindowHandler.Context.RestartGameMenuHandle(true);

                    LoginControl.DisconnectMasterServer();
                    CallbackManager.CommitChanges();
                }
                else
                {
                    CallbackManager.RevertChanges();
                }
            }
            else
            {
                CallbackManager.RevertChanges();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToTournamentMenu()
        ///
        /// Tournament menu + AI profile reading.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToTournamentMenu()
        {
            Context.WindowContentControl.Content=Context.TournamentControl;
            Context.TournamentControl.DisplayProfileNames();
            Context.TournamentControl.SetFocus();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToOnlineLobby()
        ///
        /// Online mode + read current server games.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToOnlineLobby()
        {
            // set callback events and messages
            if (CallbackManager.ChangeGameMode(GameState.GAME_STATE_ONLINE_LOBBY))
            {
                Context.WindowContentControl.Content = Context.OnlineLobbyControl;
                Context.OnlineLobbyControl.RequestGamesList();
                CallbackManager.CommitChanges();
            }
            else
            {
                CallbackManager.RevertChanges();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToOptionsMenu()
        ///
        /// Option menu.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToOptionsMenu()
        {
            Context.WindowContentControl.Content = Context.OptionsControl;
            Context.OptionsControl.DisplayOptionsValue();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToRadioOptions()
        ///
        /// Radio menu + read playlists and songs.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToRadioOptions()
        {
            Context.WindowContentControl.Content = Context.RadioOptionControl;
            Context.RadioOptionControl.DisplayPlaylists();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToAIOptions()
        ///
        /// AI option menu + read AI profiles.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToAIOptions()
        {
            Context.WindowContentControl.Content = Context.AIOptionControl;
            Context.AIOptionControl.DisplayAIProfiles();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.GoToKeyboardOption()
        ///
        /// Keyboard options + display current configuration.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void GoToKeyboardOption()
        {
            Context.WindowContentControl.Content = Context.KeyboardOptionControl;
            Context.KeyboardOptionControl.DisplayPlayerTwoKeys();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.LoadMapFromLocal()
        ///
        /// Load a map from a path.
        /// 
        /// @param[in] string : The map's file path.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void LoadMapFromLocal(string pMapName)
        {
            LoadMap(pMapName);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.SaveMapToLocal()
        ///
        /// Save a map to the specified path.
        /// 
        /// @param[in] string : The map's file path.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void SaveMapToLocal(string pMapName)
        {
            SaveMap(pMapName);
            Context.EditionModeControl.SetGuidanceInstuction("Map saved to file \"" + pMapName + "\".");
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.DialogLoadMapFromLocal()
        ///
        /// Load map from a dialog window.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void DialogLoadMapFromLocal()
        {
            mOpenFileDialog.Title = "Choose a map file";
            mOpenFileDialog.Multiselect = false;
            mOpenFileDialog.Filter = "XML Files (*.xml)|*.xml";
            if (mOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                LoadMapFromLocal(mOpenFileDialog.FileName);
                mCurrentMap = mOpenFileDialog.FileName;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.DialogSaveMapToLocal()
        ///
        /// Save map from a dialog window.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void DialogSaveMapToLocal()
        {
            mSaveFileDialog.Title = "Enter the name of your map file";
            mSaveFileDialog.Filter = "XML Files (*.xml)|*.xml";
            if (mSaveFileDialog.ShowDialog().Value)
            {
                SaveMapToLocal(mSaveFileDialog.FileName);
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.QuickSaveMapToLocal()
        ///
        /// Save map to the last path it was saved in (prompt a dialog if it's the first time).
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void QuickSaveMapToLocal()
        {
            if (mCurrentMap == "")
            {
                DialogSaveMapToLocal();
            }
            else
            {
                SaveMapToLocal(mCurrentMap);
            }
        }



        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.CallbackMapUploaded()
        ///
        /// Upload map callback
        /// 
        /// @param[in] UploadOperationStatus : status.
        /// @param[in] int : map id.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void CallbackMapUploaded(HttpHockeduRequests.UploadOperationStatus pStatus, int pMapId)
        {
            // Implement this
            Console.WriteLine(pStatus);
            Console.WriteLine(pMapId);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.LoadPlayingMap()
        ///
        /// Call the DLL to load a map.
        /// 
        /// @param[in] string : The map's file path.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void LoadPlayingMap(string pMapFile)
        {
            SetPlayMap(pMapFile);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindowHandler.Cleanup()
        ///
        /// Reset data.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void Cleanup()
        {
            CallbackManager.ChangeGameMode(GameState.GAME_STATE_NONE);
        }


        public static void UploadAchievementsResponse( UploadOperationStatus pStatus )
        {
            if ( pStatus == UploadOperationStatus.UPLOAD_SUCCESS )
            {
                HttpManager wManager = new HttpManager();
                wManager.downloadAchievements( LoginControl.mLoginInfo.mUserId, LoginControl.mLoginInfo.mAuthKey, DownloadAchievementsResponse );
            }
            else
            {
                Console.WriteLine( pStatus );
            }
        }

        [DllImport( @"RazerGame.dll", CallingConvention = CallingConvention.Cdecl )]
        public static extern void ReloadAchievementsProgress();

        public static void DownloadAchievementsResponse( DownloadOperationStatus pStatus )
        {
            Console.WriteLine( pStatus );
            if ( pStatus == DownloadOperationStatus.DOWNLOAD_SUCCESS )
            {
                ReloadAchievementsProgress();
            }
        }
        

        public static void SynchroniseAchievements()
        {
            if(LoginControl.mLoginInfo.mAuthOnWeb)
            {
                HttpManager wManager = new HttpManager();
                wManager.uploadAchievements( LoginControl.mLoginInfo.mUserId, LoginControl.mLoginInfo.mAuthKey, UploadAchievementsResponse );
            }
        }


    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


