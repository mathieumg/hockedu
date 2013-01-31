///////////////////////////////////////////////////////////////////////////////
/// @file Chat.cs
/// @author Vincent Lemire
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Input;

namespace UIHeavyClientPrototype
{
    struct ChatUser
    {
        public string mUserName;
        public string mUserState;
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
            message = "    [" + DateTime.Now.ToString("HH:mm")  + "]  " + message + "\n";

            // Don't write the name if it's the same user again
            if (userName != mLastUser)
            {
                mWholeMessage += (userName + " dit :\n");
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

        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageDLL(string pUsername, string pMessage);


        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        // Keep synch with C++ in GestionnaireReseau
        enum EventType
        {
            USER_ALREADY_CONNECTED,
            USER_DID_NOT_SEND_NAME_ON_CONNECTION,
            USER_CONNECTED,
            USER_DISCONNECTED,
            CONNECTION_CANCELED,
            RECONNECTION_TIMEOUT,
            RECONNECTION_IN_PROGRESS,
            WRONG_PASSWORD,
            CHAT_MESSAGE_RECEIVED,
            SERVER_USER_CONNECTED,
            SERVER_USER_DISCONNECTED,
            NB_EVENT_CODES // Must be always last !
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////


        delegate bool EventReceivedCallBack(int id, IntPtr message);
        [DllImport(@"INF2990.dll")]
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
            if (mLoginWindow != null && id >= 0 && (int)EventType.NB_EVENT_CODES > id)
            {
                string message = Marshal.PtrToStringAnsi(pMessage);
                EventType type = (EventType)id;
                switch (type)
                {
                    case EventType.USER_CONNECTED:
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
                    case EventType.USER_ALREADY_CONNECTED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.UserNameAlreadyChosen();
                        });
                        mLoginWindow = null;
                        break;
                    case EventType.USER_DID_NOT_SEND_NAME_ON_CONNECTION:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.errorMessageLabel.Content = "Erreur de connection";
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventType.USER_DISCONNECTED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.errorMessageLabel.Content = "Erreur de connection";
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventType.CONNECTION_CANCELED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.errorMessageLabel.Content = "";
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventType.RECONNECTION_TIMEOUT:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.errorMessageLabel.Content = "Delais de connection dépassé";
                            mLoginWindow.UnBlockUIContent();
                        });
                        mLoginWindow = null;
                        break;
                    case EventType.RECONNECTION_IN_PROGRESS: break;
                    case EventType.WRONG_PASSWORD: break;
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
            string message = Marshal.PtrToStringAnsi(pMessage);
            if (id >= 0 && (int)EventType.NB_EVENT_CODES > id)
            {
                switch ((EventType)id)
                {
                    case EventType.USER_DISCONNECTED: 
                        
                        break;
                    case EventType.SERVER_USER_DISCONNECTED:
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
                    case EventType.SERVER_USER_CONNECTED:
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
            }
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
        [DllImport(@"INF2990.dll")]
        static extern void SetMessageCallback(MessageReceivedCallBack callback);
        static bool MessageReceived(IntPtr pUsername, IntPtr pMessage)
        {
            string message = Marshal.PtrToStringAnsi(pMessage);
            string username = Marshal.PtrToStringAnsi(pUsername);
            UpdateChat(username, message);
            if (mMainWindow != null)
            {
                mMainWindow.mTaskManager.ExecuteTask(() =>
                {
                    mMainWindow.ShowWholeMessage();
                });
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
