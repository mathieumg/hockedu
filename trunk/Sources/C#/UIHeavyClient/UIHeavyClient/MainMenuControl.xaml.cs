///////////////////////////////////////////////////////////////////////////////
/// @file MainMenuControl.xaml.cs
/// @author Vincent Lemire
/// @date 2013-02-26
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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
using Microsoft.Win32;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class MainMenuControl
    /// @brief Window for the main menu.
    ///
    /// @author Vincent Lemire
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public partial class MainMenuControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

        // To load the map for quick play
        OpenFileDialog mOpenFileDialog;
        private LoginControl mLoginControl;

        public LoginControl LoginControlElement
        {
            get { return mLoginControl; }
            set { mLoginControl = value; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.MainMenuControl()
        ///
        /// Constructor.
        ///
        /// @return none.
        ////////////////////////////////////////////////////////////////////////
        public MainMenuControl()
        {
            InitializeComponent();
            InitOperations();

            mGuidanceMessages = new Dictionary<object, string>() 
            { 
                {mEditionModeButton, "The edition mode allows you to create your own map"},
                {mQuickPlayButton, "Set up a simple game and play!"},
                {mTournamentButton, "Create and play 16 players tournaments"},
                {mOnlineModeButton, "Play online with your friends! You must have a Hockedu profile"},
                {mOptionButton, "Create AI profiles, set the radio and the keyboard options"},
                {mUnitTestButton, "If you see that button, please contact us... NOW!"},
                {mHumanRadio, "Choose this to play against a second player"},
                {mAIRadio, "Choose this to play against the computer"},
                {mAIComboBox, "Choose a profile. You can create profiles by going in the option menu"},
            };

#if !SHIPPING
            mUnitTestButton.Click += mUnitTestButton_Click;
            mUnitTestButton.Visibility = Visibility.Visible;
#else
            mUnitTestButton.Visibility = Visibility.Hidden;
#endif

            mOpenFileDialog = new OpenFileDialog();
            mOpenFileDialog.Multiselect = false;
            mOpenFileDialog.Filter = "XML Files (*.xml)|*.xml";
            mOpenFileDialog.Title = "Choose a map";

            mLoginControl = new LoginControl( MainWindowHandler.GoToOnlineLobby, false );
        }

        // C++ function
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ExecuteUnitTest();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetSecondPlayer(bool pIsHuman, string pName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlayers();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPlayers([In, Out] AIProfile[] pProfiles, int pNbrProfiles);

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.mUnitTestButton_Click()
        ///
        /// Execute unit tests from last year project.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void mUnitTestButton_Click(object sender, RoutedEventArgs e)
        {
            ExecuteUnitTest();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.quickPlayButton_Click()
        ///
        /// Enable the box to start a game.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void quickPlayButton_Click(object sender, RoutedEventArgs e)
        {
            mQuickPlayGroupBox.Visibility = Visibility.Visible;
            mOnlineGroupBox.Visibility = Visibility.Hidden;
            DisplayProfileNames();
            (sender as Button).IsDefault = false;
            mQuickPlayGoButton.IsDefault = true;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.editionModeButton_Click()
        ///
        /// Go to edition mode.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void editionModeButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToEditionMode();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.tournamentButton_Click()
        ///
        /// Go to tournament configuration.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void tournamentButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToTournamentMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.onlineModeButton_Click()
        ///
        /// Enable the box to connect online.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void onlineModeButton_Click(object sender, RoutedEventArgs e)
        {
            mOnlineGroupBox.Visibility = Visibility.Visible;
            mQuickPlayGroupBox.Visibility = Visibility.Hidden;
            mLoginControl.SetFocusToUserName();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.optionButton_Click()
        ///
        /// Go to option menu.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void optionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOptionsMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.DisplayGuidanceMessages()
        ///
        /// Display Bastien & Scapin.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void DisplayGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = mGuidanceMessages[sender];
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.ClearGuidanceMessages()
        ///
        /// Clear Bastien & Scapin.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ClearGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = "";
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.InitOperations()
        ///
        /// When you go to main menu.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void InitOperations()
        {
            mQuickPlayGroupBox.Visibility = Visibility.Hidden;
            mOnlineGroupBox.Visibility = Visibility.Hidden;
            mOnlineContentControl.Content = mLoginControl;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.mQuickPlayCancelButton_Click()
        ///
        /// Cancel going to a game.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mQuickPlayCancelButton_Click(object sender, RoutedEventArgs e)
        {
            mQuickPlayGroupBox.Visibility = Visibility.Hidden;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.mQuickPlayGoButton_Click()
        ///
        /// Start a game.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mQuickPlayGoButton_Click(object sender, RoutedEventArgs e)
        {
            bool isHuman = mHumanRadio.IsChecked.Value;
            string name = "";

            if (!isHuman)
            {
                if (mAIComboBox.SelectedItem == null)
                    return;

                name = mAIComboBox.SelectedItem.ToString();
            }

            SetSecondPlayer(isHuman, name);
            MainWindowHandler.LoadPlayingMap(mMapTextBox.Text);
            MainWindowHandler.GoToPlayMode(ActionType.ACTION_ALLER_MODE_JEU);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.button1_Click()
        ///
        /// Load a map.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void button1_Click(object sender, RoutedEventArgs e)
        {
            if (mOpenFileDialog.ShowDialog().Value)
            {
                mMapTextBox.Text = mOpenFileDialog.FileName;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainMenuControl.DisplayProfileNames()
        ///
        /// Display AI profiles into combo box.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void DisplayProfileNames()
        {
            mAIComboBox.Items.Clear();

            int nbrProfiles = GetNbrPlayers();
            AIProfile[] profiles = new AIProfile[nbrProfiles];

            for (int i = 0; i < nbrProfiles; ++i)
            {
                profiles[i] = new AIProfile(new string('s', 255), 1, 0);
            }

            GetPlayers(profiles, nbrProfiles);


            foreach (AIProfile p in profiles)
            {
                mAIComboBox.Items.Add(p.Name);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////

