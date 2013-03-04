///////////////////////////////////////////////////////////////////////////////
/// @file LoginWindow.xaml.cs
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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.ComponentModel;


namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @struct Server
    /// @brief A server.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-29
    ///////////////////////////////////////////////////////////////////////////
    struct Server
    {
        public string mName;
        public string mIPAdress;
        public bool isAvailable;

        public Server(string pName, string pIPAdress)
        {
            mName = pName;
            mIPAdress = pIPAdress;
            isAvailable = true;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @class LoginWindow
    /// @brief A simple window where the user will input his user name.
    ///        The server will be called to validate the name.
    ///
    /// @author Vincent Lemire
    /// @date 2013-02-12
    ///////////////////////////////////////////////////////////////////////////
    public partial class LoginWindow : Window
    {

        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CancelConnection(string pConnectionId);


        /// DEPRECATED DO NOT USE ANYMORE, USE TaskManager in MainWindowHandler
        public TaskManager mTaskManager = new TaskManager();

        // The user name input
        string mUserName = "";

        // The main window must know if the user is connected
        bool mUserConnected = false;

        bool mConnecting = false;

        // The server list
        Server[] listedServer;

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
        /// @propertie bool LoginWindow.UserConnected
        ///
        /// Propertie to know if user is connected.
        ///
        /// @return If the user is connected.
        ////////////////////////////////////////////////////////////////////////
        public bool UserConnected
        {
            get { return mUserConnected; }
            set { mUserConnected = value; }
        }

        public void SetUserMessageFeedBack(string message, bool isError)
        {
            if (isError)
            {
                errorMessageLabel.Foreground = Brushes.Red;
            }
            else
            {
                errorMessageLabel.Foreground = Brushes.White;
            }
            errorMessageLabel.Content = message;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn LoginWindow.LoginWindow()
        ///
        /// Default constructor.
        ///
        /// @return None (constructor).
        ////////////////////////////////////////////////////////////////////////
        public LoginWindow()
        {
            InitializeComponent();

            InitSavedValues();

            cancelButton.IsEnabledChanged += Chat.ControlEnabledChanged;
            loginButton.IsEnabledChanged += Chat.ControlEnabledChanged;
            userNameInput.IsEnabledChanged += Chat.ControlEnabledChanged;
            refreshButton.IsEnabledChanged += Chat.ControlEnabledChanged;
            serverComboBox.IsEnabledChanged += Chat.ControlEnabledChanged;
            ManualServerEntry.IsEnabledChanged += Chat.ControlEnabledChanged;

            refreshButton.Visibility = Visibility.Hidden;
            listedServer = new Server[]
            {
                new Server("Local", "127.0.0.1"),
                new Server("Math's house", "173.177.0.193"),
            };

            foreach (Server s in listedServer)
            {
                ComboBoxItem buffer = new ComboBoxItem();
                buffer.Content = s.mName;// +(s.isAvailable ? " (Actif)" : " (Inactif)");
                buffer.Background = serverComboBox.Background;
                buffer.Foreground = serverComboBox.Foreground;

                serverComboBox.Items.Add(buffer);
            }

            serverComboBox.SelectedIndex = 0;

            userNameInput.Focus();
        }



        void Window_Closed(object sender, EventArgs e)
        {
            Chat.SetupLoginCallBackEvents(null);
        }

        public void ConnectionSuccessful()
        {
            SetUserMessageFeedBack("Connection successful !", false);
            mUserName = userNameInput.Text;
            mUserConnected = true;
            Mouse.OverrideCursor = Cursors.Arrow;
            this.Close();
        }

        public void UserNameAlreadyChosen()
        {
            SetUserMessageFeedBack("User name already used.\nPlease choose another.", true);
            UnBlockUIContent();
        }



        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.TryConnecting()
        ///
        /// Make necessary validations and registers inputs.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void TryConnecting()
        {
            if (userNameInput.Text != "" && passwordInput.Password != "")
            {

                // Block everything while connecting
                string serverName;
                string ipAdress;
                if (ManualServerEntry.Text != "")
                {
                    ipAdress = ManualServerEntry.Text;
                    serverName = ipAdress;
                }
                else
                {
                    ipAdress = listedServer[serverComboBox.SelectedIndex].mIPAdress;
                    serverName = listedServer[serverComboBox.SelectedIndex].mName;
                }

                if (Chat.IsIPv4(ipAdress))
                {
                    Chat.mLoginInfo.mUserName = userNameInput.Text;
                    Chat.mLoginInfo.mPassword = passwordInput.Password;
                    Chat.mLoginInfo.mIpAddress = ipAdress;
                    BlockUIContent();
                    // Setup to be ready to receive events
                    Chat.SetupLoginCallBackEvents(this);

                    SetUserMessageFeedBack(String.Format("Connecting to server {0}\nPlease wait...", serverName), false);
                    Chat.RequestLogin(userNameInput.Text, passwordInput.Password, ipAdress);
                }
                else
                {
                    SetUserMessageFeedBack(String.Format("IP Address [{0}] invalid", ipAdress), true);
                }
            }
            else
            {
                SetUserMessageFeedBack("Please enter a user name and a password", true);
            }
        }

        public void BlockUIContent()
        {
            mConnecting = true;

            userNameInput.IsEnabled = false;
            loginButton.IsEnabled = false;
            refreshButton.IsEnabled = false;
            serverComboBox.IsEnabled = false;
            ManualServerEntry.IsEnabled = false;
            cancelButton.Content = "Cancel";
            Mouse.OverrideCursor = Cursors.Wait;
        }

        public void UnBlockUIContent()
        {
            // Unblock everything while connecting
            userNameInput.IsEnabled = true;
            loginButton.IsEnabled = true;
            refreshButton.IsEnabled = true;
            serverComboBox.IsEnabled = true;
            ManualServerEntry.IsEnabled = true;
            cancelButton.Content = "Exit";
            Mouse.OverrideCursor = Cursors.Arrow;

            mConnecting = false;
        }
        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.loginButton_Click()
        ///
        /// Event when the connexion button is clicked.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void loginButton_Click(object sender, RoutedEventArgs e)
        {
            TryConnecting();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.messageTextBox_KeyDown()
        ///
        /// Event when the user pressed a key.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            // If enter key is pressed, simulate a login button click
            if (e.Key == Key.Escape)
                cancelButton_Click(sender, e);
        }
        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.refreshButton_Click(object sender, RoutedEventArgs e)
        ///
        /// Event when the user pressed the refresh button.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void refreshButton_Click(object sender, RoutedEventArgs e)
        {
            foreach (Server s in listedServer)
            {
                //s.isAvailable = ... TODO : CALL DLL
            }
        }

        private void cancelButton_Click(object sender, RoutedEventArgs e)
        {
            if (mConnecting)
            {
                CancelConnection("ServerMaster");
                SetUserMessageFeedBack("Cancel requested", false);
            }
            else
            {
                Close();
            }
        }

        private void InitSavedValues()
        {
            userNameInput.Text = Chat.mLoginInfo.mUserName;
            ManualServerEntry.Text = Chat.mLoginInfo.mIpAddress;
        }



    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


