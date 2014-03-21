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
using System.ComponentModel;
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
        public string opponent { get; set; }
        public string mapName { get; set; }
        public bool needPassword { get; set; }
        public string needPasswordString { get; set; }

        public OnlineGameInfos(int pId, uint pServerId, string pName, string pCreatorName, string pMapName, bool pNeedPassword, string pOpponent)
            : this()
        {
            id = pId;
            serverId = pServerId;
            name = pName;
            creatorName = pCreatorName;
            mapName = pMapName;
            needPassword = pNeedPassword;
            if ( pNeedPassword )
            {
                needPasswordString = "Yes";
            }
            else
            {
                needPasswordString = "No";
            }
            opponent = pOpponent;
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
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGamesList();

        private PasswordPrompt mPasswordPrompt;
        private GameCreationPrompt mGameCreationPrompt;
        private ServerMapPrompt mServerMapPrompt;

        private HttpManager mHttpManager;
        public HttpHockeduRequests.HttpManager HttpManager
        {
            get { return mHttpManager; }
        }
        private Chat mChat;

        private bool mIsWaitingForOnlineGame;
        private OnlineGameInfos mGameWaitingToConnect;

        private GridViewColumnHeader mLastClickedHeader;
        private ListSortDirection mLastSortDirection;

        private Dictionary<string, string> mColumnToMember;

        // Properties
        public Chat ChatObject
        {
            get { return mChat;}
        }

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGameCreationServerGame(string pGameName, string pMapName, int pMapId, string pPassword);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestMatchmaking();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void connectPartieServerGame(int pGameId, uint pServerId, string pInputPassword);
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

            mIsWaitingForOnlineGame = false;

            mColumnToMember = new Dictionary<string, string>() 
            { 
                {"ID", "id"},
                {"Server ID", "serverId"},
                {"Name", "name"},
                {"Creator's Name", "creatorName"},
                {"Need Password", "needPasswordString"},
                {"Map Name", "mapName"},
            };

            var buttons = RazerUtilities.FindTypedChildren<Button>( this, true );
            // delete button is disabled by default and it is assigned before the callback can be seted
            //mDeleteButton.Foreground = Brushes.Black;
            foreach ( var button in buttons )
            {
                /// the button will no longer flash after a click
                button.Focusable = false;
            }

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

            if (LoginControl.mLoginInfo.mAuthOnWeb)
            {
                HttpRequestForLoadingMap();
            }
            else
            {
                WebLogin.CreateWebLoginWindow(HttpRequestForLoadingMap);
            }
        }

        private void HttpRequestForLoadingMap()
        {
            mServerMapPrompt = new ServerMapPrompt();

            mServerMapPrompt.GetUserMaps();
            mServerMapPrompt.ShowDialog();

            if (mServerMapPrompt.OkIsClicked)
            {
                if (mServerMapPrompt.SelectedMap != null)
                {
                    mFeedbackLabel.Content = "Loading, please wait...";
                    HandleUIButtons(false);

                    mHttpManager.downloadMap(Convert.ToInt32(LoginControl.mLoginInfo.mUserId), mServerMapPrompt.SelectedMap.id, LoginControl.mLoginInfo.mAuthKey, HandleDownloadedMap);
                }
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
                    mPasswordPrompt.SetFocus();
                    mPasswordPrompt.ShowDialog();

                    if (mPasswordPrompt.OkIsClicked)
                    {
                        mFeedbackLabel.Content = "Loading, please wait...";
                        HandleUIButtons(false);

                        connectPartieServerGame(selected.Value.id, selected.Value.serverId, mPasswordPrompt.Password);
                        mGameWaitingToConnect = selected.Value;
                        mIsWaitingForOnlineGame = true;   
                    }

                    mPasswordPrompt.Close();
                }
                else
                {
                    mFeedbackLabel.Content = "Loading, please wait...";
                    HandleUIButtons(false);

                    connectPartieServerGame(selected.Value.id, selected.Value.serverId, "");
                    mGameWaitingToConnect = selected.Value;
                    mIsWaitingForOnlineGame = true;
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
            mGameCreationPrompt.SetFocus();
            mGameCreationPrompt.ShowDialog();

            if (mGameCreationPrompt.OkIsClicked)
            {
                mFeedbackLabel.Content = "Loading, please wait...";
                HandleUIButtons(false);
                requestGameCreationServerGame(mGameCreationPrompt.GameName, mGameCreationPrompt.Map.name, mGameCreationPrompt.Map.id, mGameCreationPrompt.Password);
                mGameWaitingToConnect = new OnlineGameInfos(-1, 0, mGameCreationPrompt.GameName, mGameCreationPrompt.Map.author, mGameCreationPrompt.Map.name, mGameCreationPrompt.Password != "", "");
                mIsWaitingForOnlineGame = true;
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
            MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame = true;
            requestMatchmaking();
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
            RequestGamesList();
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
                Chat.SendMessageDLL("MasterServer", MainWindowHandler.Context.gMainMenuControl.LoginControlElement.UserName, messageTextBox.Text);
                messageTextBox.Clear();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.CallbackMapDownloaded()
        ///
        /// Callback for when a map is downloaded.
        /// 
        /// @param[in] string : The local map path.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void CallbackMapDownloaded(string pOutputPath, int pMapId, string pName, string pDescription, bool pIsPublic)
        {
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                HandleUIButtons(true);

                if(pOutputPath.Length > 0)
                {
                    // Assigner la map a jouer dans le modele avec le filepah recu
                    MainWindowHandler.LoadPlayingMap(pOutputPath);
                    // On vient de recevoir la map download, on veut maintenant passer au mode jeu
                    MainWindowHandler.GoToPlayMode(ActionType.ACTION_ALLER_MODE_JEU);
                    MainWindowHandler.Context.RestartGameMenuHandle(true);
                    MainWindowHandler.Context.ReplayMenuHandle(true);
                    MainWindowHandler.Context.gPlayModeControl.HandleChatVisibility(true);
                }
            });
        }
        public void CallbackMapListForMapDownload( List<UserMapDetailedJSON> pList )
        {
            // Une fois qu'on a charge la liste des maps, on trouve la bonne 
            string wMapToFind = MainWindowHandler.Context.gOnlineLobbyControl.mGameWaitingToConnect.mapName;
            string wAuthor    = MainWindowHandler.Context.gOnlineLobbyControl.mGameWaitingToConnect.creatorName;
            foreach (UserMapDetailedJSON wMap in pList)
            {
                if (wMap.name == wMapToFind && wMap.author == wAuthor)
                {
                    // Trouve, pas besoin du user_id pcq la map devrait etre publique si une partie a ete creee avec
                    MainWindowHandler.Context.gOnlineLobbyControl.mHttpManager.downloadMap(Convert.ToInt32(LoginControl.mLoginInfo.mUserId), wMap.id, "", MainWindowHandler.Context.gOnlineLobbyControl.CallbackMapDownloaded);
                    break;
                }
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

            if (id >= 0 && EventCodes.NB_EVENT_CODES > id)
            {
                OnlineLobbyControl wThis = MainWindowHandler.Context.gOnlineLobbyControl;
                switch (id)
                {
                    case EventCodes.GAME_CREATION_FAILED:
                    {
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            // Faire un check pour savoir si on a vraiment demander de se connecter ou de creer une partie
                            if (MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame)
                            {
                                MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame = false;
                                MainWindowHandler.Context.gOnlineLobbyControl.HandleUIButtons(true);
                                MainWindowHandler.Context.gOnlineLobbyControl.RequestGamesList();
                                if ( message == "1" )
                                {
                                    MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack( "No Game Server Found" );
                                }
                                else if ( message == "2" )
                                {
                                    MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack( "A game with that name already exists" );
                                }
                                else
                                {
                                    MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack( "Game creation failed for unknown reason." );
                                }
                            }
                        });
                        break;
                    }
                    case EventCodes.SERVER_USER_DISCONNECTED:
                    {
                        if (message.IndexOf("GameServer") != -1)
                        {
                            return true;
                        }

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
                        if (message.IndexOf("GameServer") != -1)
                        {
                            return true;
                        }

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
                    case EventCodes.GAME_ADDED:
                    {
                        // Splits the message
                        char SEPARATOR = '|';
                        string[] values = message.Split(SEPARATOR);
                        OnlineGameInfos wOnlineGameInfos = new OnlineGameInfos(int.Parse(values[1]), uint.Parse(values[0]), values[2], values[3], values[6], bool.Parse(values[5]), values[4]);
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            wThis.mOnlineGameListView.Items.Add((object)wOnlineGameInfos);
                        });
                    }
                    break;
                    case EventCodes.GAME_CONNECTION_RESPONSE_SUCCESS:
                    {
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            MainWindowHandler.Context.RestartGameMenuHandle(false);
                            MainWindowHandler.Context.AddAIOpponentHandle(false);
                        });

                        // Faire un check pour savoir si on a vraiment demander de se connecter ou de creer une partie
                        if (MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame)
                        {
                            MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame = false;

                            // Download la map pour la partie et on ne change pas de state avant que ce soit termine
                            // La seule facon de trouver les infos de la bonne map est d'aller chercher la liste et de trouver celle avec le bon nom
                            MainWindowHandler.Context.gOnlineLobbyControl.mHttpManager.getPublicMapList(wThis.CallbackMapListForMapDownload);
                        }
                    }
                    break;
                    case EventCodes.GAME_CONNECTION_RESPONSE_ALREADY_CONNECTED:
                    {
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            // Faire un check pour savoir si on a vraiment demander de se connecter ou de creer une partie
                            if (MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame)
                            {
                                MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame = false;

                                MainWindowHandler.Context.gOnlineLobbyControl.HandleUIButtons(true);
                                MainWindowHandler.Context.gOnlineLobbyControl.RequestGamesList();
                                MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack("You are already connected to this game...");
                            }
                        });
                    }
                    break;
                    case EventCodes.GAME_CONNECTION_RESPONSE_GAME_FULL:
                    {
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            // Faire un check pour savoir si on a vraiment demander de se connecter ou de creer une partie
                            if (MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame)
                            {
                                MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame = false;

                                MainWindowHandler.Context.gOnlineLobbyControl.HandleUIButtons(true);
                                MainWindowHandler.Context.gOnlineLobbyControl.RequestGamesList();
                                MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack("This game already have two player... try another one!");
                            }
                        });
                    }
                    break;
                    case EventCodes.GAME_CONNECTION_RESPONSE_GAME_NOT_FOUND:
                    {
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            // Faire un check pour savoir si on a vraiment demander de se connecter ou de creer une partie
                            if (MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame)
                            {
                                MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame = false;

                                MainWindowHandler.Context.gOnlineLobbyControl.HandleUIButtons(true);
                                MainWindowHandler.Context.gOnlineLobbyControl.RequestGamesList();
                                MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack("This game doesn't exist anymore... try another one!");
                            }
                        });
                    }
                    break;
                    case EventCodes.GAME_CONNECTION_RESPONSE_WRONG_PASSWORD_CS:
                    {
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            // Faire un check pour savoir si on a vraiment demander de se connecter ou de creer une partie
                            if (MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame)
                            {
                                MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame=false;

                                MainWindowHandler.Context.gOnlineLobbyControl.HandleUIButtons(true);
                                MainWindowHandler.Context.gOnlineLobbyControl.RequestGamesList();
                                MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack("Wrong Password");
                            }
                        });
                    }
                    break;
                    case EventCodes.GAME_CONNECTION_RESPONSE_GAME_CONNECTION_GENERAL_FAILURE:
                    {
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            // Faire un check pour savoir si on a vraiment demander de se connecter ou de creer une partie
                            if (MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame)
                            {
                                MainWindowHandler.Context.gOnlineLobbyControl.mIsWaitingForOnlineGame=false;

                                MainWindowHandler.Context.gOnlineLobbyControl.HandleUIButtons(true);
                                MainWindowHandler.Context.gOnlineLobbyControl.RequestGamesList();
                                MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack("An error occured while joining game.");
                            }
                        });
                    }
                    break;
                    case EventCodes.GAME_CONNECTION_RESPONSE_MATCHMAKING:
                    {
                        wThis.mGameWaitingToConnect.mapName = message;
                    }
                    break;
                    default: break;
                }
            }

            return true;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.RequestGamesList()
        ///
        /// Query game list.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void RequestGamesList()
        {
            mFeedbackLabel.Content = "";
            HandleUIButtons(true);
            mOnlineGameListView.Items.Clear();
            requestGamesList();
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
        public void HandleDownloadedMap(string pFilepath, int pMapId, string name, string description, bool pIsPublic)
        {
            if(pFilepath.Length > 0)
            {
                // Load the map to edition mode
                MainWindowHandler.mTaskManager.ExecuteTask(() =>
                {
                    MainWindowHandler.MapId = pMapId;
                    MainWindowHandler.LoadMapFromLocal(pFilepath);
                    MainWindowHandler.CurrentMap = pFilepath;
                    MainWindowHandler.MapId = pMapId;
                    MainWindowHandler.ServerMapName = name;
                    //SetOnlineEditionInfos(pMapId, name, description, pIsPublic);
                    MainWindowHandler.GoToEditionMode(false);
                });
            }
            else
            {
                MainWindowHandler.mTaskManager.ExecuteTask(() =>
                {
                    MainWindowHandler.Context.gOnlineLobbyControl.HandleUIButtons(true);
                    MainWindowHandler.Context.gOnlineLobbyControl.RequestGamesList();
                    MainWindowHandler.Context.gOnlineLobbyControl.DisplayFeedBack("Failed to download map");
                    
                });
            }
            
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
        public static bool CallbackMessage(IntPtr pUsername, IntPtr pMessage, IntPtr pGroupName)
        {
            string message=Marshal.PtrToStringAnsi(pMessage);
            string username = Marshal.PtrToStringAnsi(pUsername);
            string groupName = Marshal.PtrToStringAnsi(pGroupName);

            if(groupName == "lobby")
            {
                OnlineLobbyControl wThis = MainWindowHandler.Context.gOnlineLobbyControl;
                wThis.ChatObject.UpdateChat(username, message);
                MainWindowHandler.mTaskManager.ExecuteTask(() =>
                {
                    wThis.UpdateChatView();
                });
            }

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
        /// @fn void OnlineLobbyControl.UpdateConnectedUserList()
        ///
        /// Connected user list.
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
                SortListView("id", ListSortDirection.Ascending);
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.BlockUIContent()
        ///
        /// Disable some UI elements.
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
        /// @fn void OnlineLobbyControl.UnBlockUIContent()
        ///
        /// Enable some UI elements.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void UnBlockUIContent()
        {
            messageTextBox.IsEnabled=true;
            submitButton.IsEnabled=true;
            onlineListView.IsEnabled=true;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.ResizeGridColumns()
        ///
        /// Resize the list view columns.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] SizeChangedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ResizeGridColumns(object sender, SizeChangedEventArgs e)
        {
            double totalWidth = (sender as ListView).ActualWidth;
            GridView grid = (sender as ListView).View as GridView;

            grid.Columns[0].Width = totalWidth * 0.1;
            grid.Columns[1].Width = totalWidth * 0.15;
            grid.Columns[2].Width = totalWidth * 0.2;
            grid.Columns[3].Width = totalWidth * 0.15;
            grid.Columns[4].Width = totalWidth * 0.2;
            grid.Columns[5].Width = totalWidth * 0.2;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.ClickForSorting()
        ///
        /// Sort by clicking on a header.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ClickForSorting(object sender, RoutedEventArgs e)
        {
            GridViewColumnHeader clickedHeader = e.OriginalSource as GridViewColumnHeader;
            ListSortDirection sortDirection;

            if (clickedHeader != null)
            {
                if (clickedHeader.Role != GridViewColumnHeaderRole.Padding)
                {
                    if (clickedHeader != mLastClickedHeader)
                    {
                        sortDirection = ListSortDirection.Ascending;
                    }
                    else
                    {
                        if (mLastSortDirection == ListSortDirection.Ascending)
                        {
                            sortDirection = ListSortDirection.Descending;
                        }
                        else
                        {
                            sortDirection = ListSortDirection.Ascending;
                        }
                    }

                    string header = mColumnToMember[(clickedHeader.Column.Header as string)];
                    SortListView(header, sortDirection);

                    mLastClickedHeader = clickedHeader;
                    mLastSortDirection = sortDirection;
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.SortListView()
        ///
        /// Sort by a header.
        /// 
        /// @param[in] string : The member'a name.
        /// @param[in] ListSortDirection : The sorting direction.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void SortListView(string pSortBy, ListSortDirection pDirection)
        {
            ICollectionView items = CollectionViewSource.GetDefaultView(mOnlineGameListView.Items);

            if (items != null)
            {
                items.SortDescriptions.Clear();
                items.SortDescriptions.Add(new SortDescription(pSortBy, pDirection));
                items.Refresh();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.ClearOnlineUsers()
        ///
        /// When deconnecting, clear online users.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void ClearOnlineUsers()
        {
            onlineListView.Items.Clear();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.DisplayFeedBack()
        ///
        /// Public access for feedback label.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void DisplayFeedBack(string pMessage)
        {
            mFeedbackLabel.Content = pMessage;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OnlineLobbyControl.HandleUIButtons()
        ///
        /// Hide/show some UI button.
        /// 
        /// @param[in] bool : Hide or show.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void HandleUIButtons(bool pIsEnable)
        {
            mCreateButton.IsEnabled = pIsEnable;
            mJoinButton.IsEnabled = pIsEnable;
            mRandomButton.IsEnabled = pIsEnable;
            mRefreshButton.IsEnabled = pIsEnable;
            mEditionModeButton.IsEnabled = pIsEnable;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


