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

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour MainMenuControl.xaml
    /// </summary>
    public partial class MainMenuControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

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

#if DEBUG
            mUnitTestButton.Click += mUnitTestButton_Click;
            mUnitTestButton.Visibility = Visibility.Visible;
#else
            mUnitTestButton.Visibility = Visibility.Hidden;
#endif
        }

        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ExecuteUnitTest();

        void mUnitTestButton_Click(object sender, RoutedEventArgs e)
        {
            ExecuteUnitTest();
        }

        private void quickPlayButton_Click(object sender, RoutedEventArgs e)
        {
            mQuickPlayGroupBox.Visibility = Visibility.Visible;
        }

        private void editionModeButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToEditionMode();
        }

        private void tournamentButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToTournamentMenu();
        }

        private void onlineModeButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOnlineLobby();
        }

        private void optionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOptionsMenu();
        }

        private void DisplayGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = mGuidanceMessages[sender];
        }

        private void ClearGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = "";
        }

        public void InitOperations()
        {
            mQuickPlayGroupBox.Visibility = Visibility.Hidden;
        }

        private void mQuickPlayCancelButton_Click(object sender, RoutedEventArgs e)
        {
            mQuickPlayGroupBox.Visibility = Visibility.Hidden;
        }

        private void mQuickPlayGoButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToPlayMode();
        }
    }
}
