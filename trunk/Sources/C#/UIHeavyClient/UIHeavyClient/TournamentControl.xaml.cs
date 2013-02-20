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
    /// Logique d'interaction pour TournamentControl.xaml
    /// </summary>
    public partial class TournamentControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

        public TournamentControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mAddButton, "Add a new tournament in the list"},
                {mDeleteButton, "Delete the selected tournament"},
                {mSaveButton, "Save the tournament"},
                {mTournamentBeginButton, "Let the show begin!!!"},
                {mBackToMainButton, "Return to the main menu"},
            };
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
