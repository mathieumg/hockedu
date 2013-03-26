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
using System.Windows.Shapes;
using HttpHockeduRequests;

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour GameCreationPrompt.xaml
    /// </summary>
    public partial class GameCreationPrompt : Window
    {

        public void callbackMapsFunction(List<UserMapDetailedJSON> pList)
        {
            // Update the combobox with the list
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                foreach (UserMapDetailedJSON wItem in pList)
                {
                    mMapComboBox.Items.Add(wItem);
                }
            });
            
        }

        private bool mOkIsClicked;

        public string Name
        {
            get { return mNameTextBox.Text; }
        }
        public string Password
        {
            get { return mPasswordCheckBox.IsChecked.Value ? mNameTextBox.Text : ""; }
        }
        public string Map
        {
            get { return mMapComboBox.SelectedItem.ToString(); }
        }
        public bool OkIsClicked
        {
            get { return mOkIsClicked; }
        }

        public GameCreationPrompt()
        {
            InitializeComponent();
            mOkIsClicked = false;
        }

        private void mOkButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = true;
            Close();
        }

        private void mCancelButton_Click(object sender, RoutedEventArgs e)
        {
            mOkIsClicked = false;
            Close();
        }

        public void ClearInputAndLoadMapList()
        {
            mNameTextBox.Clear();
            mPasswordCheckBox.IsChecked = false;
            mPasswordTextBox.Clear();
            mFeedbackLabel.Content = "";
            GetServerMaps();
        }

        private void GetServerMaps()
        {
            mMapComboBox.Items.Clear();

            // Load map list async!!!!
            HttpManager wManager = new HttpManager();
            wManager.getPublicMapList(callbackMapsFunction);
        }
    }
}
