///////////////////////////////////////////////////////////////////////////////
/// @file TournamentControl.xaml.cs
/// @author Vincent Lemire
/// @date 2013-02-10
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
    /// @class TournamentControl
    /// @brief Window to configure tournaments.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class TournamentControl : UserControl
    {
        // C++ functions
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetNbrPlayers();
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPlayers([In, Out] AIProfile[] pProfiles, int pNbrProfiles);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void BeginNewTournament(string pTournamentName, string pMapName, string[] pPlayerNames, int pNbrPlayers);
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ContinueExistingTournament(string pTournamentName);

        Dictionary<object, string> mGuidanceMessages;

        // File dialog
        Microsoft.Win32.OpenFileDialog mMapFileDialog;
        Microsoft.Win32.OpenFileDialog mTournamentFileDialog;

        // To easily access some element in a loop
        List<RadioButton> mHumanRadios;
        List<ComboBox> mAIComboBoxes;

        public TournamentControl()
        {
            InitializeComponent();

            mGuidanceMessages = new Dictionary<object, string>()
            {
                {mTournamentBeginButton, "Let the show begin!!!"},
                {mBackToMainButton, "Return to the main menu"},
            };

            mHumanRadios = new List<RadioButton>()
            {
                mHuman1Radio, mHuman2Radio, mHuman3Radio, mHuman4Radio, mHuman5Radio, mHuman6Radio, mHuman7Radio, mHuman8Radio, mHuman9Radio, mHuman10Radio, mHuman11Radio, mHuman12Radio, mHuman13Radio, mHuman14Radio, mHuman15Radio, mHuman16Radio,
            };

            mAIComboBoxes = new List<ComboBox>()
            {
                mAI1ComboBox, mAI2ComboBox, mAI3ComboBox, mAI4ComboBox, mAI5ComboBox, mAI6ComboBox, mAI7ComboBox, mAI8ComboBox, mAI9ComboBox, mAI10ComboBox, mAI11ComboBox, mAI12ComboBox, mAI13ComboBox, mAI14ComboBox, mAI15ComboBox, mAI16ComboBox, 
            };

            mMapFileDialog = new Microsoft.Win32.OpenFileDialog();
            mMapFileDialog.Multiselect = false;
            mMapFileDialog.Filter = "XML Files (*.xml)|*.xml";
            mMapFileDialog.Title = "Choose a map file";

            mTournamentFileDialog = new Microsoft.Win32.OpenFileDialog();
            mTournamentFileDialog.Multiselect = false;
            mTournamentFileDialog.Filter = "XML Files (*.xml)|*.xml";
            mTournamentFileDialog.Title = "Choose a tournament file";
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

        private void mTournamentLoadButton_Click(object sender, RoutedEventArgs e)
        {
            if (mTournamentFileDialog.ShowDialog().Value)
            {
                ContinueExistingTournament(mTournamentFileDialog.FileName);
                MainWindowHandler.GoToPlayMode(ActionType.ACTION_ALLER_MODE_TOURNOI);
            }
        }

        public void DisplayProfileNames()
        {
            foreach (ComboBox cb in mAIComboBoxes)
            {
                cb.Items.Clear();
            }

            int nbrProfiles = GetNbrPlayers();
            AIProfile[] profiles = new AIProfile[nbrProfiles];

            for (int i = 0; i < nbrProfiles; ++i)
            {
                profiles[i] = new AIProfile(new string('s', 255), 1, 0);
            }

            GetPlayers(profiles, nbrProfiles);


            foreach (AIProfile p in profiles)
            {
                foreach (ComboBox cb in mAIComboBoxes)
                {
                    cb.Items.Add(p.Name);
                }
            }
        }

        private void mLoadMapButton_Click(object sender, RoutedEventArgs e)
        {
            if (mMapFileDialog.ShowDialog().Value)
            {
                mMapNameTextBox.Text = mMapFileDialog.FileName;
            }
        }

        private void mTournamentBeginButton_Click(object sender, RoutedEventArgs e)
        {
            string mapName = mMapNameTextBox.Text;
            string tournamentName = mTournamentNameTextBox.Text;
            int nbrPlayers = mAIComboBoxes.Count;
            string[] playersNames = new string[nbrPlayers];

            if (tournamentName == "")
            {
                MessageBox.Show("Please enter a name for the tournament!");
                return;
            }
            if (mapName == "")
            {
                MessageBox.Show("Please select a map for the tournament!");
                return;
            }

            for (int i = 0; i < nbrPlayers; ++i)
            {
                bool isHuman = mHumanRadios[i].IsChecked.Value;

                if (!isHuman)
                {
                    if (mAIComboBoxes[i].SelectedItem == null)
                    {
                        MessageBox.Show("Please choose a profile for player " + (i + 1) + "!");
                        return;
                    }

                    playersNames[i] = mAIComboBoxes[i].SelectedItem.ToString();
                }
                else 
                {
                    playersNames[i] = "";
                }
            }

            BeginNewTournament(tournamentName, mapName, playersNames, nbrPlayers);
            MainWindowHandler.GoToPlayMode(ActionType.ACTION_ALLER_MODE_TOURNOI);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
