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
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    struct AIProfile
    {
        [MarshalAs(UnmanagedType.LPStr)]
        public string Name;
        [MarshalAs(UnmanagedType.LPStr)]
        public string OriginName;
        public int Speed;
        public int FailProb;

        public AIProfile(string pName, int pSpeed, int pFailProb)
        {
            Name = pName;
            OriginName = pName;
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
        AIProfile? mCurrentEdition;

        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void AddPlayer(string pName, int pSpeed, int pFailProb);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl, CharSet=CharSet.Auto)]
        private static extern void RemovePlayer(string pName);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlayers();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPlayers(AIProfile[] pProfiles, int pNbrProfiles);

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
            mCurrentEdition = null;
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
            AddPlayer("New AI", 1, 0);

            DisplayAIProfiles();
        }

        private void mDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if (mCurrentEdition != null)
            {
                RemovePlayer(mCurrentEdition.Value.OriginName);
            }

            DisplayAIProfiles();
        }

        private void mSaveButton_Click(object sender, RoutedEventArgs e)
        {
            if (mCurrentEdition != null)
            {
                if (mProfileNameTextBox.Text != mCurrentEdition.Value.OriginName)
                    RemovePlayer(mCurrentEdition.Value.OriginName);

                AddPlayer(mProfileNameTextBox.Text, (int)mSpeedSlider.Value, (int)mReflexSlider.Value);
            }
        }

        public void DisplayAIProfiles()
        {
            int nbrProfiles = GetNbrPlayers();
            AIProfile[] profiles = new AIProfile[nbrProfiles];

            for (int i = 0; i < nbrProfiles; ++i)
            {
                profiles[i] = new AIProfile("ouin ouin...", 1, 0);
            }

            GetPlayers(profiles, nbrProfiles); // PROBLEM HERE, le string ne change pas

            mProfiles.Clear();
            mCurrentAIList.Items.Clear();

            foreach (AIProfile p in profiles)
            {
                if (!mProfiles.ContainsKey(p.Name))
                {
                    mProfiles.Add(p.Name, p);
                    mCurrentAIList.Items.Add(p.Name);
                }
            }
        }

        private void mCurrentAIList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (mProfiles.Count > 0)
            {
                mCurrentEdition = mProfiles[(sender as ListView).SelectedItem.ToString()];
            }

            if (mCurrentEdition != null)
            {
                mProfileNameTextBox.Text = mCurrentEdition.Value.OriginName;
                mSpeedSlider.Value = mCurrentEdition.Value.Speed;
                mReflexSlider.Value = mCurrentEdition.Value.FailProb;
            }
        }
    }
}
