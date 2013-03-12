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
    /// Logique d'interaction pour KeyboardOptionControl.xaml
    /// </summary>
    public partial class KeyboardOptionControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetKeyboardControl(int[] pControls);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetKeyboardControl(int pUp, int pDown, int pLeft, int pRight);

        public KeyboardOptionControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mBackToMainButton, "Return to the main menu"},
                {mBackToOptionButton, "Return to option menu"},
                {mSaveButton, "Save configuration"},
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
