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

        private Dictionary<string, List<string>> mPlaylists;
        private string mSelectedPlaylist;
        

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlaylists();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetRadioPlaylists(string[] pPlaylists, int pNbrPlaylists);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrSongs(string pPlaylist);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPlaylistSongs(string pPlaylist, string[] pSongs, int pNbrSongs);
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

            mPlaylists = new Dictionary<string, List<string>>();
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
            int nbrPlaylists = GetNbrPlaylists();

            string[] playlists = new string[nbrPlaylists];
            for (int i = 0; i < nbrPlaylists; ++i)
            {
                playlists[i] = "";
            }

            GetRadioPlaylists(playlists, nbrPlaylists);

            foreach (string s in playlists)
            { 
                mPlaylists.Add(s, new List<string>());
                mCurrentPlaylistsList.Items.Add(s);

                int nbrSongs = GetNbrSongs(s);

                string[] songs = new string[nbrSongs];
                for (int i = 0; i < nbrSongs; ++i)
                {
                    songs[i] = "";
                }

                GetPlaylistSongs(s, songs, nbrSongs);

                foreach (string s2 in songs)
                {
                    mPlaylists[s].Add(s2);
                }
            }
        }

        private void mPlaylistAddButton_Click(object sender, RoutedEventArgs e)
        {
            AddRadioPlaylist("New PLaylist", new string[0], 0);
        }

        private void mPlaylistDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            RemoveRadioPlaylist(mCurrentPlaylistsList.SelectedItem.ToString());
        }

        private void mCurrentPlaylistsList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            mSelectedPlaylist = (sender as ListView).SelectedItems.ToString();

            foreach (string s in mPlaylists[mSelectedPlaylist])
            {
                mSongsList.Items.Add(s);
            }
        }
    }
}
