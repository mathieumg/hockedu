///////////////////////////////////////////////////////////////////////////////
/// @file MainWindow.xaml.cs
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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.Threading;
using System.ComponentModel;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class MainWindow
    /// @brief The chat window.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class MainWindow : Window
    {
        // The uset name
        string mUserName;

        // The login window which is displayed at begining
        LoginWindow mLoginWindow;

        // Validate if the user is connected
        bool mIsUserConnected;

        ////////////////////////////////////////////////////////////////////////
        /// @propertie string LoginWindow.UserName
        ///
        /// Propertie for the user name.
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public string UserName
        {
            get { return mUserName; }
            set { mUserName = value; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @propertie string LoginWindow.IsUserConnected
        ///
        /// Propertie for validate the user connexion.
        ///
        /// @return If the user is connected.
        ////////////////////////////////////////////////////////////////////////
        public bool IsUserConnected
        {
            get { return mIsUserConnected; }
            set { mIsUserConnected = value; }
        }

        // C++ function to initialise C# controller on that side
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitDLL();

        ////////////////////////////////////////////////////////////////////////////////////
        //
        // Keep synch with C++ in GestionnaireReseau
        public enum EventType { 
            USER_ALREADY_CONNECTED,
            USER_DID_NOT_SEND_NAME_ON_CONNECTION, 
            USER_CONNECTED, 
            USER_DISCONNECTED, 
            RECONNECTION_TIMEOUT, 
            RECONNECTION_IN_PROGRESS, 
            WRONG_PASSWORD,
            CHAT_MESSAGE_RECEIVED,
            SERVER_USER_CONNECTED,
            SERVER_USER_DISCONNECTED,
            NB_ELEM
        };

        //Callback to received event messages from C++
        //declare the callback prototype
        public delegate bool EventReceivedCallBack(int id, IntPtr message);
        [DllImport(@"INF2990.dll")]
        static extern void SetEventCallback(EventReceivedCallBack callback);
        static bool EventReceived(int id, IntPtr pMessage)
        {
            string message = Marshal.PtrToStringAnsi(pMessage);
            if (id >= 0 && MainWindow.EventType.NB_ELEM.CompareTo(id) < 0)
            {
                MainWindow.EventType type = (MainWindow.EventType)id;
                switch (type)
                {
                    case MainWindow.EventType.USER_CONNECTED: break;
                    case MainWindow.EventType.USER_ALREADY_CONNECTED: break;
                    case MainWindow.EventType.USER_DID_NOT_SEND_NAME_ON_CONNECTION: break;
                    case MainWindow.EventType.USER_DISCONNECTED: break;
                    default: break;
                }
            }
            Chat.UpdateChat("", message);
            return true;
        }
        EventReceivedCallBack mEventCallback = EventReceived;
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
            Chat.UpdateChat(username, message);
            return true;
        }
        MessageReceivedCallBack mMessageCallback = MessageReceived;
        ////////////////////////////////////////////////////////////////////////////////////

        private void bw_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            while(true)
            {
                if ((worker.CancellationPending == true))
                {
                    e.Cancel = true;
                    break;
                }
                else
                {
                    System.Threading.Thread.Sleep(100);
                    worker.ReportProgress(100);
                }
            }
        }
        private void bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            ShowWholeMessage();
        }
        BackgroundWorker mBgWorker = new BackgroundWorker();

        ////////////////////////////////////////////////////////////////////////
        /// @fn MainWindow.MainWindow()
        ///
        /// Default constructor.
        ///
        /// @return None (constructor).
        ////////////////////////////////////////////////////////////////////////
        public MainWindow()
        {
            InitializeComponent();
            InitDLL();

            // Init attributes
            mUserName = "";
            mIsUserConnected = false;
            // Display connexion window

            mLoginWindow = new LoginWindow();
            mLoginWindow.ShowDialog();
            // Once the login is closed, validate if the user is connected
            IsUserConnected = mLoginWindow.UserConnected;
            UserName = mLoginWindow.UserName;
            if (!IsUserConnected)
            {
                Close();
            }
            SetMessageCallback(mMessageCallback);
			messageTextBox.Focus();

            // Worker pour faire le rafraichissement de la fenetre
            BackgroundWorker mBgWorker = new BackgroundWorker();
            mBgWorker.WorkerReportsProgress = true;
            mBgWorker.DoWork +=
                new DoWorkEventHandler(bw_DoWork);
            mBgWorker.ProgressChanged +=
                new ProgressChangedEventHandler(bw_ProgressChanged);
            mBgWorker.RunWorkerAsync();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn MainWindow.~MainWindow()
        ///
        /// Destructor. Used to stop the sub thread
        ///
        /// @return None (destructor).
        ////////////////////////////////////////////////////////////////////////
        ~MainWindow()
        {
            
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.submitButton_Click()
        ///
        /// Event when the submit button is clicked.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void submitButton_Click(object sender, RoutedEventArgs e)
        {
            if (messageTextBox.Text != "" && mIsUserConnected)
            {
                Chat.SendNewMessage(mUserName, messageTextBox.Text);
                Chat.UpdateChat(mUserName, messageTextBox.Text); // TEMP
                Chat.CheckForConnectedUsers(); // TEMP
                messageTextBox.Clear();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.ShowWholeMessage()
        ///
        /// Update the chat.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void ShowWholeMessage()
        {
            wholeMessageBox.Text = Chat.WholeMessage;

            if (Chat.NewMessages)
            {
                wholeMessageBox.ScrollToEnd();
                Chat.NewMessages = false;
            }

            foreach (string s in Chat.ConnectedUsers)
            {
                if(onlineListView.Items.IndexOf(s) == -1)
                    onlineListView.Items.Add(s);
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.messageTextBox_KeyDown()
        ///
        /// Event when the user pressed a key.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////        
        private void messageTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            // If enter key is pressed, simulate a submit button click
            if (e.Key == Key.Enter)
                submitButton_Click(sender, e);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.Window_Closed()
        ///
        /// Event when the user close the window.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void Window_Closed(object sender, EventArgs e)
        {
            // Make sure the login window is closed
            mLoginWindow.Close();
        }
        
    }
}
