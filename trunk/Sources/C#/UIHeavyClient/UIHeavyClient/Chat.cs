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
    /// @class Chat
    /// @brief The chat.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public class Chat
    {
        ////////// DLL functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageDLL(string pConnectionId, string pUsername, string pMessage);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageGameDLL(string pMessage);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void SetMessageCallback(MessageReceivedCallBack callback);
        

        ////////// Delegates functions definitions
        delegate bool EventReceivedCallBack(int id, IntPtr message);
        delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message);


        ////////// Attibutes
        // The whole conversation
        string mWholeMessage;
        // Connected users
        List<string> mConnectedUsers=new List<string>();
        // Last user who has talked
        string mLastUser = "";
        // New messages?
        bool mNewMessages = false;

        
        

        ////////////////////////////////////////////////////////////////////////
        /// @property string Chat.WholeMessage
        ///
        /// Property for the conversation.
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public string WholeMessage
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
        public List<string> ConnectedUsers
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
        public bool NewMessages
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
        public void UpdateChat(string userName, string message)
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
        public void AddServerEventMessage(string message)
        {
            message = "[" + DateTime.Now.ToString("HH:mm") + "]  " + message + "\n";
            mLastUser = null;
            mWholeMessage += message;
            mNewMessages = true;
        }



        ////////////////////////////////////////////////////////////////////////
        /// @fn static void ClearContent()
        ///
        /// Clear the content of the chat
        /// 
        /// @return void
        ////////////////////////////////////////////////////////////////////////
        public void ClearContent()
        {
            mWholeMessage = "";
            mConnectedUsers.Clear();
        }


        public void addChatUser(string pUser)
        {
            mConnectedUsers.Add(pUser);
            mConnectedUsers.Sort();
        }

        public void removeChatUser(string pUser)
        {
            // Juste pour etre certain. Pas sur si le == operator ferait la job.
            foreach (string wUser in mConnectedUsers)
            {
                if(wUser.ToString() == pUser.ToString())
                {
                    mConnectedUsers.Remove(wUser);
                    mConnectedUsers.Sort();
                    break;
                }
            }
        }

    }

}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


