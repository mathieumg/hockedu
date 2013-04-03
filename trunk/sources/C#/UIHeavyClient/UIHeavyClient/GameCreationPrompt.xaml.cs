///////////////////////////////////////////////////////////////////////////////
/// @file GameCreationPrompt.xaml.cs
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
    /// @class GameCreationPrompt
    /// @brief Popup to create a online game.
    ///
    /// @author Vincent Lemire
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public partial class GameCreationPrompt : Window
    {
        // Attributes
        private bool mOkIsClicked;

        // Properties
        public string GameName
        {
            get { return mNameTextBox.Text; }
        }
        public string Password
        {
            get { return mPasswordCheckBox.IsChecked.Value ? mNameTextBox.Text : ""; }
        }
        public UserMapDetailedJSON Map
        {
            get { return (mMapComboBox.SelectedItem as UserMapDetailedJSON); }
        }
        public bool OkIsClicked
        {
            get { return mOkIsClicked; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.callbackMapsFunction()
        ///
        /// Call back when maps are loaded.
        /// 
        /// @param[in] List<UserMapDetailedJSON> : Maps list.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void callbackMapsFunction(List<UserMapDetailedJSON> pList)
        {
            // Update the combobox with the list
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                foreach (UserMapDetailedJSON wItem in pList)
                {
                    mMapComboBox.Items.Add(wItem);
                }
            });
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.GameCreationPrompt()
        ///
        /// Constructor.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public GameCreationPrompt()
        {
            InitializeComponent();
            mOkIsClicked = false;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.mOkButton_Click()
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
        /// @fn void GameCreationPrompt.mCancelButton_Click()
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
        /// @fn void GameCreationPrompt.ClearInputAndLoadMapList()
        ///
        /// Reset popup.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void ClearInputAndLoadMapList()
        {
            mNameTextBox.Clear();
            mPasswordCheckBox.IsChecked = false;
            mPasswordTextBox.Clear();
            mFeedbackLabel.Content = "";
            GetServerMaps();
            mOkButton.IsEnabled = false;
            mNameTextBox.Focus();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.GetServerMaps()
        ///
        /// Asynchronusly query server's maps.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void GetServerMaps()
        {
            mMapComboBox.Items.Clear();

            // Load map list async!!!!
            HttpManager wManager = new HttpManager();
            wManager.getPublicMapList(callbackMapsFunction);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.NameChanged()
        ///
        /// On name change event, disable ok if empty.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] TextChangedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void NameChanged(object sender, TextChangedEventArgs e)
        {
            mOkButton.IsEnabled = ((sender as TextBox).Text != "" && mMapComboBox.SelectedItem != null);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.MapChanged()
        ///
        /// On map change event, disable ok if empty.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] SelectionChangedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void MapChanged(object sender, SelectionChangedEventArgs e)
        {
            mOkButton.IsEnabled = (mNameTextBox.Text != "" && (sender as ComboBox).SelectedItem != null);
        }
    }
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////

