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
using System.Security.Cryptography;
using HttpHockeduRequests;
using System.Threading;
using System.Net.NetworkInformation;
using System.Net;
using System.Diagnostics;

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

    public delegate void ConnectionSuccessfulCallback();

    ///////////////////////////////////////////////////////////////////////////
    /// @struct LoginControlSavedInfo
    /// @brief To handle login infos.
    ///
    /// @author Michael Ferris
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public struct LoginControlSavedInfo
    {
        public LoginControlSavedInfo(int i)
        {
            mUserName = "";
            mPassword = "";
            mIpAddress = "";


            mAuthOnWeb = false;
            mUserId = null;
            mAuthKey = null;
            mKeyExpiration = null;
        }
        public string mUserName;
        public string mPassword;  /// Password hashed with SHA1
        public string mIpAddress;


        public bool mAuthOnWeb;
        public string mUserId; /// not authenticated if null
        public string mAuthKey;
        public string mKeyExpiration;
    }

    public class ConnectionRequest
    {
        public void CancelRequest()
        {
            if ( mWebAuthThread != null)
            {
                mWebAuthThread.Abort();
            }
            if ( !mWebAuthOnly )
            {
                LoginControl.CancelConnection( "ServerMaster" );
            }
        }

        public void ConnectionSuccess()
        {
            if ( mConnectionSuccessfulCallback != null )
            {
                mConnectionSuccessfulCallback();
            }
        }

        public bool mWebAuthOnly = false;
        public Thread mWebAuthThread = null;
        public ConnectionSuccessfulCallback mConnectionSuccessfulCallback = null;
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
        static public LoginControlSavedInfo mLoginInfo = new LoginControlSavedInfo(0);
        /// can only do one request at a time
        static public ConnectionRequest mCurrentRequest = null;

        // C++ functions
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

        bool mAuthWebOnly = false;
        public ConnectionSuccessfulCallback mConnectionSuccessfulCallback = null;

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

        ////////////////////////////////////////////////////////////////////////
        /// @propertie bool LoginControl.AuthWebOnly
        ///
        /// Propertie to know if the control is used only to authenticate on the web
        ///
        /// @return If the control state
        ////////////////////////////////////////////////////////////////////////
        public bool AuthWebOnly
        {
            get { return mAuthWebOnly; }
            set { mAuthWebOnly = value; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.SetUserMessageFeedBack()
        ///
        /// Feedback message.
        /// 
        /// @param[in] string : The message.
        /// @param[in] bool : Is it an error?.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
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
        public LoginControl(ConnectionSuccessfulCallback connectionSuccessfulCallback,bool onlyWebAuth = false)
        {
            InitializeComponent();

            InitSavedValues();


            mConnectionSuccessfulCallback = connectionSuccessfulCallback;
            AuthWebOnly = onlyWebAuth;
            if ( AuthWebOnly )
            {
                ServerLabel.Visibility = System.Windows.Visibility.Collapsed;
                serverComboBox.Visibility = System.Windows.Visibility.Collapsed;
                ManualServerEntryLabel.Visibility = System.Windows.Visibility.Collapsed;
                ManualServerEntry.Visibility = System.Windows.Visibility.Collapsed;
            }
            else
            {
                serverComboBox.IsEnabledChanged += ControlEnabledChanged;
                ManualServerEntry.IsEnabledChanged += ControlEnabledChanged;
                listedServer = new Server[]
                {
                    new Server("Main Server", "Hockedu.com"/*"173.231.120.124"*/),
                    new Server("Local", "127.0.0.1"),
                };

                foreach ( Server s in listedServer )
                {
                    ComboBoxItem buffer = new ComboBoxItem();
                    buffer.Content = s.mName;// +(s.isAvailable ? " (Actif)" : " (Inactif)");
                    buffer.Background = serverComboBox.Background;
                    buffer.Foreground = serverComboBox.Foreground;

                    serverComboBox.Items.Add( buffer );
                }

                serverComboBox.SelectedIndex = 0;
            }

            cancelButton.IsEnabledChanged      += ControlEnabledChanged;
            loginButton.IsEnabledChanged       += ControlEnabledChanged;
            userNameInput.IsEnabledChanged     += ControlEnabledChanged;

            UnBlockUIContent();
            userNameInput.Focus();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.ControlEnabledChanged()
        ///
        /// Ajust UI colors.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] DependencyPropertyChangedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.ConnectionSuccessful()
        ///
        /// Operations when connection is successful.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void ConnectionSuccessful()
        {
            SetUserMessageFeedBack("Connection successful !", false);
            mUserName = userNameInput.Text;
            mUserConnected = true;
            Mouse.OverrideCursor = Cursors.Arrow;
            if ( mCurrentRequest != null )
            {
                mCurrentRequest.ConnectionSuccess();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.UserNameAlreadyChosen()
        ///
        /// Operation when login is already in use.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
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
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void TryConnecting()
        {
            if ( mCurrentRequest != null )
            {
                mCurrentRequest.CancelRequest();
            }
            mCurrentRequest = null;
            if (userNameInput.Text != "" && passwordInput.Password != "")
            {
                if ( AuthWebOnly )
                {
                    BlockUIContent();
                    mCurrentRequest = new ConnectionRequest();
                    mLoginInfo.mUserName = userNameInput.Text;
                    mLoginInfo.mPassword = CalculateSHA1( passwordInput.Password );
                    mCurrentRequest.mConnectionSuccessfulCallback = mConnectionSuccessfulCallback;
                    mCurrentRequest.mWebAuthOnly = true;
                    AuthenticateToWeb( mCurrentRequest );

                    SetUserMessageFeedBack( String.Format( "Connecting to server \nPlease wait..." ), false );
                }
                else
                {
                    string serverName;
                    string ipAdress;


                    if ( ManualServerEntry.Text != "" )
                    {
                        ipAdress = ManualServerEntry.Text;
                        serverName = ipAdress;
                        //IPAddress[] ips = Dns.GetHostAddresses( ManualServerEntry.Text );
                    }
                    else
                    {
                        ipAdress = listedServer[serverComboBox.SelectedIndex].mIPAdress;
                        serverName = listedServer[serverComboBox.SelectedIndex].mName;

                        if ( serverComboBox.SelectedIndex == 1 )
                        {
                            MainWindowHandler.StartServers();
                        }
                    }
                    IPAddress[] ips = Dns.GetHostAddresses( ipAdress );

                    if ( ips.Count() > 0)
                    {
                        ipAdress = ips[0].ToString();
                        if ( !Utilities.IsIPv4( ipAdress ) )
                        {
                            SetUserMessageFeedBack( String.Format( "IP Address [{0}] invalid", ipAdress ), true );
                            return;
                        }
                        else
                        {
                            mCurrentRequest = new ConnectionRequest();

                            mLoginInfo.mUserName = userNameInput.Text;
                            mLoginInfo.mPassword = CalculateSHA1( passwordInput.Password );
                            mLoginInfo.mIpAddress = ipAdress;
                            BlockUIContent();
                            mCurrentRequest.mConnectionSuccessfulCallback = mConnectionSuccessfulCallback;
                            AuthenticateToWeb( mCurrentRequest );

                            SetUserMessageFeedBack( String.Format( "Connecting to server {0}\nPlease wait...", serverName ), false );
                            RequestLogin( userNameInput.Text, passwordInput.Password, ipAdress );
                        }
                    }
                }
            }
            else
            {
                SetUserMessageFeedBack("Please enter a user name and a password", true);
            }
        }

        public static void AuthenticateToWeb(ConnectionRequest request)
        {
            HttpManager wManager = new HttpManager();
            request.mWebAuthThread = wManager.authenticate( mLoginInfo.mUserName, mLoginInfo.mPassword, AuthenticateCallback );
        }

        public static void AuthenticateCallback( HttpHockeduRequests.AuthentificationJSON response )
        {
            mLoginInfo.mAuthOnWeb = false;
            if ( response != null )
            {
                mLoginInfo.mUserId = response.id_user;
                mLoginInfo.mAuthKey = response.auth_key;
                mLoginInfo.mKeyExpiration = response.auth_key_expiration;
                mLoginInfo.mAuthOnWeb = 
                    !string.IsNullOrEmpty(mLoginInfo.mUserId) && 
                    !string.IsNullOrEmpty(mLoginInfo.mAuthKey) && 
                    !string.IsNullOrEmpty(mLoginInfo.mKeyExpiration);
            }

            if ( mLoginInfo.mAuthOnWeb )
            {
                /// en tout temps, si la connection reussi, on sync les achievements
                MainWindowHandler.SynchroniseAchievements();
            }


            if ( mCurrentRequest != null && mCurrentRequest.mWebAuthOnly )
            {
                if ( mLoginInfo.mAuthOnWeb )
                {
                    // si on tente uniquement de ce connecter au web on l'indique, sinon, la vraie requete est pour le serveur maitre
                    mCurrentRequest.ConnectionSuccess();
                }
                else
                {
                    /// ici on assume qu'on est dans une fenetre de weblogin
                    WebLogin webLogin = WebLogin.getInstance();
                    if ( webLogin != null )
                    {
                        webLogin.mLoginControl.UnBlockUIContent();
                        if ( response != null && response.error != null )
                        {
                            webLogin.mLoginControl.SetUserMessageFeedBack( "Error: " + response.error,true );
                        }
                        else
                        {
                            webLogin.mLoginControl.SetUserMessageFeedBack( "Unknown error", true );
                        }
                    }
                }
            }
        }

        public static string CalculateSHA1( string text )
        {
            byte[] buffer = Encoding.UTF8.GetBytes( text );
            SHA1CryptoServiceProvider cryptoTransformSHA1 = new SHA1CryptoServiceProvider();
            return BitConverter.ToString( cryptoTransformSHA1.ComputeHash( buffer ) ).Replace("-","").ToLower();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.BlockUIContent()
        ///
        /// DIsable some UI content while connecting.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void BlockUIContent()
        {
            userNameInput.IsEnabled = false;
            loginButton.IsEnabled = false;
            serverComboBox.IsEnabled = false;
            ManualServerEntry.IsEnabled = false;
            cancelButton.Content = "Cancel";
            Mouse.OverrideCursor = Cursors.Wait;

            cancelButton.Visibility = Visibility.Visible;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.UnBlockUIContent()
        ///
        /// Enable UI content.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void UnBlockUIContent()
        {
            // Unblock everything while connecting
            userNameInput.IsEnabled = true;
            loginButton.IsEnabled = true;
            serverComboBox.IsEnabled = true;
            ManualServerEntry.IsEnabled = true;
            cancelButton.Content = "Exit";
            Mouse.OverrideCursor = Cursors.Arrow;

            if ( !mAuthWebOnly )
            {
                cancelButton.Visibility = Visibility.Hidden;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.loginButton_Click()
        ///
        /// Event when the connexion button is clicked.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void loginButton_Click(object sender, RoutedEventArgs e)
        {
            TryConnecting();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.OnKeyDown()
        ///
        /// Event when the user pressed a key.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] KeyEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            // If enter key is pressed, simulate a login button click
            if (e.Key == Key.Escape)
                cancelButton_Click(sender, e);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.cancelButton_Click()
        ///
        /// Event when connection is canceled.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void cancelButton_Click(object sender, RoutedEventArgs e)
        {
            if (mCurrentRequest != null)
            {
                mCurrentRequest.CancelRequest();
                SetUserMessageFeedBack("Cancel requested", false);
                mCurrentRequest = null;
            }
            else
            {
                WebLogin wl = WebLogin.getInstance();
                if (wl != null)
                {
                    wl.Close();
                }
            }
            UnBlockUIContent();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.InitSavedValues()
        ///
        /// Display user values.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void InitSavedValues()
        {
            userNameInput.Text     = mLoginInfo.mUserName;
            ManualServerEntry.Text = mLoginInfo.mIpAddress;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.SetFocusToUserName()
        ///
        /// Focus on user name textbox.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void SetFocusToUserName()
        {
            userNameInput.Focus();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.LoginControlEventReceived()
        ///
        /// Event callback for connection.
        /// 
        /// @param[in] EventCodes : The event Id.
        /// @param[in] IntPtr : The event message.
        ///
        /// @return bool.
        ////////////////////////////////////////////////////////////////////////
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
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        if ( mCurrentRequest != null )
                        {
                            wThis.ConnectionSuccessful();
                        }
                        mCurrentRequest = null;
                        wThis.UnBlockUIContent();
                    });

                    break;
                }
                case EventCodes.USER_ALREADY_CONNECTED:
                {
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.UserNameAlreadyChosen();
                        mCurrentRequest = null;
                    } );
                    break;
                }
                case EventCodes.USER_DID_NOT_SEND_NAME_ON_CONNECTION:
                {
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Connection Error", true);
                        wThis.UnBlockUIContent();
                        mCurrentRequest = null;
                    } );
                    break;
                }
                case EventCodes.USER_DISCONNECTED:
                {
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Connection Error", true);
                        wThis.UnBlockUIContent();
                        mCurrentRequest = null;
                    } );
                    break;
                }
                case EventCodes.CONNECTION_CANCELED:
                {
                    // Signal à la fenetre l'événement
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Connection Canceled", false);
                        wThis.UnBlockUIContent();
                        mCurrentRequest = null;
                    } );
                    break;
                }
                case EventCodes.RECONNECTION_TIMEOUT:
                {
                    // On ne veut pas afficher des vieux events quand on n'attend plus pour se connecter
                    if ( mCurrentRequest != null )
                    {
                        // Signal à la fenetre l'événement
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            wThis.SetUserMessageFeedBack("Connection Timed out", true);
                            wThis.UnBlockUIContent();
                        } );
                        mCurrentRequest = null;
                    }
                    break;
                }
                case EventCodes.RECONNECTION_IN_PROGRESS: break;
                case EventCodes.WRONG_PASSWORD:
                {
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Wrong username/password", true);
                        wThis.UnBlockUIContent();
                        mCurrentRequest = null;
                    } );
                    break;
                }
                case EventCodes.DATABASE_CONNECTION_ERROR:
                {
                    MainWindowHandler.mTaskManager.ExecuteTask(() =>
                    {
                        wThis.SetUserMessageFeedBack("Database connection error (server side)", true);
                        wThis.UnBlockUIContent();
                        mCurrentRequest=null;
                    });
                    break;
                }
                default: break;
                }
            }
            return true;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginControl.mProfileButton_Click()
        ///
        /// Open web site in browser.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mProfileButton_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.hockedu.com");
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


