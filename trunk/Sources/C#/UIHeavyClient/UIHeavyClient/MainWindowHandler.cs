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

namespace UIHeavyClient
{
    public delegate bool EventReceivedCallBack( EventCodes id, IntPtr message );
    public delegate bool EditionEventCallBack( EditionEventCodes pEvent );
    //declare the callback prototype
    public delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message);

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

        private static LoginControl mLoginControl = new LoginControl();

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
        public static LoginControl LoginUI
        {
            get { return mLoginControl; }
        }

        public static TaskManager mTaskManager = new TaskManager();

        [DllImport(@"RazerGame.dll")]
        public static extern bool ActionPerformed(ActionType action);

        [DllImport(@"RazerGame.dll")]
        static extern void SetEventCallback(EventReceivedCallBack callback);

        [DllImport( @"RazerGame.dll" )]
        static extern void SetEditionEventCallBack( EditionEventCallBack callback );

        //Callback to received user messages from C++
        [DllImport(@"RazerGame.dll")]
        static extern void SetMessageCallback(MessageReceivedCallBack callback);

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

        static EventReceivedCallBack mCurrentEventCallBack;
        static MessageReceivedCallBack mCurrentMessageCallback;

        public static void GoToEditionMode()
        {
            MessageReceivedCallBack messageCallBack = null;
            EditionEventCallBack eventCallBack = Context.EditionModeControl.EventCallBack;
            SetMessageCallback(messageCallBack);
            SetEditionEventCallBack( eventCallBack );
            SetEventCallback(null);

            if (ActionPerformed(ActionType.ACTION_ALLER_MODE_EDITION))
            {
                mCurrentEventCallBack = null;
                mCurrentMessageCallback = messageCallBack;

                Context.WindowContentControl.Content = Context.EditionModeControl;
                Context.PlayModeControl.RemoveOpenGL();
                Context.EditionModeControl.AppendOpenGL();
                Context.EditionModeControl.InitButtons();
                Context.EditionModeControl.mPropertiesGroupBox.DisplayProperties(RazerKey.RAZER_KEY_NONE);
            }
            else
            {
                SetMessageCallback(mCurrentMessageCallback);
                SetEventCallback(mCurrentEventCallBack);
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

            MessageReceivedCallBack messageCallBack = PlayModeControl.mMessageCallback;
            EventReceivedCallBack eventCallBack = null;
            SetMessageCallback(messageCallBack);
            SetEventCallback(eventCallBack);

            if (ActionPerformed(pAction))
            {
                mCurrentMessageCallback = messageCallBack;
                mCurrentEventCallBack = eventCallBack;


                Context.WindowContentControl.Content = Context.PlayModeControl;
                Context.EditionModeControl.RemoveOpenGL();
                Context.PlayModeControl.AppendOpenGL();
                Context.PlayModeControl.DisplayRadioPlaylists();
            }
            else
            {
                SetMessageCallback(mCurrentMessageCallback);
                SetEventCallback(mCurrentEventCallBack);
            }

            Context.HandleEditionMenuItem(false);
        }

        public static void GoToMainMenu()
        {
            MessageReceivedCallBack messageCallBack = null;
            EventReceivedCallBack eventCallBack = null;
            SetMessageCallback(messageCallBack);
            SetEventCallback(eventCallBack);

            if (ActionPerformed(ActionType.ACTION_ALLER_MENU_PRINCIPAL))
            {
                mCurrentMessageCallback = messageCallBack;
                mCurrentEventCallBack = eventCallBack;

                Context.WindowContentControl.Content = Context.MainMenuControl;
                Context.MainMenuControl.InitOperations();

                Context.EditionModeControl.RemoveOpenGL();
                Context.PlayModeControl.RemoveOpenGL();

                Context.MainMenuControl.DisplayProfileNames();
            }
            else
            {
                SetMessageCallback(mCurrentMessageCallback);
                SetEventCallback(mCurrentEventCallBack);
            }

            Context.HandleEditionMenuItem(false);
        }

        public static void GoToTournamentMenu()
        {
            Context.WindowContentControl.Content = Context.TournamentControl;
            Context.TournamentControl.DisplayProfileNames();
        }

        public static void GoToOnlineLobby()
        {
            // set callback events and messages

            Context.WindowContentControl.Content = Context.OnlineLobbyControl;
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

        public static void LoadPlayingMap(string pMapFile)
        {
            SetPlayMap(pMapFile);
        }

        public static void Cleanup()
        {
            SetEventCallback(null);
            SetMessageCallback(null);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


