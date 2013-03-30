///////////////////////////////////////////////////////////////////////////////
/// @file LoginControl.xaml.cs
/// @author Vincent Lemire
/// @date 2013-02-26
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
            mName=pName;
            mIPAdress=pIPAdress;
            isAvailable=true;
        }
    }


    ///////////////////////////////////////////////////////////////////////////
    /// @struct LoginControlSavedInfo
    /// @brief To handle login infos.
    ///
    /// @author Michael Ferris
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public struct LoginControlSavedInfo
    {
        public string mUserName;
        public string mPassword;
        public string mIpAddress;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @class LoginControl
    /// @brief A simple window where the user will input his user name.
    ///        The server will be called to validate the name.
    ///
    /// @author Vincent Lemire
    /// @date 2013-02-12
    ///////////////////////////////////////////////////////////////////////////
    public partial class LoginControl : UserControl
    {
        public LoginControlSavedInfo mLoginInfo = new LoginControlSavedInfo();

        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CancelConnection(string pConnectionId);
        [DllImport(@"RazerGame.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void RequestLogin(string pUsername, string pPassword, string pIpAdress);
        [DllImport(@"RazerGame.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void DisconnectMasterServer();

        // The user name input
        string mUserName = "";

        // The main window must know if the user is connected
        bool mUserConnected = false;

        bool mConnecting = false;

        // The server list
        Server[] listedServer;

        ////////////////////////////////////////////////////////////////////////
        /// @propertie string LoginControl.UserName
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
        /// @propertie bool LoginControl.UserConnected
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
        /// @fn LoginControl.LoginControl()
        ///
        /// Default constructor.
        ///
        /// @return None (constructor).
        ////////////////////////////////////////////////////////////////////////
        public LoginControl()
        {
            InitializeComponent();

            InitSavedValues();

            cancelButton.IsEnabledChanged      += ControlEnabledChanged;
            loginButton.IsEnabledChanged       += ControlEnabledChanged;
            userNameInput.IsEnabledChanged     += ControlEnabledChanged;
            serverComboBox.IsEnabledChanged    += ControlEnabledChanged;
            ManualServerEntry.IsEnabledChanged += ControlEnabledChanged;

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



        public static void ControlEnabledChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            Control control = sender as Control;
            if ((bool)e.NewValue)
            {
                control.Foreground = Brushes.White;
            }
            else
            {
                control.Foreground = Brushes.Black;
            }
        }

        public void ConnectionSuccessful()
        {
            SetUserMessageFeedBack("Connection successful !", false);
            mUserName = userNameInput.Text;
            mUserConnected = true;
            Mouse.OverrideCursor = Cursors.Arrow;
            MainWindowHandler.GoToOnlineLobby();
        }

        public void UserNameAlreadyChosen()
        {
            SetUserMessageFeedBack("User name already used.\nPlease choose another.", true);
            UnBlockUIContent();
        }



        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.TryConnecting()
        ///
        /// Make necessary validations and registers inputs.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void TryConnecting()
        {
            if (userNameInput.Text != "" && passwordInput.Password != "")
            {
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

                if (Utilities.IsIPv4(ipAdress))
                {
                    mLoginInfo.mUserName  = userNameInput.Text;
                    mLoginInfo.mPassword  = passwordInput.Password;
                    mLoginInfo.mIpAddress = ipAdress;
                    BlockUIContent();

                    SetUserMessageFeedBack(String.Format("Connecting to server {0}\nPlease wait...", serverName), false);
                    RequestLogin(userNameInput.Text, passwordInput.Password, ipAdress);
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
            serverComboBox.IsEnabled = true;
            ManualServerEntry.IsEnabled = true;
            cancelButton.Content = "Exit";
            Mouse.OverrideCursor = Cursors.Arrow;
            mConnecting = false;
        }
        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.loginButton_Click()
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
        /// @fn void LoginControl.messageTextBox_KeyDown()
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

        private void cancelButton_Click(object sender, RoutedEventArgs e)
        {
            if (mConnecting)
            {
                CancelConnection("ServerMaster");
                SetUserMessageFeedBack("Cancel requested", false);
                mConnecting = false;
                UnBlockUIContent();
            }
        }

        private void InitSavedValues()
        {
            userNameInput.Text     = mLoginInfo.mUserName;
            ManualServerEntry.Text = mLoginInfo.mIpAddress;
        }

        public void SetFocusToUserName()
        {
            userNameInput.Focus();
        }



        // Callbacks for the Login window
        public static bool LoginControlEventReceived(EventCodes id, IntPtr pMessage)
        {
            if ( id >= 0 && EventCodes.NB_EVENT_CODES > id )
            {
                string message=Marshal.PtrToStringAnsi(pMessage);
                EventCodes type=(EventCodes)id;
                LoginControl wThis=MainWindowHandler.Context.MainMenuControl.LoginControlElement;
                switch (type)
                {
                case EventCodes.USER_CONNECTED:
                {
                        wThis.mConnecting = false;
                        // Signal à la fenetre l'événement
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            wThis.ConnectionSuccessful();
                            wThis.UnBlockUIContent();
                        });

                        break;
                }
                case EventCodes.USER_ALREADY_CONNECTED:
                {
                    wThis.mConnecting=false;
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.UserNameAlreadyChosen();
                    });
                    break;
                }
                case EventCodes.USER_DID_NOT_SEND_NAME_ON_CONNECTION:
                {
                    wThis.mConnecting=false;
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Connection Error", true);
                        wThis.UnBlockUIContent();
                    });
                    break;
                }
                case EventCodes.USER_DISCONNECTED:
                {
                    wThis.mConnecting=false;
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Connection Error", true);
                        wThis.UnBlockUIContent();
                    });
                    break;
                }
                case EventCodes.CONNECTION_CANCELED:
                {
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Connection Canceled", false);
                        wThis.UnBlockUIContent();
                    });
                    break;
                }
                case EventCodes.RECONNECTION_TIMEOUT:
                {
                    // On ne veut pas afficher des vieux events quand on n'attend plus pour se connecter
                    if (wThis.mConnecting)
                    {
                        // Signal à la fenetre l'événement
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            wThis.SetUserMessageFeedBack("Connection Timed out", true);
                            wThis.UnBlockUIContent();
                        });
                    }
                    wThis.mConnecting=false;
                    break;
                }
                case EventCodes.RECONNECTION_IN_PROGRESS: break;
                case EventCodes.WRONG_PASSWORD:
                {
                    wThis.mConnecting=false;
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Wrong username/password", true);
                        wThis.UnBlockUIContent();
                    });
                    break;
                }
                default: break;
                }
            }
            return true;
        }




    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


