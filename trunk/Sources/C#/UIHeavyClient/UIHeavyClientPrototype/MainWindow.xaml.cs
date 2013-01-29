﻿///////////////////////////////////////////////////////////////////////////////
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
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitDLL(string username);

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


        ////////////////////////////////////////////////////////////////////////////////////
        //Callback to received event from C++
        //declare the callback prototype
        delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message);
        [DllImport(@"INF2990.dll")]
        static extern void SetCallback(MessageReceivedCallBack callback);
        static bool MessageReceived(IntPtr pUsername, IntPtr pMessage)
        {
            string message = Marshal.PtrToStringAnsi(pMessage);
            string username = Marshal.PtrToStringAnsi(pUsername);
            Chat.UpdateChat(username, message);
            return true;
        }
        MessageReceivedCallBack mCallback = MessageReceived;
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
            SetCallback(mCallback);
			messageTextBox.Focus();
            InitDLL(UserName);

            // Worker pour faire le rafraichissement de la fenetre
            BackgroundWorker mBgWorker = new BackgroundWorker();
            mBgWorker.WorkerReportsProgress = true;
            mBgWorker.DoWork +=
                new DoWorkEventHandler(bw_DoWork);
            mBgWorker.ProgressChanged +=
                new ProgressChangedEventHandler(bw_ProgressChanged);
            mBgWorker.RunWorkerAsync();
        }

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
                messageTextBox.Clear();
            }
        }

        public void ShowWholeMessage()
        {
            wholeMessageBox.Text = Chat.WholeMessage;
            currentOnlineTextBox.Text = Chat.ConnectedUsers;
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
