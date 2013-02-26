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
    /// Logique d'interaction pour AIOptionControl.xaml
    /// </summary>
    public partial class AIOptionControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

        public AIOptionControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mAddButton, "Add a new AI profile"},
                {mDeleteButton, "Delete selected AI profile"},
                {mSaveButton, "Save modifications"},
                {mBackToOptionButton, "Return to option menu, don't forget to save!"},
                {mBackToMainButton, "Return to main menu, don't forget to save!"},
                {mSpeedSlider, "The speed tells how fast the AI will move"},
                {mReflexSlider, "The reflex tells how often the AI will succed hitting the puck"},
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
