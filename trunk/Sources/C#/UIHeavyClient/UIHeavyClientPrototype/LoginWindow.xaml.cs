///////////////////////////////////////////////////////////////////////////////
/// @file LoginWindow.xaml.cs
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
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.ComponentModel;


namespace UIHeavyClientPrototype
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
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class LoginWindow : Window
    {
        // sends a request to connect the user. Will not be necessarly connected when exiting this function
        // must wait for a callback indicating the status of this user's connection
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RequestLogin( string pUsername, string pIpAdress );

        public TaskManager mTaskManager = new TaskManager();

        // The user name input
        string mUserName = "";

        // The main window must know if the user is connected
        bool mUserConnected = false;

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

            listedServer = new Server[]
            {
                new Server("Local", "127.0.0.1"),
                new Server("Chez Math", "173.177.0.193"),
            };

            foreach (Server s in listedServer)
            {
                ComboBoxItem buffer = new ComboBoxItem();
                buffer.Content = s.mName + (s.isAvailable ? " (Actif)" : " (Inactif)");
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
            mUserName = userNameInput.Text;
            mUserConnected = true;
            Mouse.OverrideCursor = Cursors.Arrow;
            this.Close();
        }

        public void UserNameAlreadyChosen()
        {
            errorMessageLabel.Content = "Ce pseudonyme est déjà utilisé.\nVeuillez en choisir un autre.";
            UnBlockUIContent();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.TryConnecting()
        ///
        /// Make necessary validations and registers inputs.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void TryConnecting()
        {
            // Block everything while connecting
            BlockUIContent();

            // Setup to be ready to receive events
            Chat.SetupLoginCallBackEvents(this);
            RequestLogin(userNameInput.Text, listedServer[serverComboBox.SelectedIndex].mIPAdress);
        }

        public void BlockUIContent()
        {
            userNameInput.IsEnabled = false;
            loginButton.IsEnabled = false;
            refreshButton.IsEnabled = false;
            errorMessageLabel.Content = "Connecting to server, please wait...";
            serverComboBox.IsEnabled = false;
            Mouse.OverrideCursor = Cursors.Wait;
        }

        public void UnBlockUIContent()
        {
            // Unblock everything while connecting
            userNameInput.IsEnabled = true;
            loginButton.IsEnabled = true;
            refreshButton.IsEnabled = true;
            serverComboBox.IsEnabled = true;
            Mouse.OverrideCursor = Cursors.Arrow;
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
        private void messageTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            // If enter key is pressed, simulate a login button click
            if (e.Key == Key.Enter)
                loginButton_Click(sender, e);
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

    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


