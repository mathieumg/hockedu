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

namespace UIHeavyClient
{
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
        static string mConnectedUsers = "En ligne :\nTa Mère\nTa Soeur\nTa Cousine";

        // Last user who has talked
        static string mLastUser = "";

        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageDLL(string username, string message);

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
        public static string ConnectedUsers
        {
            get { return mConnectedUsers; }
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
            // Don't write the name if it's the same user again
            if (userName != mLastUser)
            {
                mWholeMessage += (userName + " dit :\n");
                mLastUser = userName;
            }

            mWholeMessage += (message + "\n");
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void Chat.CheckForConnectedUsers()
        ///
        /// Call the server to check for connected users.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public static void CheckForConnectedUsers()
        {
            // TODO : CALL DLL
            // ...

            mConnectedUsers = "En ligne :\nTa Mère\nTa Soeur\nTa Cousine"; // TEMP
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void Chat.CheckForNewMessage()
        ///
        /// Call the server to send a new messages.
        /// 
        /// @param[in] string : The user name.
        /// @param[in] string : The message.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public static void SendNewMessage(string userName, string message)
        {
            SendMessageDLL(userName,message);
        }
    }
}
