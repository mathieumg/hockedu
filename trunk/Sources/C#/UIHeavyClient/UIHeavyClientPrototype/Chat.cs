using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace UIHeavyClient
{
    static class Chat
    {
        public static MainWindow mContext;
        static string mWholeMessage;
        static string mLastUser = "";

        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendMessageDLL(string message);
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetMessageDLL();

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
        }

        public static void CheckForNewMessage()
        {
            GetMessageDLL();

        }

        public static void SendNewMessage(string userName, string message)
        {
            SendMessageDLL(message);
        }
    }
}
