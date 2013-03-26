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

namespace UIHeavyClient
{
    struct OnlineGameInfos
    {
        int id;
        uint serverId;
        string name;
        string creatorName;
        bool needPassword;

    }

    /// <summary>
    /// Logique d'interaction pour OnlineLobbyControl.xaml
    /// </summary>
    public partial class OnlineLobbyControl : UserControl
    {
        private PasswordPrompt mPasswordPrompt;
        private GameCreationPrompt mGameCreationPrompt;

        private Dictionary<int, OnlineGameInfos> mOnlineGameInfos;

        public OnlineLobbyControl()
        {
            InitializeComponent();
            mPasswordPrompt = new PasswordPrompt();
            mGameCreationPrompt = new GameCreationPrompt();
        }

        private void mBackToMainButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        private void mEditionModeButton_Click(object sender, RoutedEventArgs e)
        {
            // Vincent, tu peux aller chercher la liste des maps d'un user avec 
            // HttpManager wManager = new HttpManager();
            // wManager.getUserMapList(int pUserId, string pAuthentication)
            // apres avoir ete chercher les infos dans la DLL

            MainWindowHandler.GoToEditionMode();
        }

        private void mJoinButton_Click(object sender, RoutedEventArgs e)
        {
            mPasswordPrompt.ClearInput();
            mPasswordPrompt.ShowDialog();

            if (mPasswordPrompt.OkIsClicked)
            { 
                // TODO
                // Join game logic
            }
        }

        private void mCreateButton_Click(object sender, RoutedEventArgs e)
        {
            mGameCreationPrompt.ClearInputAndLoadMapList();
            mGameCreationPrompt.ShowDialog();



            if (mGameCreationPrompt.OkIsClicked)
            {
                // TODO
                // Create game logic
            }   
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
    }
}
