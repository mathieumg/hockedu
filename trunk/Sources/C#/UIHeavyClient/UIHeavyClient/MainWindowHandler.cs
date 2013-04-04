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
        public static void GoToEditionMode()
        {
            
            if(CallbackManager.ChangeGameMode(GameState.GAME_STATE_EDITION))
            {
                if (ActionPerformed(ActionType.ACTION_ALLER_MODE_EDITION))
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
        /// @fn void MainWindowHandler.SaveMapToServer()
        ///
        /// Online save.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public static void SaveMapToServer()
        {
            HttpManager wHttpManager = new HttpManager();
            wHttpManager.uploadNewMap(12, "", "", "", true, mCurrentMap, null); 
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
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


