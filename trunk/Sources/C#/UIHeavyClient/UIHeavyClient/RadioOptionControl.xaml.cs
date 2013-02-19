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

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour RadioOptionControl1.xaml
    /// </summary>
    public partial class RadioOptionControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

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
    }
}
