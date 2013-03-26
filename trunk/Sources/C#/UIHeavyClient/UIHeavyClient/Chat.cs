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
    ///////////////////////////////////////////////////////////////////////////
    /// @struct ChatUser
    /// @brief To handle a chat user.
    ///
    /// @author Michael Ferris
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    struct ChatUser
    {
        public string mUserName;
        public string mPassword;
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
        ////////// DLL functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageDLL(string pConnectionId, string pUsername, string pMessage);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageGameDLL(string pMessage);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void SetMessageCallback(MessageReceivedCallBack callback);
        // sends a request to connect the user. Will not be necessarly connected when exiting this function
        // must wait for a callback indicating the status of this user's connection

        //****** A METTRE AILLEURS 
        /*
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RequestLogin(string pUsername, string pPassword, string pIpAdress);
        [DllImport(@"RazerGame.dll")]
        static extern void SetEventCallback(EventReceivedCallBack callback);
        static LoginControl mLoginWindow = null;
        public static bool SetupLoginCallBackEvents(LoginControl pLoginWindow)
        {
            mLoginWindow = pLoginWindow;
            if (mLoginWindow != null)
            {
                SetEventCallback(mLoginEventCallback);
                return true;
            }
            return false;
        }
        
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
                            mLoginWindow = null;
                        });
                        break;
                    case EventCodes.USER_ALREADY_CONNECTED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.UserNameAlreadyChosen();
                            mLoginWindow = null;
                        });
                        break;
                    case EventCodes.USER_DID_NOT_SEND_NAME_ON_CONNECTION:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Error", true);
                            mLoginWindow.UnBlockUIContent();
                            mLoginWindow = null;
                        });
                        break;
                    case EventCodes.USER_DISCONNECTED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Error", true);
                            mLoginWindow.UnBlockUIContent();
                            mLoginWindow = null;
                        });
                        break;
                    case EventCodes.CONNECTION_CANCELED:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Canceled", false);
                            mLoginWindow.UnBlockUIContent();
                            mLoginWindow = null;
                        });
                        break;
                    case EventCodes.RECONNECTION_TIMEOUT:
                        // On n'écoute plus les événements
                        SetEventCallback(null);
                        // Signal à la fenetre l'événement
                        mLoginWindow.mTaskManager.ExecuteTask(() =>
                        {
                            mLoginWindow.SetUserMessageFeedBack("Connection Timed out", true);
                            mLoginWindow.UnBlockUIContent();
                            mLoginWindow = null;
                        });
                        break;
                    case EventCodes.RECONNECTION_IN_PROGRESS: break;
                    case EventCodes.WRONG_PASSWORD: break;
                    default: break;
                }
            }
            return true;
        }
        static EventReceivedCallBack mLoginEventCallback = LoginWindowEventReceived;
        */

        ////////// Delegates functions definitions
        delegate bool EventReceivedCallBack(int id, IntPtr message);
        delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message);


        ////////// Attibutes
        // The whole conversation
        static string mWholeMessage;
        // Connected users
        static List<string> mConnectedUsers = new List<string>();
        // Last user who has talked
        static string mLastUser = "";
        // New messages?
        static bool mNewMessages = false;

        
        ////////// Callbacks saved in the chat
        static MessageReceivedCallBack mMessageCallback = MessageReceived;



        ////////////////////////////////////////////////////////////////////////
        /// @property string Chat.WholeMessage
        ///
        /// Property for the conversation.
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public static string WholeMessage
        {
            get { return mWholeMessage; }
        }


        ////////////////////////////////////////////////////////////////////////
        /// @property string Chat.ConnectedUsers
        ///
        /// Property for the connected users.
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public static List<string> ConnectedUsers
        {
            get { return mConnectedUsers; }
        }


        ////////////////////////////////////////////////////////////////////////
        /// @property static bool Chat.NewMessages
        ///
        /// Property to know if there are new messages
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public static bool NewMessages
        {
            get { return mNewMessages; }
            set { mNewMessages = value; }
        }

        

        ////////////////////////////////////////////////////////////////////////
        /// @fn void Chat.UpdateChat()
        ///
        /// Update conversation. (Reception of a new Message)
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


        ////////////////////////////////////////////////////////////////////////
        /// @fn static void AddServerEventMessage(string message)
        ///
        /// Add a message from a server message
        /// 
        /// @param[in] string message : Message from server
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public static void AddServerEventMessage(string message)
        {
            message = "[" + DateTime.Now.ToString("HH:mm") + "]  " + message + "\n";
            mLastUser = null;
            mWholeMessage += message;
            mNewMessages = true;
        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn bool MessageReceived(IntPtr pUsername, IntPtr pMessage)
        ///
        /// Add a message from a server message
        /// 
        /// @param[in] IntPtr   pUsername   : Username (pointer to)
        /// @param[in] IntPtr   pMessage    : Message (pointer to)
        ///
        /// @return bool : not used for not
        ////////////////////////////////////////////////////////////////////////
        static bool MessageReceived(IntPtr pUsername, IntPtr pMessage)
        {
            string message = Marshal.PtrToStringAnsi(pMessage);
            string username = Marshal.PtrToStringAnsi(pUsername);
            UpdateChat(username, message);
            /*if (mMainWindow != null)
            {
                mMainWindow.mTaskManager.ExecuteTask(() =>
                {
                    mMainWindow.ShowWholeMessage();
                });
            }*/
            return true;
        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn static void ClearContent()
        ///
        /// Clear the content of the chat
        /// 
        /// @return void
        ////////////////////////////////////////////////////////////////////////
        public static void ClearContent()
        {
            mWholeMessage = "";
            mConnectedUsers.Clear();
        }



    }

}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


