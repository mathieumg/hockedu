///////////////////////////////////////////////////////////////////////////////
/// @file PasswordPrompt.xaml.cs
/// @author Vincent Lemire
/// @date 2013-03-26
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

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class PlayModeControl
    /// @brief Popup to input a game password.
    ///
    /// @author Vincent Lemire
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public partial class PasswordPrompt : Window
    {
        // Attributes
        private bool mOkIsClicked;

        // Properties
        public string Password
        {
            get { return mPasswordBox.Password; }
        }
        public bool OkIsClicked
        {
            get { return mOkIsClicked; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void PasswordPrompt.PasswordPrompt()
        ///
        /// Constructor.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public PasswordPrompt()
        {
            InitializeComponent();
            mOkIsClicked = false;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void PasswordPrompt.ClearInput()
        ///
        /// Clear input.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void ClearInput()
        {
            mPasswordBox.Clear();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void PasswordPrompt.mCancelButton_Click()
        ///
        /// Cancel button event.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mCancelButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = false;
            Hide();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void PasswordPrompt.mOkButton_Click()
        ///
        /// Ok button event.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mOkButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = true;
            Hide();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void PasswordPrompt.SetFocus()
        ///
        /// Set focus to password text box.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void SetFocus()
        {
            mPasswordBox.Focus();
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


