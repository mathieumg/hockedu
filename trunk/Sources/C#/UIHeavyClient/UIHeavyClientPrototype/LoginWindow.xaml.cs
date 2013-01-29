﻿///////////////////////////////////////////////////////////////////////////////
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
    /// @class LoginWindow
    /// @brief A simple window where the user will input his user name.
    ///        The server will be called to validate the name.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class LoginWindow : Window
    {
        // The user name input
        string mUserName = "";

        // The main window must know if the user is connected
        bool mUserConnected = false;

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
            if(ValidateUser(userNameInput.Text))
            {
                mUserName = userNameInput.Text;
                mUserConnected = true;
                this.Close();
            }
            else
            {
                // Show an error message if connexion failed
                errorMessageLabel.Content = "Ce pseudonyme est déjà utilisé.\nVeuillez en choisir un autre.";
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
        private bool ValidateUser(string pUserName)
        {
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


