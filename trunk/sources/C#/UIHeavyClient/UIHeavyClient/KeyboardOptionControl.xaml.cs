///////////////////////////////////////////////////////////////////////////////
/// @file KeyboardOptionControl.xaml.cs
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
    ///////////////////////////////////////////////////////////////////////////
    /// @class KeyboardOptionControl
    /// @brief Window for configuring the keyboard.
    ///
    /// @author Vincent Lemire
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public partial class KeyboardOptionControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetKeyboardControl(int[] pControls);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetKeyboardControl(int[] pControls);

        private int[] mPlayerTwoKeys;

        public KeyboardOptionControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mBackToMainButton, "Return to the main menu"},
                {mBackToOptionButton, "Return to option menu"},
                {mSaveButton, "Save configuration"},
            };

            mPlayerTwoKeys = new int[4];
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

        public void DisplayPlayerTwoKeys()
        {
            GetKeyboardControl(mPlayerTwoKeys);

            mUpKeyTextBox.Text = ""+(Key)mPlayerTwoKeys[0];
            mDownKeyTextBox.Text = "" + (Key)mPlayerTwoKeys[1];
            mLeftKeyTextBox.Text = "" + (Key)mPlayerTwoKeys[2];
            mRightKeyTextBox.Text = "" + (Key)mPlayerTwoKeys[3];
        }

        private void UpKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[0] = (int)e.Key;
            (sender as TextBox).Text = e.ToString();
        }

        private void DownKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[1] = (int)e.Key;
            (sender as TextBox).Text = e.ToString();
        }

        private void LeftKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[2] = (int)e.Key;
            (sender as TextBox).Text = e.ToString();
        }

        private void RightKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[3] = (int)e.Key;
            (sender as TextBox).Text = e.ToString();
        }

        private void mSaveButton_Click(object sender, RoutedEventArgs e)
        {
            SetKeyboardControl(mPlayerTwoKeys);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


