using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UIHeavyClient
{
    static class Chat
    {
        public static MainWindow mContext;
        static string mWholeMessage;
        static string mLastUser = "";

        public static string WholeMessage
        {
            get { return mWholeMessage; }
        }

        public static void UpdateChat(string userName, string message)
        {
            if (userName != mLastUser)
            {
                mWholeMessage += (userName + " dit :\n");
                mLastUser = userName;
            }

            mWholeMessage += (message + "\n");

            SendNewMessage(userName, message);
        }

        public static void CheckForNewMessage()
        {
            string userName = "", message = "";
            bool newMessage = false;

            // CALL DLL
            // ...

            if (newMessage)
                UpdateChat(userName, message);
        }

        public static void SendNewMessage(string userName, string message)
        { 
            // CALL DLL
            // ...
        }
    }
}
