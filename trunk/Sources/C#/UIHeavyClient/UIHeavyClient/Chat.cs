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

        public static string WholeMessage
        {
            get { return mWholeMessage; }
        }

        public static void UpdateChat(string userName, string message)
        {
            mWholeMessage += (userName + " dit :\n");
            mWholeMessage += (message + "\n");
        }

        public static void CheckForNewMessage()
        {
            string userName = "", message = "";
            bool newMessage = false;

            // CALL DLL

            if (newMessage)
                UpdateChat(userName, message);
        }
    }
}
