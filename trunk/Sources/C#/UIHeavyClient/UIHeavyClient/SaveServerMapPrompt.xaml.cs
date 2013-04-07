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
            mNameTextBox.Focus();
            mOkButton.IsDefault = true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
