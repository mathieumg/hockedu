///////////////////////////////////////////////////////////////////////////////
/// @file SaveServerMapPrompt.xaml.cs
/// @author Vincent Lemire
/// @date 2013-04-04
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

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class SaveServerMapPrompt
    /// @brief Popup to save a map to the web server.
    ///
    /// @author Vincent Lemire
    /// @date 2013-04-04
    ///////////////////////////////////////////////////////////////////////////
    public partial class SaveServerMapPrompt : Window
    {
        

        private bool mOkIsClicked;

        public bool OkIsClicked
        {
            get { return mOkIsClicked; }
        }

        public string MapName
        {
            get { return mNameTextBox.Text; }
            set { mNameTextBox.Text = value; }
        }
        public string MapDescription
        {
            get { return mDescriptionTextBox.Text; }
            set { mDescriptionTextBox.Text = value; }
        }
        public bool IsPublic
        {
            get { return mPublicCheckBox.IsChecked.Value; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void SaveServerMapPrompt.SaveServerMapPrompt()
        ///
        /// Constructor.
        ///
        /// @return none.
        ////////////////////////////////////////////////////////////////////////
        public SaveServerMapPrompt()
        {
            InitializeComponent();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void SaveServerMapPrompt.GiveFocus()
        ///
        /// Give focus to map name textbox.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void GiveFocus()
        {
            mNameTextBox.Clear();
            mDescriptionTextBox.Clear();
            mPublicCheckBox.IsChecked = false;

            mNameTextBox.Focus();
            mOkButton.IsDefault = true;
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
