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
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class LoginWindow : Window
    {
        // sends a request to connect the user. Will not be necessarly connected when exiting this function
        // must wait for a callback indicating the status of this user's connection
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RequestLogin( string pUsername, string pIpAdress );

        ////////////////////////////////////////////////////////////////////////////////////
        // Callback to received event messages from C++
        // declare the callback prototype
        // use this callback to validate user's connection
        [DllImport(@"INF2990.dll")]
        static extern void SetEventCallback(MainWindow.EventReceivedCallBack callback);
        static bool EventReceived(int id, IntPtr pMessage)
        {
            if(id >= 0 && MainWindow.EventType.NB_ELEM.CompareTo(id) < 0)
            {
                string message = Marshal.PtrToStringAnsi(pMessage);
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
            return true;
        }
        MainWindow.EventReceivedCallBack mEventCallback = EventReceived;
        ////////////////////////////////////////////////////////////////////////////////////

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
                new Server("Chez Joe", "437.0.69.1"),
            };

            foreach (Server s in listedServer)
            {
                serverComboBox.Items.Add(s.mName);
            }

            serverComboBox.SelectedIndex = 0;

            userNameInput.Focus();
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
            if(ValidateUser(userNameInput.Text, listedServer[serverComboBox.SelectedIndex].mIPAdress))
            {
                mUserName = userNameInput.Text;
                mUserConnected = true;
                Mouse.OverrideCursor = Cursors.Arrow;
                this.Close();
            }
            else
            {
                // Show an error message if connexion failed
                errorMessageLabel.Content = "Ce pseudonyme est déjà utilisé.\nVeuillez en choisir un autre.";

                // Unblock everything while connecting
                userNameInput.IsEnabled = true;
                loginButton.IsEnabled = true;
                serverComboBox.IsEnabled = true;
                Mouse.OverrideCursor = Cursors.Arrow;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn bool LoginWindow.ValidateUser()
        ///
        /// Call the server to validate the user name.
        /// 
        /// @param[in] string : The user name.
        ///
        /// @return If the connexion succed.
        ////////////////////////////////////////////////////////////////////////
        private bool ValidateUser(string pUserName, string pIPAdress)
        {
            // Block everything while connecting
            userNameInput.IsEnabled = false;
            loginButton.IsEnabled = false;
            errorMessageLabel.Content = "Connecting to server, please wait...";
            serverComboBox.IsEnabled = false;
            Mouse.OverrideCursor = Cursors.Wait;

            // TODO : CALL DLL
            // ...

            return pUserName != ""; // TEMP
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
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


