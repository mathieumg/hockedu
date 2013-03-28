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
    public struct OnlineGameInfos
    {
        public int id {get; set;}
        public uint serverId { get; set; }
        public string name { get; set; }
        public string creatorName { get; set; }
        public string mapName { get; set; }
        public bool needPassword { get; set; }
        public string needPasswordString { get; set; }

        public OnlineGameInfos(int pId, uint pServerId, string pName, string pCreatorName, string pMapName, bool pNeedPassword, string pNeedPasswordString)
            : this()
        {
            id = pId;
            serverId = pServerId;
            name = pName;
            creatorName = pCreatorName;
            mapName = pMapName;
            needPassword = pNeedPassword;
            needPasswordString = pNeedPasswordString;
        }
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

        //private Dictionary<int, OnlineGameInfos> mOnlineGameInfos;
        private Chat mChat;

        // Properties
        public Chat ChatObject
        {
            get { return mChat;}
        }

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGameCreationServerGame(string pGameName, string pMapName, string pPassword);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void connectPartieServerGame(int pGameId, string pInputPassword);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetNbrServerGames();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetServersGames([In, Out] OnlineGameInfos[] pGames, int pNbrGames);

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.OnlineLobbyControl()
        ///
        /// Constructor.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public OnlineLobbyControl()
        {
            InitializeComponent();

            mPasswordPrompt = new PasswordPrompt();
            mGameCreationPrompt = new GameCreationPrompt();
            mServerMapPrompt = new ServerMapPrompt();

            mHttpManager = new HttpManager();
            mChat = new Chat();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.mBackToMainButton_Click()
        ///
        /// Return to main menu.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mBackToMainButton_Click(object sender, RoutedEventArgs e)
        {
            mChat.ClearContent();
            MainWindowHandler.GoToMainMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.mEditionModeButton_Click()
        ///
        /// Goto to edition mode.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.mJoinButton_Click()
        ///
        /// Join an existing game.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mJoinButton_Click(object sender, RoutedEventArgs e)
        {
            OnlineGameInfos? selected = (mOnlineGameListView.SelectedItem as OnlineGameInfos?);

            if (selected != null)
            {
                if (selected.Value.needPassword)
                {
                    mPasswordPrompt = new PasswordPrompt();
                    mPasswordPrompt.ShowDialog();

                    if (mPasswordPrompt.OkIsClicked)
                    {
                        connectPartieServerGame(selected.Value.id, mPasswordPrompt.Password);
                    }

                    mPasswordPrompt.Close();
                }
                else
                {
                    connectPartieServerGame(selected.Value.id, "");
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.mCreateButton_Click()
        ///
        /// Create a new game and wait for someone.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mCreateButton_Click(object sender, RoutedEventArgs e)
        {
            mGameCreationPrompt = new GameCreationPrompt();
            mGameCreationPrompt.ClearInputAndLoadMapList();
            mGameCreationPrompt.ShowDialog();

            if (mGameCreationPrompt.OkIsClicked)
            {
                requestGameCreationServerGame(mGameCreationPrompt.GameName, mGameCreationPrompt.Map.name, mGameCreationPrompt.Password);
            }

            mGameCreationPrompt.Close();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.mRandomButton_Click()
        ///
        /// Join a random existing game.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mRandomButton_Click(object sender, RoutedEventArgs e)
        {
            // TODO
            // Join random game logic
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.mRefreshButton_Click()
        ///
        /// Refresh game data.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mRefreshButton_Click(object sender, RoutedEventArgs e)
        {
            // Display games again
            DisplayServerGames();
        }

        ////////// Methodes pour le chat dans le lobby

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.submitButton_Click()
        ///
        /// Send message to chat.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void submitButton_Click(object sender, RoutedEventArgs e)
        {
            if (messageTextBox.Text!="")
            {
                Chat.SendMessageDLL("MasterServer", MainWindowHandler.Context.MainMenuControl.LoginControlElement.UserName, messageTextBox.Text);
                messageTextBox.Clear();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.CallbackEvent()
        ///
        /// Callback for events.
        /// 
        /// @param[in] EventCodes : event's id.
        /// @param[in] IntPtr : event's message.
        ///
        /// @return bool.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.HandleDownloadedMap()
        ///
        /// Receive a server map.
        /// 
        /// @param[in] string : The map's file path.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void HandleDownloadedMap(string pFilepath)
        {
            // Load the map to edition mode
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                MainWindowHandler.LoadMapFromLocal(pFilepath);
                MainWindowHandler.GoToEditionMode();
            });
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.CallbackMessage()
        ///
        /// Callback when receiving a message.
        /// 
        /// @param[in] IntPtr : The user.
        /// @param[in] IntPtr : The message.
        ///
        /// @return bool.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.PropertiesRefreshWarning()
        ///
        /// Waning.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.PropertiesRefreshWarning()
        ///
        /// Waning.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void BlockUIContent()
        {
            messageTextBox.IsEnabled=false;
            submitButton.IsEnabled=false;
            onlineListView.IsEnabled=false;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.PropertiesRefreshWarning()
        ///
        /// Waning.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void UnBlockUIContent()
        {
            messageTextBox.IsEnabled=true;
            submitButton.IsEnabled=true;
            onlineListView.IsEnabled=true;
        }

        public void DisplayServerGames()
        {
            int nbrServerGames = GetNbrServerGames();

            OnlineGameInfos[] gameInfos = new OnlineGameInfos[nbrServerGames];

            for (uint i = 0; i < nbrServerGames; ++i )
            {
                gameInfos[i] = new OnlineGameInfos(0, 0, new string('s', 255), new string('s', 255), new string('s', 255), false, new string('s', 4));
            }

            GetServersGames(gameInfos, nbrServerGames);

            mOnlineGameListView.Items.Clear();

            foreach (OnlineGameInfos g in gameInfos)
            {
                mOnlineGameListView.Items.Add((object)g);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


