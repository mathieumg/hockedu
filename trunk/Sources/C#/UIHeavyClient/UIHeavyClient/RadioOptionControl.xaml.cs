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
    /// <summary>
    /// Logique d'interaction pour RadioOptionControl1.xaml
    /// </summary>
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
        }

        private void mBackToOptionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOptionsMenu();
        }

        private void mBackToMainButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        private void DisplayGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = mGuidanceMessages[sender];
        }

        private void ClearGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = "";
        }

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
                playlists[i] = "";
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
                    songs[i] = "";
                }

                GetPlaylistSongs(s, songs, nbrSongs);

                foreach (string s2 in songs)
                {
                    mPlaylistsContent[s].Add(s2);
                }
            }
        }

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

        private void mPlaylistDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            RemoveRadioPlaylist(mSelectedPlaylist);
            DisplayPlaylists();
        }

        private void mCurrentPlaylistsList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //mSongsEditGroupBox.IsEnabled = true;
            ClearPlaylistInfo();

            mSelectedPlaylist = (sender as ListView).SelectedItem.ToString();
            mSelectedPlaylistContent = mPlaylistsContent[mSelectedPlaylist];

            mPlaylistNameTextBox.Text = mSelectedPlaylist;

            foreach (string s in mSelectedPlaylistContent)
            {
                mSongsListView.Items.Add(s);
            }
        }

        private void mSongDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            foreach (System.Collections.IList l in mSongsListView.SelectedItems)
            {
                mSelectedPlaylistContent.Remove(l.ToString());
            }

            mSongsListView.SelectedItems.Clear();
        }

        private void mSongAddButton_Click(object sender, RoutedEventArgs e)
        {
            mOpenFileDialog.Title = "Select a song file";

            if (mOpenFileDialog.ShowDialog().Value)
            {
                mSelectedPlaylistContent.Add(mOpenFileDialog.FileName);
                mSongsListView.Items.Add(mOpenFileDialog.FileName);
            }
        }

        private void mSaveButton_Click(object sender, RoutedEventArgs e)
        {
            // Is there a name?
            if (mPlaylistNameTextBox.Text != "")
            {
                // If we change the name, delete old playlist
                if (mPlaylistNameTextBox.Text != mSelectedPlaylist)
                {
                    RemoveRadioPlaylist(mSelectedPlaylist);
                }

                AddRadioPlaylist(mPlaylistNameTextBox.Text, mSelectedPlaylistContent.ToArray(), mSelectedPlaylistContent.Count);

                DisplayPlaylists();
            }
        }

        private void ClearPlaylistInfo()
        {
            mPlaylistNameTextBox.Text = "";
            mSelectedPlaylistContent = null;
            mSongsListView.Items.Clear();
        }
    }
}
