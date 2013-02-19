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
    /// Logique d'interaction pour OptionsControl.xaml
    /// </summary>
    public partial class OptionsControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

        public OptionsControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mAIConfigureButton, "Configure AI profiles. AI are used for quick play and tournaments"},
                {mKeyboardOptionButton, "Configure controls"},
                {mRadioOptionButton, "Configure the radio. The radio will play music during your hockey games"},
                {mBackToMenuButton, "Return to main menu"},
            };
        }

        private void backToMenuButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        private void aiConfigureButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToAIOptions();
        }

        private void radioOptionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToRadioOptions();
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
