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
using System.ComponentModel;
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

        private GridViewColumnHeader mLastClickedHeader;
        private ListSortDirection mLastSortDirection;

        private Dictionary<string, string> mColumnToMember;

        // Properties
        public string GameName
        {
            get { return mNameTextBox.Text; }
        }
        public string Password
        {
            get { return mPasswordCheckBox.IsChecked.Value ? mPasswordTextBox.Password : ""; }
        }
        public UserMapDetailedJSON Map
        {
            get { return (mMapListView.SelectedItem as UserMapDetailedJSON); }
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
                    mMapListView.Items.Add(wItem);
                }
                SortListView("id", ListSortDirection.Ascending);
                mFeedbackLabel.Content = "";
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

            mColumnToMember = new Dictionary<string, string>() 
            { 
                {"Author", "id"},
                {"Name", "name"},
                {"Description", "description"},
            };
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
            mFeedbackLabel.Content = "Downloading maps, please wait...";
            mNameTextBox.Clear();
            mPasswordCheckBox.IsChecked = false;
            mPasswordTextBox.Clear();
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
            mMapListView.Items.Clear();

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
            mOkButton.IsEnabled = ((sender as TextBox).Text != "" && mMapListView.SelectedItem != null);
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
            mOkButton.IsEnabled = (mNameTextBox.Text != "" && (sender as ListView).SelectedItem != null);
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
        private void ResizeGridColumns(object sender, SizeChangedEventArgs e)
        {
            double totalWidth = (sender as ListView).ActualWidth;
            GridView grid = (sender as ListView).View as GridView;

            grid.Columns[0].Width = totalWidth * 0.25;
            grid.Columns[1].Width = totalWidth * 0.25;
            grid.Columns[2].Width = totalWidth * 0.5;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.ClickForSorting()
        ///
        /// Sorting by click.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ClickForSorting(object sender, RoutedEventArgs e)
        {
            GridViewColumnHeader clickedHeader = e.OriginalSource as GridViewColumnHeader;
            ListSortDirection sortDirection;

            if (clickedHeader != null)
            {
                if (clickedHeader.Role != GridViewColumnHeaderRole.Padding)
                {
                    if (clickedHeader != mLastClickedHeader)
                    {
                        sortDirection = ListSortDirection.Ascending;
                    }
                    else
                    {
                        if (mLastSortDirection == ListSortDirection.Ascending)
                        {
                            sortDirection = ListSortDirection.Descending;
                        }
                        else
                        {
                            sortDirection = ListSortDirection.Ascending;
                        }
                    }

                    string header = mColumnToMember[(clickedHeader.Column.Header as string)];
                    SortListView(header, sortDirection);

                    mLastClickedHeader = clickedHeader;
                    mLastSortDirection = sortDirection;
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.SortListView()
        ///
        /// Sorting by a column.
        /// 
        /// @param[in] string : Member name.
        /// @param[in] ListSortDirection : Sort direction.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void SortListView(string pSortBy, ListSortDirection pDirection)
        {
            ICollectionView items = CollectionViewSource.GetDefaultView(mMapListView.Items);

            if (items != null)
            {
                items.SortDescriptions.Clear();
                items.SortDescriptions.Add(new SortDescription(pSortBy, pDirection));
                items.Refresh();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void GameCreationPrompt.SetFocus()
        ///
        /// Give focus to name text box.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void SetFocus()
        {
            mNameTextBox.Focus();
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////

