///////////////////////////////////////////////////////////////////////////////
/// @file MainWindow.xaml.cs
/// @author Vincent Lemire
/// @date 2013-01-28
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
using System.Runtime.InteropServices;
using System.Threading;
using System.ComponentModel;

namespace UIHeavyClientPrototype
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

        public TaskManager mTaskManager = new TaskManager();

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
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitDLL();
        // Disonnects the user from the server
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DisconnectUser(string pUserName);
        
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

            messageTextBox.IsEnabledChanged += Chat.ControlEnabledChanged;
            submitButton.IsEnabledChanged += Chat.ControlEnabledChanged;
            onlineListView.IsEnabledChanged += Chat.ControlEnabledChanged;

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
            Chat.MainWindow = this;
			messageTextBox.Focus();

            ShowWholeMessage();
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
                Chat.SendMessageDLL(mUserName, messageTextBox.Text);
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
            UpdateConnectedUserList();
        }

        private void UpdateConnectedUserList()
        {
            // handle newly conected users
            foreach (string s in Chat.ConnectedUsers)
            {
                if (onlineListView.Items.IndexOf(s) == -1)
                {
                    onlineListView.Items.Add(s);
                }
            }

            // handle disconnected users
            for(int i=0; i < onlineListView.Items.Count; ++i)
            {
                string s = (string)onlineListView.Items.GetItemAt(i);
                if (Chat.ConnectedUsers.IndexOf(s) == -1)
                {
                    onlineListView.Items.RemoveAt(i);
                    --i;
                }
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
            //if (e.Key == Key.Enter)
               // submitButton_Click(sender, e);
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
            if (mUserName != "")
            {
                DisconnectUser(mUserName);
            }
            // Make sure the login window is closed
            mLoginWindow.Close();
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        public void OnDisconnect(object sender, RoutedEventArgs e)
        {
            DisconnectUser(mUserName);
            Chat.ClearContent();
            Chat.MainWindow = null;
            mUserName = "";
            mIsUserConnected = false;

            Hide();

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
            else
            {
                Chat.MainWindow = this;
                Show();
                ShowWholeMessage();
                messageTextBox.Focus();
            }
            
        }

        public void BlockUIContent()
        {
            messageTextBox.IsEnabled = false;
            submitButton.IsEnabled = false;
            onlineListView.IsEnabled = false;
        }

        public void UnBlockUIContent()
        {
            messageTextBox.IsEnabled = true;
            submitButton.IsEnabled = true;
            onlineListView.IsEnabled = true;
        }

        
    }
}
