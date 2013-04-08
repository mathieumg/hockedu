///////////////////////////////////////////////////////////////////////////////
/// @file AIOptionControl.xaml.cs
/// @author Vincent Lemire
/// @date 2013-01-28
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
using Microsoft.Win32;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @struct AIProfile
    /// @brief To receive an AI profile from the dll.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    struct AIProfile
    {
        public string Name;
        public int Speed;
        public int FailProb;
        public bool IsLearning;
        public string FilePath;

        public AIProfile(string pName, int pSpeed, int pFailProb, bool pIsLearning, string pFilePath)
        {
            Name = pName;
            Speed = pSpeed;
            FailProb = pFailProb;
            IsLearning = pIsLearning;
            FilePath = pFilePath;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @class AIOptionControl
    /// @brief Window to configure the AI profiles.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class AIOptionControl : UserControl
    {
        // Members
        Dictionary<object, string> mGuidanceMessages;
        Dictionary<string, AIProfile> mProfiles;
        string mSelectedProfile;

        OpenFileDialog mOpenFileDialog;

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void AddPlayer(string pName, int pSpeed, int pFailProb, bool pIsLearning, string pFilePath);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void RemovePlayer(string pName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlayers();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPlayers([In, Out] AIProfile[] pProfiles, int pNbrProfiles);

        ////////////////////////////////////////////////////////////////////////
        /// @fn AIOptionControl.AIOptionControl()
        ///
        /// Constructor.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
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

            mProfiles = new Dictionary<string, AIProfile>();
            mSelectedProfile = "";

            mOpenFileDialog = new OpenFileDialog();
            mOpenFileDialog.Multiselect = false;
            mOpenFileDialog.Filter = "AiLogic (*.ailogic)|*.ailogic";
            mOpenFileDialog.Title = "Choose a reinforcement AI profile";
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void AIOptionControl.mBackToOptionButton_Click()
        ///
        /// Back to options menu.
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
        /// @fn void AIOptionControl.mBackToMainButton_Click()
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
        /// @fn void AIOptionControl.DisplayGuidanceMessages()
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
        /// @fn void AIOptionControl.ClearGuidanceMessages()
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
        /// @fn void AIOptionControl.mAddButton_Click()
        ///
        /// Add a new AI profile.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mAddButton_Click(object sender, RoutedEventArgs e)
        {
            string newName = "New AI";
            int i = 1;

            while (mProfiles.ContainsKey(newName))
            {
                newName = "New AI " + (++i); 
            }

            AddPlayer(newName, 1, 0, false, "");

            MainWindowHandler.LaunchAchievementEvent(AchievementEvent.ACHIEVEMENT_EVENT_AI_CREATED);

            DisplayAIProfiles();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void AIOptionControl.mDeleteButton_Click()
        ///
        /// Delete the selected AI profile.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if (mSelectedProfile != "")
            {
                string buffer;
                int count = mProfilesListView.SelectedItems.Count - 1;
                for (int i = count; i >= 0; --i)
                {
                    buffer = (mProfilesListView.SelectedItems[i] as string);
                    mProfilesListView.Items.Remove((buffer as string));
                    RemovePlayer(buffer);
                }   
            }

            DisplayAIProfiles();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void AIOptionControl.mSaveButton_Click()
        ///
        /// Save selected AI profile.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mSaveButton_Click(object sender, RoutedEventArgs e)
        {
            if (mSelectedProfile != "" && mProfileNameTextBox.Text != "")
            {
                RemovePlayer(mSelectedProfile);

                AddPlayer(mProfileNameTextBox.Text, (int)mSpeedSlider.Value, (int)mReflexSlider.Value, mLearningCheckbox.IsChecked.Value, mLearningTextbox.Text);
            }
            else if (mProfileNameTextBox.Text != "")
            {
                RemovePlayer(mProfileNameTextBox.Text);
                AddPlayer(mProfileNameTextBox.Text, (int)mSpeedSlider.Value, (int)mReflexSlider.Value, mLearningCheckbox.IsChecked.Value, mLearningTextbox.Text);
                MainWindowHandler.LaunchAchievementEvent(AchievementEvent.ACHIEVEMENT_EVENT_AI_CREATED);
            }


            DisplayAIProfiles();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void AIOptionControl.DisplayAIProfiles()
        ///
        /// Query Ai profiles from XML.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void DisplayAIProfiles()
        {
            ClearProfileInfo();
            mProfiles.Clear();
            mProfilesListView.Items.Clear();

            int nbrProfiles = GetNbrPlayers();
            AIProfile[] profiles = new AIProfile[nbrProfiles];

            for (int i = 0; i < nbrProfiles; ++i)
            {
                profiles[i] = new AIProfile(new string('s', 255), 1, 0, false, new string('s', 255));
            }

            GetPlayers(profiles, nbrProfiles);

            
            foreach (AIProfile p in profiles)
            {
                if (!mProfiles.ContainsKey(p.Name))
                {
                    mProfiles.Add(p.Name, p);
                    mProfilesListView.Items.Add(p.Name);
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void AIOptionControl.mCurrentAIList_SelectionChanged()
        ///
        /// Diplay selected AI profile on the right side.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void mCurrentAIList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((sender as ListView).SelectedItems.Count > 0)
            {
                mSelectedProfile = (sender as ListView).SelectedItem.ToString();
                AIProfile selectedProfile = mProfiles[mSelectedProfile];

                mProfileNameTextBox.Text = selectedProfile.Name;
                mSpeedSlider.Value = selectedProfile.Speed;
                mReflexSlider.Value = selectedProfile.FailProb;
                mLearningCheckbox.IsChecked = selectedProfile.IsLearning;
                mLearningTextbox.Text = selectedProfile.FilePath;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void AIOptionControl.ClearProfileInfo()
        ///
        /// Clear the selected profile infos.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ClearProfileInfo()
        {
            mProfileNameTextBox.Text = "";
            mSpeedSlider.Value = 0;
            mReflexSlider.Value = 0;
            mLearningCheckbox.IsChecked = false;
            mLearningTextbox.Text = "";

            mSelectedProfile = "";
        }

        private void mBrowseBotton_Click(object sender, RoutedEventArgs e)
        {
            if (mOpenFileDialog.ShowDialog().Value)
            {
                mLearningTextbox.Text = mOpenFileDialog.FileName;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
