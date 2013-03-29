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
        // members
        Dictionary<object, string> mGuidanceMessages;
        private int[] mPlayerTwoKeys;

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetKeyboardControl(int[] pControls);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetKeyboardControl(int[] pControls);

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.KeyboardOptionControl()
        ///
        /// Constructor.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public KeyboardOptionControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mBackToMainButton, "Return to the main menu"},
                {mBackToOptionButton, "Return to option menu"},
                {mSaveButton, "Save configuration"},
                {mUpKeyTextBox, "Click, then press a key"},
                {mDownKeyTextBox, "Click, then press a key"},
                {mLeftKeyTextBox, "Click, then press a key"},
                {mRightKeyTextBox, "Click, then press a key"},
            };

            mPlayerTwoKeys = new int[4];
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.mBackToOptionButton_Click()
        ///
        /// Back to option.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mBackToOptionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOptionsMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.mBackToMainButton_Click()
        ///
        /// Back to main menu.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mBackToMainButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.DisplayGuidanceMessages()
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
        /// @fn void KeyboardOptionControl.ClearGuidanceMessages()
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
        /// @fn void KeyboardOptionControl.DisplayPlayerTwoKeys()
        ///
        /// Display keyboard control.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void DisplayPlayerTwoKeys()
        {
            GetKeyboardControl(mPlayerTwoKeys);

            mUpKeyTextBox.Text    = KeyInterop.KeyFromVirtualKey(mPlayerTwoKeys[0]).ToString(); //((Key)(mPlayerTwoKeys[0])).ToString();
            mDownKeyTextBox.Text  = KeyInterop.KeyFromVirtualKey(mPlayerTwoKeys[1]).ToString();
            mLeftKeyTextBox.Text  = KeyInterop.KeyFromVirtualKey(mPlayerTwoKeys[2]).ToString();
            mRightKeyTextBox.Text = KeyInterop.KeyFromVirtualKey(mPlayerTwoKeys[3]).ToString();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.UpKeyInput()
        ///
        /// Up key.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void UpKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[0] = (int)KeyInterop.VirtualKeyFromKey(e.Key);
            (sender as TextBox).Text = e.Key.ToString();
            e.Handled = true;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.DownKeyInput()
        ///
        /// Configure down key.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void DownKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[1] = (int)KeyInterop.VirtualKeyFromKey(e.Key);
            (sender as TextBox).Text = e.Key.ToString();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.LeftKeyInput()
        ///
        /// Configure left key.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void LeftKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[2] = (int)KeyInterop.VirtualKeyFromKey(e.Key);
            (sender as TextBox).Text = e.Key.ToString();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.RightKeyInput()
        ///
        /// Configure right key.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void RightKeyInput(object sender, KeyEventArgs e)
        {
            mPlayerTwoKeys[3] = (int)KeyInterop.VirtualKeyFromKey(e.Key);  //(int)e.Key;// +21;
            (sender as TextBox).Text = e.Key.ToString();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void KeyboardOptionControl.mSaveButton_Click()
        ///
        /// Save configuration by calling the dll.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mSaveButton_Click(object sender, RoutedEventArgs e)
        {
            SetKeyboardControl(mPlayerTwoKeys);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


