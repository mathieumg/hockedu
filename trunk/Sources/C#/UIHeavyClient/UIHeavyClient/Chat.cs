///////////////////////////////////////////////////////////////////////////////
/// @file Chat.cs
/// @author Vincent Lemire
/// @date 2013-02-12
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Input;
using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Media;
using System.Windows.Controls;

namespace UIHeavyClient
{
    struct ChatUser
    {
        public string mUserName;
        public string mPassword;
        public string mUserState;
    }

    struct LoginWindowSavedInfo
    {
        public string mUserName;
        public string mPassword;
        public string mIpAddress;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @class Chat
    /// @brief The chat.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    static class Chat
    {
        // The whole conversation
        static string mWholeMessage;

        // Connected users
        static List<string> mConnectedUsers = new List<string>();

        // Last user who has talked
        static string mLastUser = "";

        // New messages?
        static bool mNewMessages = false;

        public static LoginWindowSavedInfo mLoginInfo = new LoginWindowSavedInfo();
        ////////////////////////////////////////////////////////////////////////
        /// @propertie string Chat.WholeMessage
        ///
        /// Propertie for the conversation.
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public static string WholeMessage
        {
            get { return mWholeMessage; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @propertie string Chat.ConnectedUsers
        ///
        /// Propertie for the connected users.
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public static List<string> ConnectedUsers
        {
            get { return mConnectedUsers; }
        }

        public static bool NewMessages
        {
            get { return mNewMessages; }
            set { mNewMessages = value; }
        }

        public static bool IsIPv4(string value)
        {
            IPAddress address;

            Func<char, bool> myFunc = c => c == '.';
            int nbDot = value.Count(myFunc);
            if (nbDot == 3 && IPAddress.TryParse(value, out address))
            {
                if (address.AddressFamily == AddressFamily.InterNetwork)
                {
                    return true;
                }
            }
            return false;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void Chat.UpdateChat()
        ///
        /// Update conversation.
        /// 
        /// @param[in] string : The user name.
        /// @param[in] string : The message.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public static void UpdateChat(string userName, string message)
        {
            message = "    [" + DateTime.Now.ToString("HH:mm") + "]  " + message + "\n";

            // Don't write the name if it's the same user again
            if (userName != mLastUser)
            {
                mWholeMessage += (userName + " says :\n");
                mLastUser = userName;
            }

            mWholeMessage += (message);
            mNewMessages = true;
        }

        public static void AddServerEventMessage(string message)
        {
            message = "[" + DateTime.Now.ToString("HH:mm") + "]  " + message + "\n";

            mLastUser = null;

            mWholeMessage += message;
            mNewMessages = true;
        }

        public static void ControlEnabledChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            Control control = sender as Control;
            if ((bool)e.NewValue)
            {
                control.Foreground = Brushes.White;
            }
            else
            {
                control.Foreground = Brushes.Black;
            }
        }

        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageDLL(string pConnectionId, string pUsername, string pMessage);
        // sends a request to connect the user. Will not be necessarly connected when exiting this function
        // must wait for a callback indicating the status of this user's connection
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RequestLogin(string pUsername, string pPassword, string pIpAdress);

        delegate bool EventReceivedCallBack(int id, IntPtr message);
        [DllImport(@"RazerGame.dll")]
        static extern void SetEventCallback(EventReceivedCallBack callback);
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        // Callback to received event messages from C++
        // declare the callback prototype
        // 
        public static bool SetupLoginCallBackEvents(LoginWindow pLoginWindow)
        {
            mLoginWindow = pLoginWindow;
            if (mLoginWindow != null)
            {
                SetEventCallback(mLoginEventCallback);
                return true;
            }
            return false;
        }
        static LoginWindow mLoginWindow = null;
        static bool LoginWindowEventReceived(int id, IntPtr pMessage)
        {
            if (mLoginWindow != null && id >= 0 && (int)EventCodes.NB_EVENT_CODES > id)
            {
                string message = Marshal.PtrToStringAnsi(pMessage);
                EventCodes type = (EventCodes)id;
                switch (type)
                {
                    case EventCodes.USER_CONNECTED:
                        // La fenetre principale doit maintenant ecouter les evenement pour mettre a jour le chat
                        SetEventCallback(mMainWindowEventCallback);
                        SetMessageCallback(mMessageCallback);

                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.ConnectionSuccessful();
                        });
                        mLoginWindow = null;
                        break;
                    case EventCodes.USER_ALREADY_CONNECTED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.UserNameAlreadyChosen();
                        });
                        mLoginWindow = null;
                        break;
                    case EventCodes.USER_DID_NOT_SEND_NAME_ON_CONNECTION:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Error", true);
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventCodes.USER_DISCONNECTED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Error", true);
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventCodes.CONNECTION_CANCELED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Canceled", false);
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventCodes.RECONNECTION_TIMEOUT:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Timed out", true);
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventCodes.RECONNECTION_IN_PROGRESS: break;
                    case EventCodes.WRONG_PASSWORD: break;
                    default: break;
                }
            }
            return true;
        }
        static EventReceivedCallBack mLoginEventCallback = LoginWindowEventReceived;
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        //
        //Callback to received event messages from C++
        //declare the callback prototype
        static MainWindow mMainWindow;
        public static MainWindow MainWindow
        {
            private get { return Chat.mMainWindow; }
            set { Chat.mMainWindow = value; }
        }
        static bool MainWindowEventReceived(int id, IntPtr pMessage)
        {
            /*string message = Marshal.PtrToStringAnsi(pMessage);
            if (id >= 0 && (int)EventCodes.NB_EVENT_CODES > id)
            {
                EventCodes type = (EventCodes)id;
                switch (type)
                {
                    case EventCodes.USER_CONNECTED:
                        AddServerEventMessage("Connection successful !");
                        if (mMainWindow != null)
                        {
                            mMainWindow.mTaskManager.ExecuteTask(() =>
                            {
                                mMainWindow.UnBlockUIContent();
                                mMainWindow.ShowWholeMessage();
                            });
                        }
                        break;
                    case EventCodes.USER_ALREADY_CONNECTED:
                        MessageBoxResult dialogResult1 = MessageBox.Show("User with same name already connected\nDo you want to retry connection?", "Connection Error", MessageBoxButton.YesNo);
                        if (dialogResult1 == MessageBoxResult.Yes)
                        {
                            if (mMainWindow != null)
                            {
                                mMainWindow.mTaskManager.ExecuteTask(() =>
                                {
                                    mMainWindow.BlockUIContent();
                                    RequestLogin(mLoginInfo.mUserName, mLoginInfo.mIpAddress);
                                });
                            }
                        }
                        else if (dialogResult1 == MessageBoxResult.No)
                        {
                            if (mMainWindow != null)
                            {
                                mMainWindow.mTaskManager.ExecuteTask(() =>
                                {
                                    mMainWindow.OnDisconnect(null, null);
                                });
                            }
                        }
                        break;
                    case EventCodes.USER_DISCONNECTED:
                        MessageBoxResult dialogResult2 = MessageBox.Show("Error trying to reach server\nDo you want to retry connection?", "Connection Error", MessageBoxButton.YesNo);
                        if (dialogResult2 == MessageBoxResult.Yes)
                        {
                            if (mMainWindow != null)
                            {
                                mMainWindow.mTaskManager.ExecuteTask(() =>
                                {
                                    mMainWindow.BlockUIContent();
                                    RequestLogin(mLoginInfo.mUserName, mLoginInfo.mIpAddress);
                                });
                            }
                        }
                        else if (dialogResult2 == MessageBoxResult.No)
                        {
                            if (mMainWindow != null)
                            {
                                mMainWindow.mTaskManager.ExecuteTask(() =>
                                {
                                    mMainWindow.OnDisconnect(null, null);
                                });
                            }
                        }
                        break;
                    case EventCodes.RECONNECTION_TIMEOUT:
                        MessageBoxResult dialogResult3 = MessageBox.Show("Connection to server timed out\nDo you want to retry connection?", "Connection Lost", MessageBoxButton.YesNo);
                        if (dialogResult3 == MessageBoxResult.Yes)
                        {
                            if (mMainWindow != null)
                            {
                                mMainWindow.mTaskManager.ExecuteTask(() =>
                                {
                                    mMainWindow.BlockUIContent();
                                    RequestLogin(mLoginInfo.mUserName, mLoginInfo.mIpAddress);
                                });
                            }
                        }
                        else if (dialogResult3 == MessageBoxResult.No)
                        {
                            if (mMainWindow != null)
                            {
                                mMainWindow.mTaskManager.ExecuteTask(() =>
                                {
                                    mMainWindow.OnDisconnect(null, null);
                                });
                            }
                        }
                        break;
                    case EventCodes.RECONNECTION_IN_PROGRESS:
                        AddServerEventMessage("Connection lost, attempting reconnection");
                        if (mMainWindow != null)
                        {
                            mMainWindow.mTaskManager.ExecuteTask(() =>
                            {
                                mMainWindow.BlockUIContent();
                                mMainWindow.ShowWholeMessage();
                            });
                        }
                        break;
                    case EventCodes.SERVER_USER_DISCONNECTED:
                        // on enleve le user de la liste
                        ConnectedUsers.Remove(message);

                        // affiche un message de l'événement
                        message = message + " Disconnected";
                        AddServerEventMessage(message);
                        if (mMainWindow != null)
                        {
                            mMainWindow.mTaskManager.ExecuteTask(() =>
                            {
                                mMainWindow.ShowWholeMessage();
                            });
                        }
                        break;
                    case EventCodes.SERVER_USER_CONNECTED:
                        // on ajoute le user de la liste
                        ConnectedUsers.Add(message);

                        // affiche un message de l'événement
                        message = message + " Connected";
                        AddServerEventMessage(message);
                        if (mMainWindow != null)
                        {
                            mMainWindow.mTaskManager.ExecuteTask(() =>
                            {
                                mMainWindow.ShowWholeMessage();
                            });
                        }
                        break;
                    default: break;
                }
            }*/
            return true;
        }
        static EventReceivedCallBack mMainWindowEventCallback = MainWindowEventReceived;
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        //Callback to received user messages from C++
        //declare the callback prototype
        delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message);
        [DllImport(@"RazerGame.dll")]
        static extern void SetMessageCallback(MessageReceivedCallBack callback);
        static bool MessageReceived(IntPtr pUsername, IntPtr pMessage)
        {
            string message = Marshal.PtrToStringAnsi(pMessage);
            string username = Marshal.PtrToStringAnsi(pUsername);
            UpdateChat(username, message);
            if (mMainWindow != null)
            {
                /*mMainWindow.mTaskManager.ExecuteTask(() =>
                {
                    mMainWindow.ShowWholeMessage();
                });*/
            }
            return true;
        }

        public static void ClearContent()
        {
            mWholeMessage = "";
            mConnectedUsers.Clear();
        }

        static MessageReceivedCallBack mMessageCallback = MessageReceived;
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
    }

}
