///////////////////////////////////////////////////////////////////////////////
/// @file PlayModeControl.xaml.cs
/// @author Vincent Lemire
/// @date 2013-02-06
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class RadioOptionControl
    /// @brief Window to configure radio.
    ///
    /// @author Vincent Lemire
    /// @date 2013-02-06
    ///////////////////////////////////////////////////////////////////////////
    public partial class RadioOptionControl : UserControl
    {
        private Dictionary<object, string> mGuidanceMessages;

        // Playlist Handling
        private ObservableCollection<string> mPlaylistNames;
        private ObservableCollection<string> mSelectedPlaylistContent;
        private Dictionary<string, ObservableCollection<string>> mPlaylistsContent;
        private string mSelectedPlaylist;

        private Microsoft.Win32.OpenFileDialog mOpenFileDialog;

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlaylists();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void GetRadioPlaylists([In, Out] string[] pPlaylists, int pNbrPlaylists);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrSongs(string pPlaylist);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void GetPlaylistSongs(string pPlaylist, [In, Out] string[] pSongs, int pNbrSongs);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void RemoveRadioPlaylist(string pPlaylist);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void AddRadioPlaylist(string pPlaylist, string[] pSongs, int pNbrSongs);

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.RadioOptionControl()
        ///
        /// Constructor.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public RadioOptionControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mPlaylistAddButton, "Add a new playlist"},
                {mPlaylistDeleteButton, "Delete selected playlist"},
                {mSongAddButton, "Add a new song"},
                {mSongDeleteButton, "Delete selected song"},
                {mBackToOptionButton, "Return to option menu"},
                {mBackToMainButton, "Return to main menu"},
            };

            mPlaylistNames = new ObservableCollection<string>();
            mPlaylistsContent = new Dictionary<string, ObservableCollection<string>>();

            mOpenFileDialog = new Microsoft.Win32.OpenFileDialog();
            mOpenFileDialog.Title = "Select a song file";
            mOpenFileDialog.Multiselect = true;
            mOpenFileDialog.Filter = "Sound file(*.mp3;*.wma;*.wav;*.xm)|*.mp3;*.wma;*.wav;*.xm";
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mBackToOptionButton_Click()
        ///
        /// Return to option menu.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mBackToOptionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOptionsMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mBackToMainButton_Click()
        ///
        /// Return to main menu.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mBackToMainButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.DisplayGuidanceMessages()
        ///
        /// Display Bastien & Scapin.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] MouseEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void DisplayGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = mGuidanceMessages[sender];
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.ClearGuidanceMessages()
        ///
        /// Clear Bastien & Scapin.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] MouseEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ClearGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = "";
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.DisplayPlaylists()
        ///
        /// Get playlists from DLL.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void DisplayPlaylists()
        {
            // Clear what's on screen
            ClearPlaylistInfo();
            mPlaylistsListView.Items.Clear();
            mPlaylistNames.Clear();
            mPlaylistsContent.Clear();

            // Get playlists count and names
            int nbrPlaylists = GetNbrPlaylists();

            string[] playlists = new string[nbrPlaylists];
            for (int i = 0; i < nbrPlaylists; ++i)
            {
                playlists[i] = new string('s', 255);
            }

            GetRadioPlaylists(playlists, nbrPlaylists);

            // For each playlist...
            foreach (string s in playlists)
            {
                // Add it to collections and UI
                mPlaylistNames.Add(s);
                mPlaylistsContent.Add(s, new ObservableCollection<string>());
                mPlaylistsListView.Items.Add(s);

                // Get songs count and names
                int nbrSongs = GetNbrSongs(s);

                string[] songs = new string[nbrSongs];
                for (int i = 0; i < nbrSongs; ++i)
                {
                    songs[i] = new string('r', 255);
                }

                GetPlaylistSongs(s, songs, nbrSongs);

                foreach (string s2 in songs)
                {
                    mPlaylistsContent[s].Add(s2);
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mPlaylistAddButton_Click()
        ///
        /// Add a new playlist.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mPlaylistAddButton_Click(object sender, RoutedEventArgs e)
        {
            // Make sure that the new name doesn't exist
            string newName = "New Playlist";
            int i = 1;
            while(mPlaylistNames.Contains(newName))
            {
                newName = "New Playlist " + (++i);
            }

            AddRadioPlaylist(newName, new string[0], 0);
            DisplayPlaylists();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mPlaylistDeleteButton_Click()
        ///
        /// Remove a playlist.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mPlaylistDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if (mSelectedPlaylist != "")
            {
                string buffer;
                int count = mPlaylistsListView.SelectedItems.Count - 1;
                for (int i = count; i >= 0; --i)
                {
                    buffer = (mPlaylistsListView.SelectedItems[i] as string);
                    mSelectedPlaylistContent.Remove(buffer);
                    mPlaylistsListView.Items.Remove((buffer as string));

                    RemoveRadioPlaylist(buffer);
                }   
            }
            DisplayPlaylists();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mCurrentPlaylistsList_SelectionChanged()
        ///
        /// Change selected playlist.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] SelectionChangedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mCurrentPlaylistsList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ClearPlaylistInfo();
            if ((sender as ListView).SelectedItems.Count > 0)
            {
                mSelectedPlaylist = (sender as ListView).SelectedItem.ToString();
                mSelectedPlaylistContent = mPlaylistsContent[mSelectedPlaylist];

                mPlaylistNameTextBox.Text = mSelectedPlaylist;

                foreach (string s in mSelectedPlaylistContent)
                {
                    mSongsListView.Items.Add(s);
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mSongDeleteButton_Click()
        ///
        /// Remove a song from a playlist.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mSongDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if (mSelectedPlaylistContent != null)
            {
                string buffer;
                int count = mSongsListView.SelectedItems.Count - 1;
                for (int i = count; i >= 0; --i)
                {
                    buffer = (mSongsListView.SelectedItems[i] as string);
                    mSelectedPlaylistContent.Remove(buffer);
                    mSongsListView.Items.Remove((buffer as string));
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mSongAddButton_Click()
        ///
        /// Add a new song to a playlist.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mSongAddButton_Click(object sender, RoutedEventArgs e)
        {
            if (mSelectedPlaylistContent != null)
            {
                if (mOpenFileDialog.ShowDialog().Value)
                {
                    foreach (string s in mOpenFileDialog.FileNames)
                    {
                        mSelectedPlaylistContent.Add(s);
                        mSongsListView.Items.Add(s);
                    }   
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.mSaveButton_Click()
        ///
        /// Save playlist.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void mSaveButton_Click(object sender, RoutedEventArgs e)
        {
            // Is there a name?
            if (mSelectedPlaylistContent != null && mPlaylistNameTextBox.Text != "")
            {
                RemoveRadioPlaylist(mSelectedPlaylist);
                AddRadioPlaylist(mPlaylistNameTextBox.Text, mSelectedPlaylistContent.ToArray(), mSelectedPlaylistContent.Count);

                DisplayPlaylists();
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void RadioOptionControl.ClearPlaylistInfo()
        ///
        /// Clear UI data.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ClearPlaylistInfo()
        {
            mPlaylistNameTextBox.Text = "";
            mSelectedPlaylistContent = null;
            mSongsListView.Items.Clear();
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


