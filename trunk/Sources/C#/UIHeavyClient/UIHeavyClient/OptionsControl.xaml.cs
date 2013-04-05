///////////////////////////////////////////////////////////////////////////////
/// @file OptionsControl.xaml.cs
/// @author Vincent Lemire and Micheal Ferris
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
    /// @class OptionsControl
    /// @brief Window for options menu.
    ///
    /// @author Vincent Lemire
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public partial class OptionsControl : UserControl
    {
        // Members
        Dictionary<object, string> mGuidanceMessages;

        // C++ function
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetSoundVolume();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetSoundVolume(int pVolume);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetEffectVolume();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetEffectVolume(int pVolume);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool GetIsHouseDisplay();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetIsHouseDisplay(bool pIsHouseDisplay);

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.OptionsControl()
        ///
        /// Constructor.
        ///
        /// @return none.
        ////////////////////////////////////////////////////////////////////////
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
            DisplayOptionsValue();

            // On affecte les callback apres avoirlu les valeurs du modele pour ne pas les ecraser
            mVolumeSlider.ValueChanged += mVolumeSlider_ValueChanged_1;
            mEffectSlider.ValueChanged += mEffectSlider_ValueChanged;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.DisplaySoundVolume()
        ///
        /// Ajust volume slide bars.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void DisplayOptionsValue()
        {
            mVolumeSlider.Value = GetSoundVolume();
            mEffectSlider.Value = GetEffectVolume();
            mHouseCheckBox.IsChecked = GetIsHouseDisplay();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.backToMenuButton_Click()
        ///
        /// Return to main menu.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void backToMenuButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.aiConfigureButton_Click()
        ///
        /// Go to AI profiles configuration.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void aiConfigureButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToAIOptions();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.radioOptionButton_Click()
        ///
        /// Go to radio configuration button.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void radioOptionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToRadioOptions();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.DisplayGuidanceMessages()
        ///
        /// Display Bastien & Scapin.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] MouseEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void DisplayGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = mGuidanceMessages[sender];
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.ClearGuidanceMessages()
        ///
        /// Clear Bastien & Scapin.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] MouseEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ClearGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = "";
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.mKeyboardOptionButton_Click()
        ///
        /// Go to keyboard configuration.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mKeyboardOptionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToKeyboardOption();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.mVolumeSlider_ValueChanged_1()
        ///
        /// Music volume change.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedPropertyChangedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mVolumeSlider_ValueChanged_1(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            SetSoundVolume((int)(sender as Slider).Value);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void OptionsControl.mEffectSlider_ValueChanged()
        ///
        /// Effect volume change.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedPropertyChangedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mEffectSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            SetEffectVolume((int)(sender as Slider).Value);
        }

        private void mHouseCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            SetIsHouseDisplay((sender as CheckBox).IsChecked.Value);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


