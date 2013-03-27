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
        private bool mOkIsClicked;

        public string Password
        {
            get { return mPasswordBox.Password; }
        }
        public bool OkIsClicked
        {
            get { return mOkIsClicked; }
        }

        public PasswordPrompt()
        {
            InitializeComponent();
            mOkIsClicked = false;
        }

        public void ClearInput()
        {
            mPasswordBox.Clear();
        }

        private void mCancelButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = false;
            Hide();
        }

        private void mOkButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = true;
            Hide();
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


