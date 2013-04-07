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
using System.Windows.Forms.Integration;
using System.Runtime.InteropServices;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class PlayModeControl
    /// @brief Window to configure radio.
    ///
    /// @author Vincent Lemire
    /// @date 2013-02-06
    ///////////////////////////////////////////////////////////////////////////
    public partial class PlayModeControl : UserControl
    {
        private WindowsFormsHost mWindowsFormsHost;
        private bool mIsRadioPlaying;

        // For keyboard event binding
        private Dictionary<string, RoutedCommand> mRoutedCommands;

        // C++ Radio functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PlayRadioSong();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PauseRadioSong();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void StopRadioSong();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void NextRadioSong();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PreviousRadioSong();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetRadioVolume();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetRadioVolume(int pVolume);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetCurrentRadioPlaylist(string pPlaylist);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetCurrentRadioPlaylist(string pPlaylist);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlaylists();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void GetRadioPlaylists([In, Out] string[] pPlaylists, int pNbrPlaylists);
        [DllImport(@"RazerGame.dll")]
        static extern bool ActionPerformed(ActionType action);
        [DllImport(@"RazerGame.dll", CallingConvention=CallingConvention.Cdecl)]
        private static extern void AskForAIOpponentInNetworkGame();

        public PlayModeControl(WindowsFormsHost pWindowsFormsHost)
        {
            InitializeComponent();
            mWindowsFormsHost = pWindowsFormsHost;
            mIsRadioPlaying = true;

            // Keyboard events
            InitRoutedCommands();
        }

        public void AppendOpenGL()
        {
            if (!playControlGrid.Children.Contains(mWindowsFormsHost))
            {
                playControlGrid.Children.Add(mWindowsFormsHost);
                mWindowsFormsHost.Focus();
                System.Windows.Input.Keyboard.Focus(mWindowsFormsHost);
            }
            Grid.SetColumn(mWindowsFormsHost, 0);
            Grid.SetRow(mWindowsFormsHost, 0);
            Grid.SetRowSpan(mWindowsFormsHost, 1);
            Grid.SetColumnSpan(mWindowsFormsHost, 2);
        }

        public void RemoveOpenGL()
        {
            playControlGrid.Children.Remove(mWindowsFormsHost);
        }

        private void mPeviousButton_Click(object sender, RoutedEventArgs e)
        {
            PreviousRadioSong();
        }

        private void mNextButton_Click(object sender, RoutedEventArgs e)
        {
            NextRadioSong();
        }

        private void mStopButton_Click(object sender, RoutedEventArgs e)
        {
            StopRadioSong();
            mIsRadioPlaying = false;
        }

        private void mVolumeSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            SetRadioVolume((int)(sender as Slider).Value);
        }

        public void DisplayRadioVolume()
        {
            mVolumeSlider.Value = GetRadioVolume();
        }

        private void mPlaylistComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((sender as ComboBox).SelectedItem != null)
            {
                SetCurrentRadioPlaylist((sender as ComboBox).SelectedItem.ToString());
            }
        }

        private void mPlayButton_Click(object sender, RoutedEventArgs e)
        {
            if (mIsRadioPlaying)
            {
                PauseRadioSong();
                mIsRadioPlaying = false;
                (sender as Button).Content = "||";
            }
            else
            {
                PlayRadioSong();
                mIsRadioPlaying = true;
                (sender as Button).Content = "|>";
            }
        }

        public void DisplayRadioPlaylists()
        {
            // Clear what's on screen
            mPlaylistComboBox.Items.Clear();

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
                // Add it to combo box
                mPlaylistComboBox.Items.Add(s);
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void PlayModeControl.submitButton_Click()
        ///
        /// Event when the submit button is clicked.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void submitButton_Click(object sender, RoutedEventArgs e)
        {
            if (mChatInputTextbox.Text != ""/* && mIsUserConnected*/)
            {
                Chat.SendMessageGameDLL(mChatInputTextbox.Text);
                mChatInputTextbox.Clear();
            }
        }

        static bool MessageReceived(IntPtr pUsername, IntPtr pMessage, IntPtr pGroupName)
        {
            string message = Marshal.PtrToStringAnsi(pMessage);
            string username = Marshal.PtrToStringAnsi(pUsername);
            string groupname = Marshal.PtrToStringAnsi(pGroupName);
            if(groupname == "ingame")
            {
                MainWindowHandler.mTaskManager.ExecuteTask(() =>
                {
                    MainWindowHandler.Context.PlayModeControl.mChatOutputTextBox.Text += "\n" + username + ": " + message;
                    MainWindowHandler.Context.PlayModeControl.mChatOutputTextBox.ScrollToEnd();
                });
            }
            
            return true;
        }

        static bool EventReceived(EventCodes id, IntPtr pMessage)
        {
            if (id==EventCodes.GAME_SERVER_USER_DISCONNECTED)
            {
                MainWindowHandler.mTaskManager.ExecuteTask(() =>
                {
                    // Affiche l'option dans le menu
                    MainWindowHandler.Context.AddAIOpponentHandle(false);

                    // Demande si on veut continuer a jouer avec un joueur AI en attendant
                    MessageBoxResult dr=MessageBox.Show("Do you want to play againt an AI while waiting?", "Opponent disconnected", MessageBoxButton.YesNo);

                    if (dr==MessageBoxResult.Yes)
                    {
                        AskForAIOpponentInNetworkGame();
                    }
                    // Sinon, on reste en pause

                });
            }
            else if(id==EventCodes.GAME_SERVER_USER_CONNECTED)
            {
                // Cache l'option d'ajout de AI
                MainWindowHandler.Context.AddAIOpponentHandle(true);
            }
            else if (id==EventCodes.GAME_ENDED_CS)
            {
                MainWindowHandler.mTaskManager.ExecuteTask(() =>
                {
                    // On reaffiche l'option de replay dans le menu quand la partie est terminee
                    MainWindowHandler.Context.RestartGameMenuHandle(false);
                });
            }
            return true;
        }

        

        public static MessageReceivedCallBack mMessageCallback = MessageReceived;
        public static EventReceivedCallBack mEventCallback=EventReceived;

        #region Routed Commands

        private void InitRoutedCommands()
        {
            mRoutedCommands = new Dictionary<string, RoutedCommand>();

            // 10 seconds replay
            mRoutedCommands.Add("Replay", new RoutedCommand());
            mRoutedCommands["Replay"].InputGestures.Add(new KeyGesture(Key.R, ModifierKeys.Control));
            CommandBindings.Add(new CommandBinding(mRoutedCommands["Replay"], ReplayShortcut));
        }

        // Replay
        private void ReplayShortcut(object sender, ExecutedRoutedEventArgs e)
        {
            ActionPerformed(ActionType.ACTION_REPLAY);
        }

        #endregion
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


