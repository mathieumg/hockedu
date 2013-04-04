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
using System.Collections;
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
    /// @class ServerMapPrompt
    /// @brief To select a map from server.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class ServerMapPrompt : Window
    {
        // Attributes
        private bool mOkIsClicked;

        private GridViewColumnHeader mLastClickedHeader;
        private ListSortDirection mLastSortDirection;

        private Dictionary<string, string> mColumnToMember;

        // Properties
        public bool OkIsClicked
        {
            get { return mOkIsClicked; }
        }
        public UserMapDetailedJSON SelectedMap
        {
            get { return (mMapListView.SelectedItem as UserMapDetailedJSON); }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void ServerMapPrompt.ServerMapPrompt()
        ///
        /// Constructor.
        ///
        /// @return none.
        ////////////////////////////////////////////////////////////////////////
        public ServerMapPrompt()
        {
            InitializeComponent();
            mOkIsClicked = false;

            mLastClickedHeader = null;
            mLastSortDirection = ListSortDirection.Ascending;

            mColumnToMember = new Dictionary<string, string>() 
            { 
                {"Id", "id"},
                {"Name", "name"},
                {"Description", "description"},
            };
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void ServerMapPrompt.callbackMapsFunction()
        ///
        /// Send message to chat.
        /// 
        /// @param[in] List<UserMapDetailedJSON> : The map infos list.
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
        /// @fn void ServerMapPrompt.mCancelButton_Click()
        ///
        /// Cancel map select.
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
        /// @fn void ServerMapPrompt.mOkButton_Click()
        ///
        /// Map select.
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
        /// @fn void ServerMapPrompt.GetServerMaps()
        ///
        /// Asynchronus query server's maps.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void GetServerMaps()
        {
            mFeedbackLabel.Content = "Downloading maps, please wait...";
            mMapListView.Items.Clear();

            // Load map list async!!!!
            HttpManager wManager = new HttpManager();
            wManager.getPublicMapList(callbackMapsFunction);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void ServerMapPrompt.ResizeGridColumns()
        ///
        /// Columns resize.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] SizeChangedEventArgs : The event.
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
        /// @fn void ServerMapPrompt.ClickForSorting()
        ///
        /// Sort by click on header.
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
        /// @fn void ServerMapPrompt.SortListView()
        ///
        /// Sort by a column.
        /// 
        /// @param[in] string : Member's name.
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
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////

