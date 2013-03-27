///////////////////////////////////////////////////////////////////////////////
/// @file OnlineLobbyControl.xaml.cs
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
using HttpHockeduRequests;
using System.Runtime.InteropServices;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @struct OnlineGameInfos
    /// @brief To receive game data from the Dll.
    ///
    /// @author Vincent Lemire
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    class OnlineGameInfos
    {
        public int id;
        public uint serverId;
        public string name;
        public string creatorName;
        public string mapName;
        public bool needPassword;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @class OnlineLobbyControl
    /// @brief Window for online mode.
    ///
    /// @author Vincent Lemire
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public partial class OnlineLobbyControl : UserControl
    {
        // Members
        private PasswordPrompt mPasswordPrompt;
        private GameCreationPrompt mGameCreationPrompt;
        private ServerMapPrompt mServerMapPrompt;

        private HttpManager mHttpManager;

        private Dictionary<int, OnlineGameInfos> mOnlineGameInfos;


        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGameCreationServerGame(string pGameName, string pMapName, string pPassword);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void connectPartieServerGame(int pGameId, string pInputPassword);

        public OnlineLobbyControl()
        {
            InitializeComponent();

            mPasswordPrompt = new PasswordPrompt();
            mGameCreationPrompt = new GameCreationPrompt();
            mServerMapPrompt = new ServerMapPrompt();

            mHttpManager = new HttpManager();
        }

        private void mBackToMainButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        private void mEditionModeButton_Click(object sender, RoutedEventArgs e)
        {
            mServerMapPrompt = new ServerMapPrompt();
            mServerMapPrompt.GetServerMaps();
            mServerMapPrompt.ShowDialog();

            if (mServerMapPrompt.OkIsClicked)
            {
                mHttpManager.downloadMap(12, mServerMapPrompt.SelectedMap.id, HandleDownloadedMap);
            }

            mServerMapPrompt.Close();
        }

        private void mJoinButton_Click(object sender, RoutedEventArgs e)
        {
            OnlineGameInfos selected = (mOnlineGameListView.SelectedItem as OnlineGameInfos);

            if (selected != null)
            {
                if (selected.needPassword)
                {
                    mPasswordPrompt = new PasswordPrompt();
                    mPasswordPrompt.ShowDialog();

                    if (mPasswordPrompt.OkIsClicked)
                    {
                        connectPartieServerGame(selected.id, mPasswordPrompt.Password);
                    }

                    mPasswordPrompt.Close();
                }
                else
                {
                    connectPartieServerGame(selected.id, "");
                }
            }
        }

        private void mCreateButton_Click(object sender, RoutedEventArgs e)
        {
            mGameCreationPrompt = new GameCreationPrompt();
            mGameCreationPrompt.ClearInputAndLoadMapList();
            mGameCreationPrompt.ShowDialog();

            if (mGameCreationPrompt.OkIsClicked)
            {
                requestGameCreationServerGame(mGameCreationPrompt.Name, mGameCreationPrompt.Map.name, mGameCreationPrompt.Password);
            }

            mGameCreationPrompt.Close();
        }

        private void mRandomButton_Click(object sender, RoutedEventArgs e)
        {
            // TODO
            // Join random game logic
        }

        private void mRefreshButton_Click(object sender, RoutedEventArgs e)
        {
            // TODO
            // Read data again
        }

        private void submitButton_Click(object sender, RoutedEventArgs e)
        {
            // Ouin, faudrait p-e implémenter le chat...
        }

        public void HandleDownloadedMap(string pFilepath)
        {
            // Load the map to edition mode
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                MainWindowHandler.LoadMapFromLocal(pFilepath);
                MainWindowHandler.GoToEditionMode();
            });
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


