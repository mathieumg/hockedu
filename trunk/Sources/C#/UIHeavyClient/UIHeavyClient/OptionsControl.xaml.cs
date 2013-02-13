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
        public OptionsControl()
        {
            InitializeComponent();
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
    }
}
