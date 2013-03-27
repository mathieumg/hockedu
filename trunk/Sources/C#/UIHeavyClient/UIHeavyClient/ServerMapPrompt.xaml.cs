///////////////////////////////////////////////////////////////////////////////
/// @file ServerMapPrompt.xaml.cs
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
using HttpHockeduRequests;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class ServerMapPrompt
    /// @brief To select a map from server.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class ServerMapPrompt : Window
    {
        private bool mOkIsClicked;

        public bool OkIsClicked
        {
            get { return mOkIsClicked; }
        }

        public string SelectedMap
        {
            get { return mMapComboBox.SelectedItem.ToString(); }
        }

        public ServerMapPrompt()
        {
            InitializeComponent();
            mOkIsClicked = false;
        }

        public void HandleMaps(List<UserMapDetailedJSON> pList)
        {
            mMapComboBox.Items.Clear();
            foreach (UserMapDetailedJSON m in pList)
            {
                mMapComboBox.Items.Add(m.name);
            }
        }

        private void mCancelButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = false;
            Close();
        }

        private void mOkButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = true;
            Close();
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////

