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
    struct OnlineGameInfos
    {
        int id;
        uint serverId;
        string name;
        string creatorName;
        bool needPassword;

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
        private PasswordPrompt mPasswordPrompt;
        private GameCreationPrompt mGameCreationPrompt;
        private ServerMapPrompt mServerMapPrompt;

        private HttpManager mHttpManager;

        private Dictionary<int, OnlineGameInfos> mOnlineGameInfos;
        private Chat mChat;

        public Chat ChatObject
        {
            get { return mChat;}
        }

        public OnlineLobbyControl()
        {
            InitializeComponent();

            mPasswordPrompt = new PasswordPrompt();
            mGameCreationPrompt = new GameCreationPrompt();
            mServerMapPrompt = new ServerMapPrompt();

            mHttpManager = new HttpManager();
            mChat = new Chat();
        }

        private void mBackToMainButton_Click(object sender, RoutedEventArgs e)
        {
            mChat.ClearContent();
            MainWindowHandler.GoToMainMenu();
        }

        private void mEditionModeButton_Click(object sender, RoutedEventArgs e)
        {
            // Vincent, tu peux aller chercher la liste des maps d'un user avec 
            // HttpManager wManager = new HttpManager();
            // wManager.getUserMapList(int pUserId, string pAuthentication)
            // apres avoir ete chercher les infos dans la DLL

            //mHttpManager.getUserMapList(0, "", HandleMaps);
        }

        private void HandleMaps(List<UserMapDetailedJSON> pList)
        {
            mServerMapPrompt.HandleMaps(pList);
            mServerMapPrompt.ShowDialog();

            if (mServerMapPrompt.OkIsClicked)
            {
                // TODO : 
                // Load map from server

                MainWindowHandler.GoToEditionMode();
            }
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








        ////////// Methodes pour le chat dans le lobby


        private void submitButton_Click(object sender, RoutedEventArgs e)
        {
            // Ouin, faudrait p-e implémenter le chat...

            if (messageTextBox.Text!="")
            {
                Chat.SendMessageDLL("MasterServer", MainWindowHandler.Context.MainMenuControl.LoginControlElement.UserName, messageTextBox.Text);
                messageTextBox.Clear();
            }
        }


        public static bool CallbackEvent(EventCodes id, IntPtr pMessage)
        {
            string message=Marshal.PtrToStringAnsi(pMessage);

            if (id>=0&&EventCodes.NB_EVENT_CODES>id)
            {
                OnlineLobbyControl wThis = MainWindowHandler.Context.OnlineLobbyControl;
                switch (id)
                {
                    case EventCodes.SERVER_USER_DISCONNECTED:
                    {
                        wThis.ChatObject.removeChatUser(message);

                        // affiche un message de l'événement
                        message=message+" Disconnected";
                        wThis.ChatObject.AddServerEventMessage(message);
                        
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            wThis.UpdateChatView();
                        });
                        
                    }
                    break;
                    case EventCodes.SERVER_USER_CONNECTED:
                    {
                        wThis.ChatObject.addChatUser(message);

                        // affiche un message de l'événement
                        message=message+" Connected";
                        wThis.ChatObject.AddServerEventMessage(message);

                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            wThis.UpdateChatView();
                        });
                        
                    }
                    break;
                    default: break;
                }
            }

            return true;
        }

        public static bool CallbackMessage(IntPtr pUsername, IntPtr pMessage)
        {
            string message=Marshal.PtrToStringAnsi(pMessage);
            string username=Marshal.PtrToStringAnsi(pUsername);
            OnlineLobbyControl wThis = MainWindowHandler.Context.OnlineLobbyControl;

            wThis.ChatObject.UpdateChat(username, message);

            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                wThis.UpdateChatView();
            });
            
            return true;
        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.UpdateChatView()
        ///
        /// Update the chat.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void UpdateChatView()
        {
            wholeMessageBox.Text=mChat.WholeMessage;

            if (mChat.NewMessages)
            {
                wholeMessageBox.ScrollToEnd();
                mChat.NewMessages=false;
            }
            UpdateConnectedUserList();
        }


        private void UpdateConnectedUserList()
        {
            // Si le nb d'element dans les 2 listes n'est pas le meme,  un user s'est connecte ou deconnecte.
            // On reconstruit donc le UI
            if (mChat.ConnectedUsers.Count != onlineListView.Items.Count)
            {
                onlineListView.Items.Clear();
                foreach (string wUser in mChat.ConnectedUsers)
                {
                    onlineListView.Items.Add(wUser);
                }
            }
        }


        public void BlockUIContent()
        {
            messageTextBox.IsEnabled=false;
            submitButton.IsEnabled=false;
            onlineListView.IsEnabled=false;
        }

        public void UnBlockUIContent()
        {
            messageTextBox.IsEnabled=true;
            submitButton.IsEnabled=true;
            onlineListView.IsEnabled=true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


