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
    /// Struct for AI profiles
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    struct AIProfile
    {
        public string Name;
        public int Speed;
        public int FailProb;

        public AIProfile(string pName, int pSpeed, int pFailProb)
        {
            Name = pName;
            Speed = pSpeed;
            FailProb = pFailProb;
        }
    }

    /// <summary>
    /// Logique d'interaction pour AIOptionControl.xaml
    /// </summary>
    public partial class AIOptionControl : UserControl
    {
        Dictionary<object, string> mGuidanceMessages;
        Dictionary<string, AIProfile> mProfiles;
        string mSelectedProfile;

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void AddPlayer(string pName, int pSpeed, int pFailProb);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void RemovePlayer(string pName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlayers();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPlayers([In, Out] AIProfile[] pProfiles, int pNbrProfiles);

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

        private void mAddButton_Click(object sender, RoutedEventArgs e)
        {
            string newName = "New AI";
            int i = 1;

            while (mProfiles.ContainsKey(newName))
            {
                newName = "New AI " + (++i); 
            }

            AddPlayer(newName, 1, 0);

            DisplayAIProfiles();
        }

        private void mDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if (mSelectedProfile != "")
            {
                RemovePlayer(mSelectedProfile);
            }

            DisplayAIProfiles();
        }

        private void mSaveButton_Click(object sender, RoutedEventArgs e)
        {
            if (mSelectedProfile != "" && mProfileNameTextBox.Text != "")
            {
                RemovePlayer(mSelectedProfile);

                AddPlayer(mProfileNameTextBox.Text, (int)mSpeedSlider.Value, (int)mReflexSlider.Value);
            }

            DisplayAIProfiles();
        }

        public void DisplayAIProfiles()
        {
            ClearProfileInfo();
            mProfiles.Clear();
            mProfilesListView.Items.Clear();

            int nbrProfiles = GetNbrPlayers();
            AIProfile[] profiles = new AIProfile[nbrProfiles];

            for (int i = 0; i < nbrProfiles; ++i)
            {
                profiles[i] = new AIProfile(new string('s', 255), 1, 0);
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

        private void mCurrentAIList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((sender as ListView).SelectedItems.Count > 0)
            {
                mSelectedProfile = (sender as ListView).SelectedItem.ToString();
                AIProfile selectedProfile = mProfiles[mSelectedProfile];

                mProfileNameTextBox.Text = selectedProfile.Name;
                mSpeedSlider.Value = selectedProfile.Speed;
                mReflexSlider.Value = selectedProfile.FailProb;
            }
        }

        private void ClearProfileInfo()
        {
            mProfileNameTextBox.Text = "";
            mSpeedSlider.Value = 0;
            mReflexSlider.Value = 0;

            mSelectedProfile = "";
        }
    }
}
