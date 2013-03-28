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
        private static MainWindow mContext;
        private static string mCurrentMap = "";
        private static OpenFileDialog mOpenFileDialog = new OpenFileDialog();
        private static Microsoft.Win32.SaveFileDialog mSaveFileDialog = new Microsoft.Win32.SaveFileDialog();

        

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

        [DllImport(@"RazerGame.dll")]
        public static extern bool ActionPerformed(ActionType action);

        

        // Save/Load
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SaveMap(string pFileName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void LoadMap(string pFileName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetPlayMap(string pFileName);
        // TODO
        // Save to server
        // Load to server


        // On doit definir les callbacks pour chaque etat defini dans le CallbackManager
        public static void InitCallbacks()
        {
            CallbackManager.Init();

            CallbackContainer wContainerEdition = new CallbackContainer();
            wContainerEdition.mEditionEventCallback=Context.EditionModeControl.EventCallBack;
            CallbackManager.AddCallback(GameState.GAME_STATE_EDITION, wContainerEdition);

            CallbackContainer wContainerPlay = new CallbackContainer();
            wContainerPlay.mMessageReceivedCallback = PlayModeControl.mMessageCallback;
            CallbackManager.AddCallback(GameState.GAME_STATE_PLAY, wContainerPlay);

            CallbackContainer wContainerMainMenu=new CallbackContainer();
            wContainerMainMenu.mEventReceivedCallback=LoginControl.LoginControlEventReceived;
            CallbackManager.AddCallback(GameState.GAME_STATE_MAIN_MENU, wContainerMainMenu);

            CallbackContainer wContainerOnlineLobby=new CallbackContainer();
            wContainerOnlineLobby.mEventReceivedCallback=OnlineLobbyControl.CallbackEvent;
            wContainerOnlineLobby.mMessageReceivedCallback=OnlineLobbyControl.CallbackMessage;
            CallbackManager.AddCallback(GameState.GAME_STATE_ONLINE_LOBBY, wContainerOnlineLobby);
            CallbackManager.AddCallback(GameState.GAME_STATE_MAIN_MENU, wContainerOnlineLobby);
            CallbackManager.AddCallback(GameState.GAME_STATE_PLAY, wContainerOnlineLobby);
            CallbackManager.AddCallback(GameState.GAME_STATE_EDITION, wContainerOnlineLobby);

        }


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
                    CallbackManager.CommitChanges();
                }
                else
                {
                    CallbackManager.RevertChanges();
                }
            }
            

            Context.HandleEditionMenuItem(true);
            Context.EditionModeControl.SetGuidanceInstuction("");
        }

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
                    CallbackManager.CommitChanges();
                }
                else
                {
                    CallbackManager.RevertChanges();
                }
            }
            
            Context.HandleEditionMenuItem(false);
        }

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
                    LoginControl.DisconnectMasterServer();
                    CallbackManager.CommitChanges();
                }
                else
                {
                    CallbackManager.RevertChanges();
                }
            }
            
            Context.HandleEditionMenuItem(false);
        }

        public static void GoToTournamentMenu()
        {
            if(CallbackManager.ChangeGameMode(GameState.GAME_STATE_TOURNAMENT_MENU))
            {
                Context.WindowContentControl.Content=Context.TournamentControl;
                Context.TournamentControl.DisplayProfileNames();
                CallbackManager.CommitChanges();
            }
        }

        public static void GoToOnlineLobby()
        {
            // set callback events and messages
            if(CallbackManager.ChangeGameMode(GameState.GAME_STATE_ONLINE_LOBBY))
            {
                Context.WindowContentControl.Content=Context.OnlineLobbyControl;
                CallbackManager.CommitChanges();
            }
        }

        public static void GoToOptionsMenu()
        {
            Context.WindowContentControl.Content = Context.OptionsControl;
        }

        public static void GoToRadioOptions()
        {
            Context.WindowContentControl.Content = Context.RadioOptionControl;
            Context.RadioOptionControl.DisplayPlaylists();
        }

        public static void GoToAIOptions()
        {
            Context.WindowContentControl.Content = Context.AIOptionControl;
            Context.AIOptionControl.DisplayAIProfiles();
        }

        public static void GoToKeyboardOption()
        {
            Context.WindowContentControl.Content = Context.KeyboardOptionControl;
            Context.KeyboardOptionControl.DisplayPlayerTwoKeys();
        }

        public static void LoadMapFromLocal(string pMapName)
        {
            LoadMap(pMapName);
        }

        public static void SaveMapToLocal(string pMapName)
        {
            SaveMap(pMapName);
            Context.EditionModeControl.SetGuidanceInstuction("Map saved to file \"" + pMapName + "\".");
        }

        public static void DialogLoadMapFromLocal()
        {
            mOpenFileDialog.Title = "Choose a map file";
            if (mOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                LoadMapFromLocal(mOpenFileDialog.FileName);
                mCurrentMap = mOpenFileDialog.FileName;
            }
        }

        public static void DialogSaveMapToLocal()
        {
            mSaveFileDialog.Title = "Enter the name of your map file";
            if (mSaveFileDialog.ShowDialog().Value)
            {
                SaveMapToLocal(mSaveFileDialog.FileName);
            }
        }

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

        public static void SaveMapToServer()
        {
            HttpManager wHttpManager = new HttpManager();
            wHttpManager.uploadNewMap(12, "", "", "", true, mCurrentMap, null); 
        }

        public static void LoadPlayingMap(string pMapFile)
        {
            SetPlayMap(pMapFile);
        }

        public static void Cleanup()
        {
            CallbackManager.ChangeGameMode(GameState.GAME_STATE_NONE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


